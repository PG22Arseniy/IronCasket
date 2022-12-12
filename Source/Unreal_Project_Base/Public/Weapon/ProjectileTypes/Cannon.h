// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ProjectileTypes/Projectile.h"
#include "Cannon.generated.h"


class USphereComponent; 
/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API ACannon : public AProjectile
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void CannonHitWorld(FHitResult Hit);
 
	UFUNCTION(BlueprintImplementableEvent) 
	void CannonHitMech(FHitResult Hit);

	virtual void WorldHit(FHitResult Hit) override;
	
	virtual void MechHit(FHitResult Hit) override;
};
