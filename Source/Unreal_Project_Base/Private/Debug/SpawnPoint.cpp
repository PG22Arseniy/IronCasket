// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/SpawnPoint.h"
#include "MechGameStateBase.h"

#include "Mech/MechPlayer.h"


void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

 
	UWorld* World = GetWorld();
	if (!World) return;

	AMechGameStateBase* MechGameState = World->GetGameState<AMechGameStateBase>();
	
}
