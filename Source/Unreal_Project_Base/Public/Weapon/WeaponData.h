// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileTypes/Projectile.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UWeaponData : public UDataAsset
{
	GENERATED_BODY() 

public:  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage",meta=(ClampMin = 0)) 
	float DamageForMech = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage",meta=(ClampMin = 0))
	float DamageForDestructible = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage", meta=(ClampMin = -1, ClampMax = 10)) 
	int PenetrationDepth = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMax = 10000.0f, ClampMin = 0), Category="Damage")   
	int32 ImpulseStrength = 5; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponStats",meta=(ClampMin = 0, ClampMax = 100)) 
	float ShootPower = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponStats",meta=(ClampMin = 0))
	int32 BulletsPerShot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo",meta=(ClampMin = 0))
	int32 CurrentAmmo = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo",meta=(ClampMin = 0))
	int32 Magazine = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo",meta=(ClampMin = 0))
	int32 AllAmmo = 800;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Ammo")
	bool InfiniteAmmo = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponStats") 
	bool Automatic = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponStats", meta=(ClampMin = 0, ClampMax = 100)) 
	int32 ShootingRate = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponStats", meta=(ClampMin = 0))
	float Distance = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inaccuracy", meta=(ClampMin = 0, ClampMax = 1000))
	float MaxVerticalInaccuracy = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inaccuracy", meta=(ClampMin = 0, ClampMax = 1000))
	float MaxHorizontalInaccuracy = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inaccuracy", meta=(ClampMin = 0, ClampMax = 1000))
	float MinVerticalInaccuracy = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inaccuracy", meta=(ClampMin = 0, ClampMax = 1000))
	float MinHorizontalInaccuracy = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponStats", meta=(ClampMin=0))
	float TimeBetweenShots = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reload", meta=(ClampMin=0)) 
	float TimeBetweenReloads = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reload")
	bool AutoReload = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reload") 
	bool AllowShootingWhileReload = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reload", meta=(ClampMin=0, ClampMax=5))
	float AutoReloadTime = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<AProjectile> Projectile = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeatBar", meta=(ClampMin=0, ClampMax=100))
	float HeatCost = 10.0f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentReloadTimeValue = 1; 

	
};
