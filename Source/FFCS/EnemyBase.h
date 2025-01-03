// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EHitDirection : uint8
{
	Fwd,
	Right,
	Backward,
	Left
};

UCLASS()
class FFCS_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBase();

	UFUNCTION(BlueprintCallable)
	void ApplyDamages(const float Damage, const EHitDirection HitDirection);
	void Death(const EHitDirection HitDirection);
	bool GetIsTargetable() const;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool Targetable = true;
};
