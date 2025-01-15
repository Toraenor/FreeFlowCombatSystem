// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum EAIStates : uint8
{
	GetInRange,
	GetBack,
	Wait,
	LookForWeapons,
	Strafe,
	Attack
};

UCLASS()
class FFCS_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBase();
	
	virtual void Death(const EHitDirection HitDirection) override;
	bool GetIsTargetable() const;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool Targetable = true;
};
