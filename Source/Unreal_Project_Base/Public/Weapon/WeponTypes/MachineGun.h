// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "MachineGun.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AMachineGun : public AWeapon
{
	GENERATED_BODY()

public:

	AMachineGun(); 

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ProjectileBasedMG = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> FireTraceObjects;

	UPROPERTY(BlueprintReadOnly)  
	float Cooldown = 100; 
	
	UPROPERTY()
	int32 ShootingTime = 0;

	UPROPERTY()
	int32 FiringTime = 0;   
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMax = 100.0f, ClampMin = 0), Category="Stats") 
	int32 CooldownRate = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMax = 1000.0f, ClampMin = 0), Category="Stats")  
	int32 DamageRate = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cooldown", meta=(ClampMin = 0, ClampMax = 15)) 
	float TimeBeforeRecovery = 2.0f;
	

	// Blueprint Events  
	UFUNCTION(BlueprintImplementableEvent)
	void MachineGunHitWorld(FHitResult Hit);

	UFUNCTION(BlueprintImplementableEvent)
	void MachineGunHitMech(FHitResult Hit);
	
	virtual void Fire() override;


protected:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;


	
};
