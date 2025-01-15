// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::Death(const EHitDirection HitDirection)
{
	Super::Death(HitDirection);
	Targetable = false;
}

bool AEnemyBase::GetIsTargetable() const
{
	return Targetable;
}
