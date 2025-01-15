// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFFCS.h"

#include "DebugMode.h"
#include "EnemyBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "FreeFlowCombatComponent.h"

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
#define ECC_FreeFlow ECollisionChannel::ECC_GameTraceChannel1

APlayerFFCS::APlayerFFCS()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetupAttachment(SpringArm);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 70.f), FRotator(0.f, -17.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;
}

float APlayerFFCS::GetMouseDotProduct(const AActor* Enemy) const
{
	if (!IsValid(Enemy))
		return -1.f;

	CameraComp->GetForwardVector().Normalize();
	return FVector::DotProduct(GetPlayerToEnemyVec(Enemy), CameraComp->GetForwardVector());
}

float APlayerFFCS::GetKeyboardDotProduct(const AActor* Enemy, const FVector2D& MoveValue) const
{
	if (!IsValid(Enemy) || !IsValid(IA_Move))
		return -1.f;

	const FRotator Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector RightVec = UKismetMathLibrary::Quat_RotateVector(Rotation.Quaternion(), FVector::RightVector);
	const FVector ForwardVec = UKismetMathLibrary::Quat_RotateVector(Rotation.Quaternion(), FVector::ForwardVector);
	FVector Vec = ForwardVec * MoveValue.Y + RightVec * MoveValue.X;

	if (!Vec.IsNearlyZero())
	{
		Vec.Normalize();
	}
	else
	{
		return -1.f;
	}

	if (const FVector PlayerToEnemy = GetPlayerToEnemyVec(Enemy); !PlayerToEnemy.IsNearlyZero())
	{
		return FVector::DotProduct(Vec, PlayerToEnemy);
	}

	return -1.f;
}

float APlayerFFCS::FindBestInputDotProductWithEnemy(const AActor* Enemy) const
{
	if (!IsValid(Enemy) || !IsValid(IA_Move) || !IsValid(GetWorld()) || !IsValid(GetWorld()->GetFirstPlayerController())
		|| !IsValid(
			GetWorld()->GetFirstPlayerController()->GetLocalPlayer()))
		return -1.f;

	const UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			GetWorld()->GetFirstPlayerController()->GetLocalPlayer());

	if (!IsValid(InputSubsystem))
		return -1.f;

	const FInputActionValue InputValue = InputSubsystem->GetPlayerInput()->GetActionValue(IA_Move);

	if (const FVector2D MoveValue = InputValue.Get<FVector2D>(); MoveValue.Length() != 0.f)
	{
		return GetKeyboardDotProduct(Enemy, MoveValue);
	}

	return GetMouseDotProduct(Enemy);
}

FVector APlayerFFCS::GetEnemyToPlayerVec(const AActor* Enemy) const
{
	FVector EnemyToPlayerVec = GetActorLocation() - Enemy->GetActorLocation();
	EnemyToPlayerVec.Normalize();
	return EnemyToPlayerVec;
}

void APlayerFFCS::GetEnemyTeleportLocationRotation(const AActor* Enemy, const float Offset, FVector& OutLocation,
                                                   FRotator& OutRotation) const
{
	OutLocation = GetEnemyToPlayerVec(Enemy) * Offset + Enemy->GetActorLocation();
	OutRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Enemy->GetActorLocation());
}

float APlayerFFCS::ScaleValueToCombo(const float ValueToScale, const bool Multiply) const
{
	const float ComboPercent = CurrentCombo2 / 10.f + 1;
	if (Multiply)
	{
		return ValueToScale * ComboPercent;
	}
	return ValueToScale / ComboPercent;
}

FVector APlayerFFCS::GetPlayerToEnemyVec(const AActor* Enemy) const
{
	FVector PlayerToEnemyVec = Enemy->GetActorLocation() - GetActorLocation();
	PlayerToEnemyVec.Normalize();
	return PlayerToEnemyVec;
}

bool APlayerFFCS::CheckCollisionBeforeTeleport(const AActor* Enemy, FHitResult& OutHit) const
{
	if (!IsValid(GetWorld()) || !IsValid(GetWorld()->GetFirstPlayerController()))
		return false;

	bool DrawDebug = false;

	if (GetWorld()->GetFirstPlayerController()->Implements<UDebugMode>())
	{
		DrawDebug = IDebugMode::Execute_GetDebugMode(GetWorld()->GetFirstPlayerController());
	}

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(Enemy);

	const bool IsHit = GetWorld()->SweepSingleByChannel(OutHit, GetActorLocation(), Enemy->GetActorLocation(),
	                                                    FQuat::Identity,
	                                                    ECC_FreeFlow,
	                                                    FCollisionShape::MakeSphere(
		                                                    GetCapsuleComponent()->GetScaledCapsuleRadius() / 2.f),
	                                                    CollisionParams);
	if (DrawDebug)
	{
		if (IsHit)
		{
			// Draw a red sphere at the hit location (the point where the sweep collided)
			DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, GetCapsuleComponent()->GetScaledCapsuleRadius() / 2.f, 12,
			                FColor::Red, false, 2.0f);

			// Optionally, draw a debug line from start to hit location
			DrawDebugLine(GetWorld(), GetActorLocation(), OutHit.ImpactPoint, FColor::Red, false, 2.0f);
		}
		else
		{
			// No hit: draw a green line from start to end
			DrawDebugLine(GetWorld(), GetActorLocation(), Enemy->GetActorLocation(), FColor::Green, false, 2.0f);
		}
	}

	return IsHit;
}

AActor* APlayerFFCS::FindBestEnemy(TArray<AActor*> Enemies) const
{
	float DotProduct = -1.f;
	AActor* BestEnemy = nullptr;

	for (const auto Enemy : Enemies)
	{
		FHitResult HitResult;
		if (const AEnemyBase* EnemyBase = Cast<AEnemyBase>(Enemy); !EnemyBase->GetIsTargetable())
			continue;

		const bool IsHit = CheckCollisionBeforeTeleport(Enemy, HitResult);
		const float NewDotProduct = FindBestInputDotProductWithEnemy(Enemy);

		//ensures that newDotProduct is greater than last one
		if (NewDotProduct <= DotProduct || IsHit)
			continue;

		// //ensures that if we have a hit it isn't the same enemy
		// if (const AActor* CurrentEnemy = FFCC->GetCurrentEnemy();
		// 	IsHit && CurrentEnemy != Enemy)
		// {
		// 	BestEnemy = Enemy;
		// 	DotProduct = NewDotProduct;
		// }
		// else
		// {
			BestEnemy = Enemy;
			DotProduct = NewDotProduct;
		//}
	}

	return BestEnemy;
}
