// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileTypes/Projectile.h"
#include "Subsystems/Subsystem.h"
#include "ObjectPoolingSystem.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UObjectPoolingSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public: 
	
	
	UFUNCTION(BlueprintCallable)
	void ReturnProjectile (AProjectile* Projectile); 

	UFUNCTION(BlueprintCallable)
	AProjectile* GetProjectile();

private:
	TQueue<AProjectile*> ProjectileArray;
};
