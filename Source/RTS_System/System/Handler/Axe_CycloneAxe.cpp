// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe_CycloneAxe.h"
#include "CoolDownHandler.h"
#include "SkillHelper.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../Actors/Units/Animation/AxeSkillAnimInstance.h"
#include "../../Actors/Units/BaseMeleeUnit.h"
#include "../../DataTable/ABaseSkillTable.h"
#include "../../UI/MainHUD.h"



UAxe_CycloneAxe::UAxe_CycloneAxe() {
	Skill_ID = "4";

	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_AxeSkill(
		TEXT("/Game/Mannequin/Animations/ANIM_UnitAxeSkill.ANIM_UnitAxeSkill_C"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_SkillAreaDecal(
		TEXT("/Game/Resources/Circle/MAT_Arc.MAT_Arc"));

	SkillAreaDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("D_CycloneAxe"));

	if (MAT_SkillAreaDecal.Succeeded()) {
		AreaMaterial = MAT_SkillAreaDecal.Object;
		//SkillAreaDecal->SetDecalMaterial(AreaMaterial);
	}

	if (ANIM_AxeSkill.Succeeded())
		AxeSkillAnimInstanceClass = ANIM_AxeSkill.Class;
}


void UAxe_CycloneAxe::ActiveSkill(AUnit* pUnit) {
	// Require direction from HUD.
	auto IController = UGameplayStatics::GetPlayerController(pUnit->GetWorld(), 0);
	auto IHUD = Cast<AMainHUD>(IController->GetHUD());

	if (IsValid(IHUD)) {
		IHUD->SetMouseLeftButtonAction(LeftButtonAction::SKILLAREA);
	}
}

void UAxe_CycloneAxe::PlaySkillAnimation(AUnit* pUnit) {
	auto IUnit = Cast<ABaseMeleeUnit>(pUnit);

	IUnit->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	IUnit->GetMesh()->SetAnimInstanceClass(AxeSkillAnimInstanceClass);

	MouseLocation.Z = IUnit->GetActorLocation().Z;
	
	auto DesiredRotation = UKismetMathLibrary::FindLookAtRotation(
		IUnit->GetActorLocation(), 
		MouseLocation);
	
	IUnit->SetActorRotation(DesiredRotation);
	IUnit->TurnOffSkillRange();
	IUnit->StopMovement();

	auto pAnim = IUnit->GetMesh()->GetAnimInstance();
	auto AxeAnim = Cast<UAxeSkillAnimInstance>(pAnim);

	if (IsValid(AxeAnim)) {
		AxeAnim->PlayCycloneAxe();
	}
}

void UAxe_CycloneAxe::ShowSkillArea(AUnit* pUnit, FVector CursorLocation) {
	auto IUnit = Cast<ABaseMeleeUnit>(pUnit);

	float corner = SkillParams->Variable03;
	float range = SkillParams->Variable04;

	auto SkillAreaMaterial = UMaterialInstanceDynamic::Create(AreaMaterial, NULL);

	if (!IsValid(SkillAreaMaterial)) return;

	SkillAreaMaterial->SetScalarParameterValue(FName("theta"), corner);

	IUnit->TurnOnBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);
	IUnit->SkillAreaRange->SetDecalMaterial(SkillAreaMaterial);
	IUnit->SkillAreaRange->DecalSize.Y = range;
	IUnit->SkillAreaRange->DecalSize.Z = range;

	IUnit->SkillAreaRange->SetVisibility(true);
	//IUnit->bShowSkillArea = true;

	FVector StartTrace = IUnit->GetActorForwardVector();
	FVector MouseEnd = CursorLocation - IUnit->GetActorLocation();

	float degree = GetAngle(StartTrace, MouseEnd);

	FRotator OriginRotation{ -90.f,(corner / 2.f),0.f };
	FRotator TargetRotation = OriginRotation + FRotator{ 0.f, degree, 0.f };

	IUnit->SkillAreaRange->SetRelativeRotation(TargetRotation);

	if (bDrawDebug) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("rot:%s"), *TargetRotation.ToString()));
	}

	FVector Pivot = { 1.f,0.f,0.f };
	float offset = GetAngle(Pivot, MouseEnd);
	MouseLocation = CursorLocation;

	_offset = offset;
	_degree = degree;

	if (bDrawDebug) {
		int segment = 10;
		float sector = corner / float(segment);

		for (int i = 0; i <= segment; i++) {
			float angle = (corner / float(segment) * i) + offset - (corner / 2.f);

			if (angle >= 360.f) angle -= 360.f;
			else if (angle < 0.f) angle += 360.f;

			FVector u = StartTrace.GetSafeNormal();
			FVector v = FVector::ZeroVector;

			float theta = FMath::DegreesToRadians(angle);

			v.X += FMath::Cos(theta) * 300.f;
			v.Y += FMath::Sin(theta) * 300.f;
			v += IUnit->GetActorLocation();

			DrawDebugLine(IUnit->GetWorld(),
				IUnit->GetActorLocation(),
				v, FColor::Black, false, 0, 3.f);
		}
	}

	if (bDrawDebug) {
		DrawDebugLine(IUnit->GetWorld(), IUnit->GetActorLocation(),
			(CursorLocation + FVector(0.f, 0.f, IUnit->GetActorLocation().Z)) - IUnit->GetActorLocation() + IUnit->GetActorLocation(), FColor::Green);
		DrawDebugLine(IUnit->GetWorld(), FVector::ZeroVector + FVector(0.f, 0.f, 500.f),
			IUnit->GetActorForwardVector() * 1000.f + FVector(0.f, 0.f, 500.f), FColor::Red);
		DrawDebugLine(IUnit->GetWorld(), FVector::ZeroVector, FVector(0.f, 0.f, 5000.f), FColor::Black);
	}
}

