// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "GameFramework/GameStateBase.h"
#include "MechGameStateBase.generated.h"

class AProjectile;
class UWeaponData;
class ALevelManager;
class ASpawnPoint;
class AMinigunEnemy;
class ACannonEnemy;
/**
 * 
 */

UENUM()
enum class GAME_LEVEL
{
	LEVEL1 = 0,
	LEVEL2 = 1,
	LEVEL3 = 2,
	LEVEL4 = 3
};

UCLASS()
class UNREAL_PROJECT_BASE_API AMechGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	AMechGameStateBase();

	UPROPERTY(EditAnywhere, Category="Object Pooling", meta=(ClampMin = 0, ClampMax = 1000))  
	int32 MaxProjectileNumber = 100;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileBP;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly)
	TArray<UWeaponData*> WeaponCollection;

	UPROPERTY(BlueprintReadOnly)
	GAME_LEVEL CurrentLevelEnum = GAME_LEVEL::LEVEL1;

	UFUNCTION(BlueprintCallable)
	void LoadLevel();

	UFUNCTION(BlueprintCallable)
	void ReloadLevel();

	UFUNCTION(BlueprintCallable)
	void UnloadLevel();

	UFUNCTION(BlueprintCallable)
	void NextLevel();

	UFUNCTION(BlueprintCallable)
	void PreviousLevel();

	UFUNCTION(BlueprintCallable)
	void ChangeCurrentLevel();

	UPROPERTY()
	int32 CurrentLevelInt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> LevelNames = {"SL4_BaseArea","SL2_CityCenter","SL3_RadioTower","SL1_Outskirts"};


	UPROPERTY()
	TArray<ASpawnPoint*> SpawnPoints;

	UFUNCTION(BlueprintCallable)
	void ChangePlayerLocation();

	UFUNCTION(BlueprintImplementableEvent)
	void LevelLoad (int32 LevelNumberToLoad);

	UFUNCTION(BlueprintImplementableEvent)
	void LevelUnload (int32 LevelNumberToUnload);

	UFUNCTION(BlueprintImplementableEvent)
	void LevelReload (int32 LevelNumberToUnload);

	virtual void Tick(float DeltaSeconds) override;


	UFUNCTION(BlueprintCallable)
	void IncrementAliveEnemies();
	
	UFUNCTION(BlueprintCallable)
	void DecrementAliveEnemies();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetNumberOfAliveEnemies();


	UFUNCTION(BlueprintCallable)
	void SpawnEnemies(int32 MinigunNumber, int32 CannonNumber, bool PlayerDetected);

	UPROPERTY()
	ALevelManager* LevelManager;

	UFUNCTION(BlueprintImplementableEvent)
	void NewWave(int32 Wave);

	UPROPERTY(BlueprintReadOnly)
	int32 WaveNum = -1;

	UFUNCTION(BlueprintCallable)
	void FinishWaves();

	UFUNCTION(BlueprintCallable)
	void StartWaves(); 

	UPROPERTY()
	bool WaveInProgress = false;

	UFUNCTION(BlueprintCallable)
	void FinishWave();
	
	UFUNCTION(BlueprintCallable)
	void ResetNumberOfAliveEnemies();


	UFUNCTION(BlueprintCallable)
	void GamePause(bool Paused);

	UPROPERTY()
	bool GamePaused = false;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACannonEnemy> CannonEnemySubClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMinigunEnemy> MinigunEnemySubClass;

	
private:
	
	UPROPERTY()
	int32 CurrentAliveEnemies = 0;

	UPROPERTY()
	int32 CurrentAttackingEnemies = 0;
	
	
};
