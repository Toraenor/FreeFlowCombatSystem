// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStateAIAttackCollision.h"

#include "DebugMode.h"
#include "EnemyComponent.h"
#include "FreeFlowCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimNotifyStateAIAttackCollision::UAnimNotifyStateAIAttackCollision()
{
	CounterTag = FGameplayTag::RequestGameplayTag(FName("PlayerStates.Counter"));
}

void UAnimNotifyStateAIAttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                    float TotalDuration,
                                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	//saving to a variable, so the value resets to what we set in the ANS
	AttackOnceTrigger = DoAttackOnce;
}

void UAnimNotifyStateAIAttackCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float FrameDeltaTime,
                                                   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	DoCollisionCheck(MeshComp);
}

void UAnimNotifyStateAIAttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (UEnemyComponent* EnemyComponent = MeshComp->GetOwner()->GetComponentByClass<UEnemyComponent>())
	{
		EnemyComponent->AlreadyAttacked = false;
	}
}

void UAnimNotifyStateAIAttackCollision::DoCollisionCheck(const USkeletalMeshComponent* MeshComp) const
{
	if (!MeshComp->GetOwner())
		return;
	UEnemyComponent* EnemyComponent = MeshComp->GetOwner()->GetComponentByClass<UEnemyComponent>();
	if (!EnemyComponent || (AttackOnceTrigger && EnemyComponent->AlreadyAttacked))
		return;

	FHitResult Result;
	FVector InSocket;
	FVector OutSocket;
	GetSocketOrBoneLocations(MeshComp, InSocket, OutSocket);
	const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MeshComp->GetOwner());

	bool DrawDebugByKey = false;

	if (GetWorld() && GetWorld()->GetFirstPlayerController() && GetWorld()->GetFirstPlayerController()->Implements<
		UDebugMode>())
	{
		DrawDebugByKey = IDebugMode::Execute_GetDebugMode(GetWorld()->GetFirstPlayerController());
	}

	const EDrawDebugTrace::Type DebugTrace = Debug || DrawDebugByKey
		                                         ? EDrawDebugTrace::ForDuration
		                                         : EDrawDebugTrace::None;
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(MeshComp->GetOwner()->GetWorld(), InSocket, OutSocket,
	                                                          Radius, TraceChannel, true, ActorsToIgnore,
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
			UGameplayStatics::ApplyDamage(HitActor, Damage, nullptr, MeshComp->GetOwner(), UDamageType::StaticClass());
			EnemyComponent->AlreadyAttacked = true;
		}
	}
}

void UAnimNotifyStateAIAttackCollision::GetSocketOrBoneLocations(const USkeletalMeshComponent* MeshComp,
                                                                 FVector& StartLocation, FVector& EndLocation) const
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
