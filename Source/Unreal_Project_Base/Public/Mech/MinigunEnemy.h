// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mech/MechEnemy.h"
#include "MinigunEnemy.generated.h"

class AWeapon;
class AMinigunAIController;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AMinigunEnemy : public AMechEnemy
{
	GENERATED_BODY()

public:

	AMinigunEnemy();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0))
	float MachineGunMinDistance = 3000.0f;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


public:

	UFUNCTION(BlueprintCallable)
	void AttackChange ();

	UFUNCTION()
	void AttackPlayer ();
	
	virtual void RotateCorrectly(float DeltaSeconds) override;
	
	virtual void SetWeaponTarget(AWeapon* Weapon) override;
	
	virtual void Death() override;
	

	UFUNCTION()
	void AllowAttack ();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0))
	float TimeBetweenAttackTypeChange = 2.0f;

	UPROPERTY()
	FTimerHandle AttackTypeChangeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0))
	int32 MaxRandomTimeOfNotAttacking = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0))
	int32 MinRandomTimeOfNotAttacking = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0))
	int32 MaxRandomTimeOfAttacking = 1; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0))
	int32 MinRandomTimeOfAttacking = 2;
	
};
