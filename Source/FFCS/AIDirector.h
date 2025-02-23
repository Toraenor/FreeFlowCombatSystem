// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIDirector.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FFCS_API UAIDirector : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAIDirector();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RemoveEnemy(AActor* Enemy);
};
