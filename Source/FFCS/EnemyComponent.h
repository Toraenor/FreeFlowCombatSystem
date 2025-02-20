// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Components/ActorComponent.h"
#include "EnemyComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FFCS_API UEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyComponent();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	EHitDirection HitDirection(AActor* Instigator);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAttackPlayer(bool AttackPlayer);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	bool GetAttackPlayer() const;
};
