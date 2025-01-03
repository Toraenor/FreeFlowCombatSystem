// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyComp.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UEnemyComp::UEnemyComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

UEnemyComp::EHitDirection UEnemyComp::GetHitDirection(const AActor* Instigator) const
{
	if (!Instigator || !GetOwner())
		return Error;

	// ReSharper disable once CppTooWideScopeInitStatement
	const float DeltaYawRotation = UKismetMathLibrary::NormalizedDeltaRotator(
		UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Instigator->GetActorLocation()), GetOwner()->GetActorRotation()).Yaw;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaYawRotation, -45.0f, 45.0f))
	{
		return EHitDirection::Forward;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaYawRotation, 45.0f, 135.0f))
	{
		return EHitDirection::Right;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaYawRotation, 135.0f, 180.0f) || UKismetMathLibrary::InRange_FloatFloat(
		DeltaYawRotation, -179.999f, -135.0f))
	{
		return EHitDirection::Backward;
	}
	else
	{
		return EHitDirection::Left;
	}
}
