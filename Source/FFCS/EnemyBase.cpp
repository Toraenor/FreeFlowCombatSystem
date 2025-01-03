// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::ApplyDamages(const float Damage, const EHitDirection HitDirection)
{
	CurrentHealth -= Damage;
	if (GEngine)
	{
		const FString ToDisplay = GetName() + " CurrentHealth: " + FString::SanitizeFloat(CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, ToDisplay);
	}
	
	if (CurrentHealth <= 0)
	{
		Death(HitDirection);
	}
}

void AEnemyBase::Death(const EHitDirection HitDirection)
{
	if (!GetCharacterMovement() || !GetMesh() || !GetCapsuleComponent())
		return;
	
	//GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Targetable = false;

	switch (HitDirection)
	{
	case Fwd:
		LaunchCharacter(-GetActorForwardVector() * 1000, true, true);
		break;
	case Right:
		LaunchCharacter(GetActorRightVector() * 1000, true, true);
		break;
	case Backward:
		LaunchCharacter(GetActorForwardVector() * 1000, true, true);
		break;
	case Left:
		LaunchCharacter(-GetActorRightVector() * 1000, true, true);
		break;
	}
	

}

bool AEnemyBase::GetIsTargetable() const
{
	return Targetable;
}
