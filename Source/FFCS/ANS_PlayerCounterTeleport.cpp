// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_PlayerCounterTeleport.h"

#include "FreeFlowCombatComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UANS_PlayerCounterTeleport::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (!IsValid(MeshComp->GetOwner()))
		return;
	
	const UFreeFlowCombatComponent* FreeFlowComp = MeshComp->GetOwner()->GetComponentByClass<UFreeFlowCombatComponent>();
	
	if (!IsValid(FreeFlowComp) || !IsValid(FreeFlowComp) || !IsValid(FreeFlowComp->GetCounterEnemy()))
		return;

	const FVector StartLocation = MeshComp->GetOwner()->GetActorLocation();
	const FVector EndLocation = FreeFlowComp->GetCounterEnemy()->GetActorLocation();
	const FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	const FVector TeleportLocation = EndLocation + Direction * Offset;
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);
	const FVector InterpLocation = FMath::VInterpTo(StartLocation, TeleportLocation, FrameDeltaTime, InterpSpeed);
	FreeFlowComp->GetCounterEnemy()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(EndLocation, StartLocation));
	MeshComp->GetOwner()->SetActorLocationAndRotation(InterpLocation, LookAtRotation);
}

void UANS_PlayerCounterTeleport::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp->GetOwner()))
		return;
	
	if (UFreeFlowCombatComponent* FreeFlowComp = MeshComp->GetOwner()->GetComponentByClass<UFreeFlowCombatComponent>())
	{
		FreeFlowComp->PlayCounterMontage();
	}
}
