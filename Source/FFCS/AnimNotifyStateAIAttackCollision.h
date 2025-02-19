// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyStateAIAttackCollision.generated.h"

/**
 * 
 */
UCLASS()
class FFCS_API UAnimNotifyStateAIAttackCollision : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void DoCollisionCheck(const USkeletalMeshComponent* MeshComp) const;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Settings")
	bool DoAttackOnce;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Settings")
	float Radius;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Settings")
	FName StartSocket;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Settings")
	FName EndSocket;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Settings")
	float Damage = 1.f;

private:
	UPROPERTY()
	bool AttackOnceTrigger = false;

	UPROPERTY(EditInstanceOnly, Category="Settings")
	bool Debug = false;
};
