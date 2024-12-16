// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFFCS.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"

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
	if (!IsValid(Enemy) || !IsValid(IA_Move) || !IsValid(GetWorld()) || !IsValid(GetWorld()->GetFirstPlayerController()) || !IsValid(
		GetWorld()->GetFirstPlayerController()->GetLocalPlayer()))
		return -1.f;
	
	const UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstPlayerController()->GetLocalPlayer());

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

// void APlayerFFCS::FindBestEnemyTest(TArray<const AActor*>& Enemies)
// {
// 	// float DotProduct = -1.f;
// 	// for (auto Enemy : Enemies)
// 	// {
// 	// 	
// 	// }
// }
