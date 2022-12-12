// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/DebugSpawnPoint.h"

#include "Mech/MechController.h"
#include "Mech/MechPlayer.h"

void ADebugSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World) return;

	AMechController* MechController = Cast<AMechController>(World->GetFirstPlayerController());
	if (!MechController) return;

	AMechPlayer* MechPlayer = MechController->GetPawn<AMechPlayer>();
	if (!MechPlayer) return; 

	MechPlayer->SpawnPoints.Add(this);
	
}
