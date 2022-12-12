// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolPoint.h"

APatrolPoint::APatrolPoint(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick  = false;
}

void APatrolPoint::BeginPlay()
{
	Super::BeginPlay();
}
