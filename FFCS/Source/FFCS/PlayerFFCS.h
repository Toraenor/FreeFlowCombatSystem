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

	UFUNCTION(BlueprintCallable)
	// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
	void RetrieveIAMoveValuesInCpp(const float ValueX, const float ValueY);
	UFUNCTION(BlueprintPure)
	float GetMouseDotProduct(const AActor* Enemy) const;
	UFUNCTION(BlueprintPure)
	float GetKeyboardDotProduct(const AActor* Enemy) const;
	UFUNCTION(BlueprintPure)
	float FindBestInputDotProductWithEnemy(const AActor* Enemy) const;
	UFUNCTION(BlueprintPure)
	FVector GetEnemyToPlayerVec(const AActor* Enemy) const;
	UFUNCTION(BlueprintCallable)
	void FindBestEnemyTest(TArray<const AActor*>& Enemies);

	FVector GetPlayerToEnemyVec(const AActor* Enemy) const;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArm;
	FVector2d InputActionMoveValue;
};
