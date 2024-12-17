// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FreeFlowCombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FFCS_API UFreeFlowCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFreeFlowCombatComponent();

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		UFUNCTION(BlueprintCallable)
		void SetCurrentEnemy(AActor* Enemy);
		UFUNCTION(BlueprintCallable)
		AActor* GetCurrentEnemy() const;
		// UFUNCTION(BlueprintCallable)
		// void SetTagToPlayer(const FGameplayTag& Tag);
		// UFUNCTION(BlueprintCallable)
		// const FGameplayTag& GetPlayerTag();
		UFUNCTION(BlueprintCallable)
		void GetMontageBasedOnDistance();
		// UFUNCTION(BlueprintCallable)
		// FDataTableRowHandle GetRandomAnimation(const UDataTable* Table);

private:
	UPROPERTY()
	AActor* CurrentEnemy = nullptr;
	
};
