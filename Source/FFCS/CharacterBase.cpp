// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentHealth = MaxHealth;
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::ApplyDamages(const float Damage, const EHitDirection HitDirection)
{
	CurrentHealth -= Damage;
	
	if (CurrentHealth <= 0)
	{
		Death(HitDirection);
	}
}

void ACharacterBase::Death(const EHitDirection HitDirection)
{
	if (!GetCharacterMovement() || !GetMesh() || !GetCapsuleComponent())
		return;
	

	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}

