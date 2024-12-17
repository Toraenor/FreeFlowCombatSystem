// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "EnemyComp.h"

AEnemyBase::AEnemyBase()
{
	EnemyComp = CreateDefaultSubobject<UEnemyComp>(TEXT("EnemyComp"));
}
