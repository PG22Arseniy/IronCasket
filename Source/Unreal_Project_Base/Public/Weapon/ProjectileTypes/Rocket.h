// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ProjectileTypes/Projectile.h"
#include "Rocket.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API ARocket : public AProjectile
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void RocketHitWorld(FHitResult Hit);
 
	UFUNCTION(BlueprintImplementableEvent) 
	void RocketHitMech(FHitResult Hit);

	UFUNCTION(BlueprintImplementableEvent)  
	void RocketExplosionInAir();

	virtual void WorldHit(FHitResult Hit) override;
	
	virtual void MechHit(FHitResult Hit) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(ClampMin = 0, ClampMax= 50000))
	float RadiusForImpulse = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(ClampMin = 0, ClampMax= 50000))
	float StrengthForImpulse = 1000.0f; 
	
	
};

