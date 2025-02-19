// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyStateAITeleport.generated.h"

/**
 * 
 */
UCLASS()
class FFCS_API UAnimNotifyStateAITeleport : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float Offset = 50.f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float InterpSpeed = 5.f;
};
