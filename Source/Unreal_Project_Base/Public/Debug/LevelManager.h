// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawner.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"


class ADebugSpawnPoint;
class ASpawnPoint;

USTRUCT()
struct FLevelSpawners
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere) 
	TArray<AEnemySpawner*> EnemySpawners; 
	
};




UCLASS()
class UNREAL_PROJECT_BASE_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DebugSpawnPoints")
	TArray<ADebugSpawnPoint*> DebugSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SpawnPoints")
	TArray<ASpawnPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category="EnemySpawners") 
	TArray<FLevelSpawners> LevelSpawners;


	UFUNCTION(BlueprintCallable)
	void SpawnMinigunEnemyAtRandomLoc(int32 SubLevel, bool PlayerDetected); 

	UFUNCTION(BlueprintCallable)
	void SpawnCannonEnemyAtRandomLoc(int32 SubLevel, bool PlayerDetected);
};



