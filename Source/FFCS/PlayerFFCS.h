// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerFFCS.generated.h"

/**
 * 
 */
UCLASS()
// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
class FFCS_API APlayerFFCS : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerFFCS();

	UFUNCTION(BlueprintPure)
	float GetMouseDotProduct(const AActor* Enemy) const;
	UFUNCTION(BlueprintPure)
	float GetKeyboardDotProduct(const AActor* Enemy, const FVector2D& MoveValue) const;
	UFUNCTION(BlueprintPure)
	float FindBestInputDotProductWithEnemy(const AActor* Enemy) const;
	UFUNCTION(BlueprintPure)
	FVector GetEnemyToPlayerVec(const AActor* Enemy) const;
	UFUNCTION(BlueprintCallable)
	AActor* FindBestEnemyTest(TArray<AActor*> Enemies);
	UFUNCTION(BlueprintPure)
	void GetEnemyTeleportLocationRotation(const AActor* Enemy, const float Offset, FVector& OutLocation, FRotator& OutRotation) const;
	UFUNCTION(BlueprintPure)
	float ScaleValueToCombo(const float ValueToScale, const bool Multiply) const;

	FVector GetPlayerToEnemyVec(const AActor* Enemy) const;

	UFUNCTION(BlueprintCallable)
	bool CheckCollisionBeforeTeleport(const AActor* Enemy, FHitResult& OutHit);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UCameraComponent> CameraComp;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(EditDefaultsOnly)
	// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
	TObjectPtr<class UFreeFlowCombatComponent> FFCC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
	TObjectPtr<class UInputAction> IA_Move;

	FVector2d InputActionMoveValue;
	int CurrentCombo2 = 0;
};
