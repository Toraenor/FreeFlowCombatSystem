// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "AIDirector.h"
#include "Components/CapsuleComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::Death(const EHitDirection HitDirection)
{
	Super::Death(HitDirection);
	Targetable = false;
	PrimaryActorTick.bCanEverTick = false;

	TArray<UActorComponent*> Components;
	GetComponents(Components);

	for (UActorComponent* Component : Components)
	{
		if (Component && !Component->IsA(USkeletalMeshComponent::StaticClass()) && !Component->IsA(UCapsuleComponent::StaticClass()))
		{
			Component->SetActive(false);
		}
	}

	GetController()->Destroy();

	if (UAIDirector* AIDirector = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UAIDirector>())
	{
		AIDirector->RemoveEnemy(this);
	}
}

bool AEnemyBase::GetIsTargetable() const
{
	return Targetable;
}
