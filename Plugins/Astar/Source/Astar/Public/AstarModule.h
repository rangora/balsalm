// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>
#include "CoreMinimal.h"
#include "Runtime/AIModule/Public/GraphAStar.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
//#include "GameFramework/Actor.h"
//#include "AstarModule.generated.h"


static const int POOLSIZE = 128;
static const int SEGMENT = 60;
static const int RADIUS = SEGMENT / 2;
static const int WIDTH = SEGMENT;
static const int HEIGHT = SEGMENT;




struct FNode {
	FNode() 
	: bBlocked(false),
	bOpen(true),
	index(-1),
	parentIndex(-1),
	TraversalCost(1e9),
	TotalCost(1e9){}

	FVector Pos;
	bool bBlocked;
	bool bOpen;
	int32 index;
	int32 parentIndex;
	int32 TraversalCost;
	int32 TotalCost;
	TArray<int32> AdjacentIndex;

	bool operator==(const FNode& Other) const {
		return Pos.Equals(Other.Pos);
	}

	friend uint32 GetTypeHash(const FNode& Other) {
		return GetTypeHash(Other.Pos.X + Other.Pos.Y + Other.Pos.Z);
	}
};

struct FNodeList {
	FNodeList(TMap<int32,FNode>& ArgNodeMap) 
		: NodeMapRef(ArgNodeMap){}

	void HeapPush(int32 NodeIndex) {
		IndexList.HeapPush(NodeIndex, [&](const int32 lhs, const int32 rhs) {
			return NodeMapRef[lhs].TotalCost < NodeMapRef[rhs].TotalCost;
			});
	}

	void SetNodeMapRef(TMap<int32, FNode>& ArgNodeMap) {
		NodeMapRef = ArgNodeMap;
	}

	void Clear() {
		IndexList.Empty();
	}

	int HeapPop() {
		int outputIndex = -1;

		if (IndexList.Num() > 0) {
			IndexList.HeapPop(outputIndex, [&](const int32 lhs, const int32 rhs) {
				return NodeMapRef[lhs].TotalCost < NodeMapRef[rhs].TotalCost;
				}, false);
		}
		return outputIndex;
	}

	void AddToOpenList(int32 NodeIndex) {
		if (!IndexList.Contains(NodeIndex))
			HeapPush(NodeIndex);
	}

	void AddToCloseList(int32 NodeIndex) {
		HeapPush(NodeIndex);
		NodeMapRef[NodeIndex].bOpen = false;
	}

	TMap<int32, FNode>& NodeMapRef;
	TArray<int32> IndexList;
};


struct FGraph {
	TSet<FNode> NodeSet;
	TMap<int32, FNode> NodeMap;
	TArray<FVector> Direction;
	FNodeList* OpenList;
	FNodeList* CloseList;
	UWorld* InWorld;
	

	FGraph() 
		: InWorld(nullptr) {
		CreateDirection();
		OpenList = new FNodeList(NodeMap);
		CloseList = new FNodeList(NodeMap);
	}
	~FGraph() {
		delete OpenList;
		delete CloseList;
	}

	FNode* FindOrAdd(FVector& argPos) {
		const float radius = 32.f;
		FNode tNode;
		tNode.Pos = argPos;

		auto ele = NodeSet.Find(tNode);
		if (ele == nullptr) {
			tNode.index = NodeMap.Num();
			TArray<AActor*> ActorsToIgnore;
			TArray<AActor*> ActorsToResult;
			ECollisionChannel TraceChannel = ECC_Pawn;
			TArray<FOverlapResult> OutHits;

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
			//ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
			//ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

			UKismetSystemLibrary::SphereOverlapActors((UObject*)InWorld, tNode.Pos, (float)SEGMENT, ObjectTypes,
				NULL,
				ActorsToIgnore,
				ActorsToResult);

			for (auto Element : ActorsToResult) {
				if (Element == ICharacter) {
					ActorsToResult.Remove(Element);
					break;
				}
			}

			if (ActorsToResult.Num() > 1)
				tNode.bBlocked = true;

			// Debug.
	/*		if (tNode.bBlocked)
				DrawDebugSphere(InWorld, tNode.Pos, radius, 20.f, FColor::Black, false, 3.f, 0, 1.f);
			else
				DrawDebugSphere(InWorld, tNode.Pos, radius, 20.f, FColor::Blue, false, 3.f, 0, 1.f);*/

			NodeMap.Add(tNode.index, tNode);
			NodeSet.Add(tNode);
		}
		return NodeSet.Find(tNode);

	}

