// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/EnemySpawner.h"

#include "MechGameStateBase.h"
#include "AI/AIControllers/MinigunAIController.h"

void AEnemySpawner::SpawnMinigunEnemy(bool PlayerDetected)
{
	const FActorSpawnParameters SpawnParameters;

	const FVector Loc = GetActorLocation();
	const FRotator Rot = GetActorRotation();

	// Spawn Minigun Enemy
	AMinigunEnemy* EnemyMech =  GetWorld()->SpawnActor<AMinigunEnemy>(MinigunEnemy,Loc, Rot, SpawnParameters);

	if (PlayerDetected)
	{
		// Detect Player From Start
		EnemyMech->DetectPlayer();
	}
	
	CurrentSpawnedEnemies ++;
}

void AEnemySpawner::SpawnCannonEnemy(bool PlayerDetected)
{
	const FActorSpawnParameters SpawnParameters;

	const FVector Loc = GetActorLocation();
	const FRotator Rot = GetActorRotation();

	// Spawn Cannon Enemy
	ACannonEnemy* EnemyMech = GetWorld()->SpawnActor<ACannonEnemy>(CannonEnemy,Loc, Rot, SpawnParameters);

	if (PlayerDetected)
	{
		// Detect Player From Start
		if (EnemyMech)
			EnemyMech->DetectPlayer();  
	}
	
	CurrentSpawnedEnemies ++;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnType == SPAWN_TYPE::COOLDOWN)
	{
		// Spawning is based on Time
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, CooldownTime, true);
	}
	
	if (SpawnType == SPAWN_TYPE::ENEMY_NUMBER)
	{
		// Spawning is based on Number of alive
	}
}

void AEnemySpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (SpawnType == SPAWN_TYPE::ENEMY_NUMBER)
	{
		UWorld* World = GetWorld();
		if (!World) return;

		AMechGameStateBase* MechGameState = World->GetGameState<AMechGameStateBase>();
		if (!MechGameState) return;


		if (MechGameState->GetNumberOfAliveEnemies() <= MinimumAliveEnemiesToSpawn)
		{
			if (CanSpawn)
			{
				SpawnEnemy(); 
		
				GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::AllowSpawn, MinimumRateToSpawn,  false);
			}
		
		}
	} 
	
}

void AEnemySpawner::AllowSpawn()
{
	CanSpawn = true;
}

void AEnemySpawner::SpawnEnemy()
{
	if (CurrentSpawnedEnemies >= MaxNumberOfEnemiesToSpawn)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		CanSpawn = false;
		return;
	}
	switch (Spawn_Pattern)
	{
		case SPAWN_PATTERN::MINIGUNS:

			SpawnMinigunEnemy(true);
			break;
	

		case SPAWN_PATTERN::CANNONS:

			SpawnCannonEnemy(true);
			break;

		
		case SPAWN_PATTERN::IN_ORDER:

			if (CurrentSpawnedEnemies % 2 == 0)
				SpawnMinigunEnemy(true);
		
			else
				SpawnCannonEnemy(true);

			break;
		
		default: break;
	}
	
	CanSpawn = false; 

	if (CurrentSpawnedEnemies >= MaxNumberOfEnemiesToSpawn) GetWorldTimerManager().ClearTimer(SpawnTimerHandle); 
}
