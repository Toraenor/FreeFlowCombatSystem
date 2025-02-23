// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyStateFightTrail.generated.h"

/**
 * 
 */
UCLASS()
class FFCS_API UAnimNotifyStateFightTrail : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	// Niagara System to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail Effect")
	class UNiagaraSystem* TrailEffect;

	UPROPERTY(EditAnywhere, Category = "Trail Settings")
	FBoneSocketTarget StartTarget;

	UPROPERTY(EditAnywhere, Category = "Trail Settings")
	FBoneSocketTarget EndTarget;
private:
	void GetSocketOrBoneLocations(const USkeletalMeshComponent* MeshComp,FVector& StartLocation, FVector& EndLocation) const;
	
	UPROPERTY()
	class UNiagaraComponent* SpawnedTrail;
};
