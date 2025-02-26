// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "GameplayTagContainer.h"
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
	void SetCurrentEnemy(AActor* Enemy);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	AActor* GetCurrentEnemy() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCounterEnemy(AActor* Enemy);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	AActor* GetCounterEnemy() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetTagToPlayer(FGameplayTag Tag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	FGameplayTag GetTag() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Counter")
	void PlayCounterMontage();
};
