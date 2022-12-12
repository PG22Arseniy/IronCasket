// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/LevelManager.h"

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelManager::SpawnMinigunEnemyAtRandomLoc(int32 SubLevel, bool PlayerDetected)
{

	if (LevelSpawners.Num()<= SubLevel) return;
	
	const int32 Num =  LevelSpawners[SubLevel].EnemySpawners.Num();

	if (Num <= 0) return;
	
	const int32 RandSpawnerNum =  FMath::RandRange(0,Num - 1); 

	
	LevelSpawners[SubLevel].EnemySpawners[RandSpawnerNum]->SpawnMinigunEnemy(PlayerDetected);
}

void ALevelManager::SpawnCannonEnemyAtRandomLoc(int32 SubLevel, bool PlayerDetected)
{
	if (LevelSpawners.Num()<= SubLevel) return;
	
	const int32 Num =  LevelSpawners[SubLevel].EnemySpawners.Num();

	if (Num <= 0) return;
	
	const int32 RandSpawnerNum =  FMath::RandRange(0,Num - 1); 

	
	LevelSpawners[SubLevel].EnemySpawners[RandSpawnerNum]->SpawnCannonEnemy(PlayerDetected);  
}

