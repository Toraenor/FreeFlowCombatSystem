// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Components/ActorComponent.h"
#include "FreeFlowCombatComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FFCS_API UFreeFlowCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFreeFlowCombatComponent();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	EHitDirection HitDirection(AActor* Instigator);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayHitMontage(EHitDirection HitDirection);
};