void UAxe_CycloneAxe::AreaSkillJudge(AUnit* pUnit) {
	auto IUnit = Cast<ABaseMeleeUnit>(pUnit);

	FVector StartTrace = IUnit->GetActorForwardVector();

	int segment = 10;
	float corner = SkillParams->Variable03;
	float range = SkillParams->Variable04;

	for (int i = 0; i <= segment; i++) {
		float angle = (corner / float(segment) * i) + _offset - (corner / 2.f);

		if (angle >= 360.f) angle -= 360.f;
		else if (angle < 0.f) angle += 360.f;

		FVector u = StartTrace.GetSafeNormal();
		FVector v = FVector::ZeroVector;

		float theta = FMath::DegreesToRadians(angle);

		v.X += FMath::Cos(theta) * range;
		v.Y += FMath::Sin(theta) * range;
		v += IUnit->GetActorLocation();

		// Collision.
		TArray<FHitResult> HitResults;
		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		IUnit->GetWorld()->LineTraceMultiByObjectType(
			HitResults,
			IUnit->GetActorLocation(),
			v,
			QueryParams);

		for (auto Element : HitResults) {
			auto aUnit = Cast<ABaseMeleeUnit>(Element.GetActor());

			if (IsValid(aUnit)) {
				FString name = aUnit->GetName();
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
					FString::Printf(TEXT("name:%s"), *name));
			}
		}

		if (bDrawDebug) {
			DrawDebugLine(IUnit->GetWorld(),
				IUnit->GetActorLocation(),
				v, FColor::Black, false, 0, 3.f);
		}
	}
}


// temp
float UAxe_CycloneAxe::GetAngle(FVector& StartTrace, FVector& EndTrace) {
	FVector u = StartTrace.GetSafeNormal2D();
	FVector v = EndTrace.GetSafeNormal2D();
	FVector uSide{ u.X * -1.f, u.Y * -1.f,0.f };

	float theta = FMath::Acos(FVector::DotProduct(u, v));
	float degree = FMath::RadiansToDegrees(theta);

	bool bLeft = false;

	int vQuardrant = GetQuardrant(v);
	int uQuardrant = GetQuardrant(u);

	// 1 quadrant.
	if (uQuardrant == 1) {
		if (vQuardrant == 1) {
			if (v.Y < u.Y) bLeft = true;
		}
		else if (vQuardrant == 3) {
			if (v.Y < uSide.Y) bLeft = true;
		}
		else if (vQuardrant == 4) bLeft = true;
	}
	// 2 quardrant.
	else if (uQuardrant == 2) {
		if (vQuardrant == 2) {
			if (u.Y < v.Y) bLeft = true;
		}
		else if (vQuardrant == 4) {
			if (v.Y > uSide.Y) bLeft = true;
		}
		else if (vQuardrant == 1) bLeft = true;
	}
	// 3 quardrant.
	else if (uQuardrant == 3) {
		if (vQuardrant == 3) {
			if (u.Y < v.Y) bLeft = true;
		}
		else if (vQuardrant == 1) {
			if (v.Y > uSide.Y) bLeft = true;
		}
		else if (vQuardrant == 2) bLeft = true;
	}
	// 4 quardrant.
	else {
		if (vQuardrant == 4) {
			if (u.Y > v.Y) bLeft = true;
		}
		else if (vQuardrant == 2) {
			if (v.Y < uSide.Y) bLeft = true;
		}
		else if (vQuardrant == 3) bLeft = true;
	}

	if (bLeft) degree = 360.f - degree;
	return degree;
}

int UAxe_CycloneAxe::GetQuardrant(FVector& vec) {
	if (vec.X > 0 && vec.Y > 0) return 1;
	else if (vec.X < 0 && vec.Y > 0) return 2;
	else if (vec.X < 0 && vec.Y < 0) return 3;
	else return 4;
}
