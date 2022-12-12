// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "GroupAISubsystem.generated.h"


class AMinigunEnemy;
class ACannonEnemy;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UGroupAISubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:

	
	
	// Minigun Functions
	UFUNCTION(BlueprintCallable)
	void AddAttackingMinigun(AMinigunEnemy* MinigunEnemy);

	UFUNCTION(BlueprintCallable)
	void RemoveAttackingMinigun(AMinigunEnemy* MinigunEnemy);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetMinigunNumberInAttack(AMinigunEnemy* MinigunEnemy);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AMinigunEnemy*> GetAttackingMinigunArray() const;


	// Cannon Functions
	UFUNCTION(BlueprintCallable)
	void AddAttackingCannon(ACannonEnemy* CannonEnemy);

	UFUNCTION(BlueprintCallable)
	void RemoveAttackingCannon(ACannonEnemy* CannonEnemy);

	UFUNCTION(BlueprintCallable, BlueprintPure) 
	int32 GetCannonNumberInAttack(ACannonEnemy* CannonEnemy); 
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ACannonEnemy*> GetAttackingCannonArray() const;


	// 2nd Minigun Cutoff
	UFUNCTION()
	AMinigunEnemy* GetMinigunEnemyToCutOff();
	
private:
	
	TArray<AMinigunEnemy*> AttackingMiniguns;
	
	TArray<ACannonEnemy*> AttackingCannons;

	
	
};
