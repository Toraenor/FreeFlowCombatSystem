// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStateFightTrail.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyStateFightTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp || !TrailEffect) return;

	FVector StartLocation;
	FVector EndLocation;
	GetSocketOrBoneLocations(MeshComp, StartLocation, EndLocation);

	// Spawn Niagara system but keep it deactivated
	SpawnedTrail = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailEffect, MeshComp, NAME_None,
	                                                            StartLocation, FRotator::ZeroRotator,
	                                                            EAttachLocation::KeepWorldPosition, true);

	// Ensure the system is valid
	if (SpawnedTrail)
	{
		// Set Niagara parameters BEFORE activation
		SpawnedTrail->SetVariablePosition(FName("StartPosition"), StartLocation);
		SpawnedTrail->SetVariablePosition(FName("EndPosition"), EndLocation);

		// Activate the system manually after setting parameters
		SpawnedTrail->Activate();
	}
}

void UAnimNotifyStateFightTrail::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (SpawnedTrail)
	{
		// Get the start and end locations
		FVector StartLocation;
		FVector EndLocation;
		GetSocketOrBoneLocations(MeshComp, StartLocation, EndLocation);

		SpawnedTrail->SetVariablePosition(FName("StartPosition"), StartLocation);
		SpawnedTrail->SetVariablePosition(FName("EndPosition"), EndLocation);
	}
}

void UAnimNotifyStateFightTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp || !ImpactEffect) return;

	FVector StartLocation;
	FVector EndLocation;
	GetSocketOrBoneLocations(MeshComp, StartLocation, EndLocation);
	UGameplayStatics::SpawnEmitterAtLocation(MeshComp, ImpactEffect, EndLocation, FRotator::ZeroRotator, FVector(0.3f));
}

void UAnimNotifyStateFightTrail::GetSocketOrBoneLocations(const USkeletalMeshComponent* MeshComp, FVector& StartLocation, FVector& EndLocation) const
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
