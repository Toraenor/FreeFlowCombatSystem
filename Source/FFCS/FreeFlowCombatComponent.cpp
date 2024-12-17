// Fill out your copyright notice in the Description page of Project Settings.


#include "FreeFlowCombatComponent.h"

// Sets default values for this component's properties
UFreeFlowCombatComponent::UFreeFlowCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UFreeFlowCombatComponent::SetCurrentEnemy(AActor* Enemy)
{
	CurrentEnemy = Enemy;
	//add logic
}

AActor* UFreeFlowCombatComponent::GetCurrentEnemy() const
{
	return CurrentEnemy;
}

// void UFreeFlowCombatComponent::SetTagToPlayer(const FGameplayTag& Tag)
// {
// }

// const FGameplayTag& UFreeFlowCombatComponent::GetPlayerTag()
// {
// }

void UFreeFlowCombatComponent::GetMontageBasedOnDistance()
{
}

// FDataTableRowHandle UFreeFlowCombatComponent::GetRandomAnimation(const UDataTable* Table)
// {
// }


// Called when the game starts
// void UFreeFlowCombatComponent::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	// ...
// 	
// }
//
//
// // Called every frame
// void UFreeFlowCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }

