// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStateAIAttackCollision.h"

#include "FreeFlowCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimNotifyStateAIAttackCollision::UAnimNotifyStateAIAttackCollision()
{
	CounterTag = FGameplayTag::RequestGameplayTag(FName("PlayerStates.Counter"));
}

void UAnimNotifyStateAIAttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	//saving to a variable, so the value resets to what we set in the ANS
	AttackOnceTrigger = DoAttackOnce;
}

void UAnimNotifyStateAIAttackCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                                   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (AttackOnceTrigger)
	{
		//if attack should be done only once, use the private variable that gets reset at begin notify to check
		DoCollisionCheck(MeshComp);
		AttackOnceTrigger = false;
	}
	else if (!DoAttackOnce)
	{
		//if attack can be triggered multiple times, then use the exposed variable since the private gets set to false when it is true, that means it will continue to run the multiple hits logic after attacking once
		DoCollisionCheck(MeshComp);
	}
}

void UAnimNotifyStateAIAttackCollision::DoCollisionCheck(const USkeletalMeshComponent* MeshComp) const
{
	FHitResult Result;
	FVector InSocket;
	FVector OutSocket;
	GetSocketOrBoneLocations(MeshComp, InSocket, OutSocket);
	const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MeshComp->GetOwner());

	const EDrawDebugTrace::Type DebugTrace = Debug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(MeshComp->GetOwner()->GetWorld(), InSocket, OutSocket, Radius, TraceChannel, true, ActorsToIgnore,
	                                                   DebugTrace, Result, true);
	if (bHit)
	{
		AActor* HitActor = Result.GetActor();
		if (!IsValid(HitActor))
			return;
		
		// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
		const UFreeFlowCombatComponent* FFCC = HitActor->GetComponentByClass<UFreeFlowCombatComponent>(); 
		if (IsValid(FFCC) && FFCC->GetTag() != CounterTag)
		{
			// const FString HitActorName = HitActor->GetName();
			// if (GEngine)
			// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hit Actor : %s"), *HitActorName));
			UGameplayStatics::ApplyDamage(HitActor, Damage, nullptr, MeshComp->GetOwner(), UDamageType::StaticClass());
		}
	}
}

void UAnimNotifyStateAIAttackCollision::GetSocketOrBoneLocations(const USkeletalMeshComponent* MeshComp, FVector& StartLocation, FVector& EndLocation) const
{
	if (StartTarget.bUseSocket)
	{
		StartLocation = MeshComp->GetSocketLocation(StartTarget.SocketReference.SocketName);
	}
	else
	{
		StartLocation = MeshComp->GetSocketLocation(StartTarget.BoneReference.BoneName);
	}

	if (EndTarget.bUseSocket)
	{
		EndLocation = MeshComp->GetSocketLocation(EndTarget.SocketReference.SocketName);
	}
	else
	{
		EndLocation = MeshComp->GetSocketLocation(EndTarget.BoneReference.BoneName);
	}
}
