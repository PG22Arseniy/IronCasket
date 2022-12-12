// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public: 
	
	virtual void Fire() override;

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponFired();
	
	virtual void AllowShooting() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	float GetCooldown();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerShot();
	
private:
	float Cooldown = -1; 

	
	
};
