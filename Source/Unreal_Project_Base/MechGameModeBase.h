// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MechGameModeBase.generated.h"

enum class TEAMS;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AMechGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	
public:
	AMechGameModeBase();

	UFUNCTION(BlueprintCallable)
	void EnemyKill();

	UFUNCTION(BlueprintCallable)
	void GameOver(bool Victory);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool GodMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OneShotOneKill = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InfiniteHealth = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InfiniteAmmo = false;

	UFUNCTION(BlueprintImplementableEvent, Category="Win_Loss")
	void LoseEvent();
 
	UFUNCTION(BlueprintImplementableEvent, Category="Win_Loss")
	void WinEvent();

	UFUNCTION(BlueprintCallable, Category="Win_Loss")
	void OpenGameOverScreen(bool Victory);

	UFUNCTION(BlueprintCallable)
	void GodModeToggle();

	UFUNCTION(BlueprintCallable)
	void OneShotOneKillToggle();

	UFUNCTION(BlueprintCallable)
	void InfiniteHealthToggle();

	UFUNCTION(BlueprintCallable)
	void InfiniteAmmoToggle();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 EnemiesToKill = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentEnemiesKilled = 0;  

	
	
};


UENUM()
enum class TEAMS
{
	PLAYER = 0,
	ENEMY = 1,
	NEUTRAL = 2
};