	TArray<int32> GetAdjacentList(int32 NodeIndex) {
		auto tNode = NodeMap[NodeIndex];
		
		if (tNode.AdjacentIndex.Num() == 0) {
			for (auto dis : Direction) {
				auto tVector = tNode.Pos + dis;
				auto AdjNode = FindOrAdd(tVector);
				AdjNode->parentIndex = tNode.index;
				tNode.AdjacentIndex.Add(AdjNode->index);
			}
		}

		return tNode.AdjacentIndex;
	}

	float GetHeuristicCost(const int32 lhs, const int32 rhs) const {
		auto u = NodeRefToVector(lhs);
		auto v = NodeRefToVector(rhs);

		return FVector::Distance(u, v);
	}

	float GetTraversalCost(const int32 lhs, const int32 rhs) const {
		auto u = NodeRefToVector(lhs);
		auto v = NodeRefToVector(rhs);

		return FVector::Distance(u, v);
	}

	FVector SetNewDestination(int uIndex, int vIndex) {
		const int MAXSTEP = 4;
		int step{};
		TArray<int> Nominated;
		std::queue<int> Que;
		Que.push(vIndex);

		while (!Que.empty()) {
			int cStep = Que.size();
			while (cStep--) {
				int cIndex = Que.front(); Que.pop();
				auto AdjList = GetAdjacentList(cIndex);
				
				for (auto nIndex : AdjList) {
					//UE_LOG(LogTemp, Log, TEXT("n:%d"), AdjList.Num());
					
					Que.push(nIndex);
					//DrawDebugSphere(InWorld, NodeMap[nIndex].Pos, RADIUS, 20, FColor::Black, false, 1.f);

					if (!NodeMap[nIndex].bBlocked) {
						NodeMap[nIndex].TotalCost = (int)GetTraversalCost(uIndex, nIndex);

						//DrawDebugSphere(InWorld, NodeMap[nIndex].Pos, RADIUS, 20, FColor::Purple, false, 1.f);

						Nominated.HeapPush(nIndex, [&](const int32 lhs, const int32 rhs) {
							return NodeMap[lhs].TotalCost < NodeMap[rhs].TotalCost;
							});
					}
				}
			}

			if (Nominated.Num()) {
				int newIndex = -1;
				Nominated.HeapPop(newIndex, [&](const int32 lhs, const int32 rhs) {
					return NodeMap[lhs].TotalCost < NodeMap[rhs].TotalCost;
					}, false);

				if (newIndex == -1) return NodeMap[vIndex].Pos;
				// Function out.
				return NodeMap[newIndex].Pos;
			}

			if (++step == MAXSTEP) {
				// Function out.
				return NodeMap[vIndex].Pos;
			}
		}
		// Function. out.
		return NodeMap[vIndex].Pos;
	}

