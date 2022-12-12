// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Mech/CannonEnemy.h"
#include "Mech/MinigunEnemy.h"
#include "EnemySpawner.generated.h"


UENUM()
enum class SPAWN_TYPE
{
	COOLDOWN,
	ENEMY_NUMBER,
	NONE
};

UENUM()
enum class SPAWN_PATTERN
{
	MINIGUNS,
	CANNONS,
	BOTH,
	IN_ORDER
};
/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AEnemySpawner : public ATargetPoint
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SpawnMinigunEnemy(bool PlayerDetected);

	UFUNCTION(BlueprintCallable)
	void SpawnCannonEnemy(bool PlayerDetected);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnParams")
	SPAWN_TYPE SpawnType = SPAWN_TYPE::COOLDOWN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnParams", meta=(ClampMin = 1))
	float CooldownTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnParams", meta=(ClampMin = 0))
	int32 MinimumAliveEnemiesToSpawn = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnParams", meta=(ClampMin = 0))
	int32 MaxNumberOfEnemiesToSpawn = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnParams")
	SPAWN_PATTERN Spawn_Pattern = SPAWN_PATTERN::MINIGUNS;



	
protected:

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentSpawnedEnemies = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMinigunEnemy> MinigunEnemy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	TSubclassOf<ACannonEnemy> CannonEnemy;

	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

	UFUNCTION()
	void SpawnEnemy();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	bool CanSpawn = true;

	UFUNCTION()
	void AllowSpawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnParams", meta=(ClampMin = 1))
	float MinimumRateToSpawn = 5.0f;
	
};
