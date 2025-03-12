// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UENUM(BlueprintType)
enum EHitDirection : uint8
{
	Fwd,
	Right,
	Backward,
	Left
};

UCLASS()
class FFCS_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamages(const float Damage, const EHitDirection HitDirection);
	UFUNCTION(BlueprintCallable)
	virtual void Death(const EHitDirection HitDirection);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="HealthSystem")
	float CurrentHealth = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HealthSystem")
	float MaxHealth = 10;
};