	void FindPath(FVector& StartPos, FVector& EndPos, TArray<FVector>& OutPath) {
		auto u = FindOrAdd(StartPos);
		auto v = FindOrAdd(EndPos);

		if (NodeMap[v->index].bBlocked) {
			auto newPos = SetNewDestination(u->index, v->index);
			
			if (newPos == EndPos) return;
			
			NodeMap.Empty();
			NodeSet.Empty();
			u = FindOrAdd(StartPos);
			v = FindOrAdd(newPos);
		}

		NodeMap[u->index].TotalCost = NodeMap[u->index].TraversalCost = 0;
		int cIndex = u->index;

		if (!NodeMap.Find(u->index) || !NodeMap.Find(v->index)) return;

		while(cIndex > -1 && cIndex !=v->index) {
			if (OpenList->IndexList.Num() > POOLSIZE) break;
			
			CloseList->AddToCloseList(cIndex);
			
			auto AdjList = GetAdjacentList(cIndex);

			for (auto nIndex : AdjList) {
				if (NodeMap.Find(nIndex) == nullptr) continue;
				
				if (NodeMap[nIndex].bOpen && !NodeMap[nIndex].bBlocked) {
					NodeMap[nIndex].TraversalCost = FMath::Min((int)GetTraversalCost(cIndex, nIndex) + NodeMap[cIndex].TraversalCost, (int)NodeMap[nIndex].TraversalCost);

					if (!NodeMap.Contains(v->index)) break;

					if (NodeMap[nIndex].TotalCost > NodeMap[nIndex].TraversalCost + GetHeuristicCost(nIndex, v->index)) {
						NodeMap[nIndex].TotalCost = NodeMap[nIndex].TraversalCost + GetHeuristicCost(nIndex, v->index);
						NodeMap[nIndex].parentIndex = cIndex;
						OpenList->AddToOpenList(nIndex);
					}
				}
			}
			cIndex = OpenList->HeapPop();

			if (cIndex == v->index) {
				int backIndex = cIndex;
				while (backIndex != u->index) {
					OutPath.Add(NodeMap[backIndex].Pos);
					backIndex = NodeMap[backIndex].parentIndex;
				}
				return;
			}
			
		}
	}

	FVector NodeRefToVector(const int32 NodeIndex) const {
		return NodeMap[NodeIndex].Pos;
	}

	void Reset() {
		NodeMap.Reset();
		NodeSet.Reset();
		OpenList->Clear();
		CloseList->Clear();
	}

	void CreateDirection() {
		Direction.Add(FVector(-SEGMENT, -SEGMENT, 0));
		Direction.Add(FVector(0, -SEGMENT, 0));
		Direction.Add(FVector(SEGMENT, -SEGMENT, 0));
		Direction.Add(FVector(SEGMENT, 0, 0));
		Direction.Add(FVector(SEGMENT, SEGMENT, 0));
		Direction.Add(FVector(0, SEGMENT, 0));
		Direction.Add(FVector(-SEGMENT, SEGMENT, 0));
		Direction.Add(FVector(-SEGMENT, 0, 0));
	}

	AActor* ICharacter;
};




class AstarPathFinding {
public:
	AstarPathFinding(AActor* ParamCharacter) {
		Graph.ICharacter = ParamCharacter;
	}

	void Reset(UWorld* InWorld) {
		Graph.InWorld = InWorld;
		_resetMutex.Lock();
		Graph.Reset();
		_resetMutex.Unlock();
	}

	void Find(FVector StartPos, FVector EndPos, TArray<FVector>& PathVector, UWorld* InWorld) {
		Reset(InWorld);

		StartPos.X = FMath::FloorToFloat(StartPos.X);
		StartPos.Y = FMath::FloorToFloat(StartPos.Y);
		StartPos.Z = FMath::FloorToFloat(StartPos.Z);

		EndPos.X = FMath::FloorToFloat(EndPos.X);
		EndPos.Y = FMath::FloorToFloat(EndPos.Y);
		EndPos.Z = FMath::FloorToFloat(StartPos.Z);
		//EndPos.Z = FMath::FloorToFloat(EndPos.Z+ StartPos.Z);

		StartPos.X = StartPos.X - FMath::Fmod(StartPos.X, SEGMENT);
		StartPos.Y = StartPos.Y - FMath::Fmod(StartPos.Y, SEGMENT);
		StartPos.Z = StartPos.Z - FMath::Fmod(StartPos.Z, SEGMENT);

		EndPos.X = EndPos.X - FMath::Fmod(EndPos.X, SEGMENT);
		EndPos.Y = EndPos.Y - FMath::Fmod(EndPos.Y, SEGMENT);
		EndPos.Z = EndPos.Z - FMath::Fmod(EndPos.Z, SEGMENT);

		Graph.FindPath(StartPos, EndPos, PathVector);
		
		// Debug.
	/*	for (auto path : PathVector) {
			DrawDebugSphere(Graph.InWorld, path, RADIUS, 20, FColor::Red, false, 1.f);
		}*/
	}

	FGraph Graph;
	FCriticalSection _resetMutex;
};
