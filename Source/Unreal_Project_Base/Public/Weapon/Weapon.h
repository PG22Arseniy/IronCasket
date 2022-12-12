// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


class USceneComponent;
class UArrowComponent;
class UWeaponData;

UCLASS()
class UNREAL_PROJECT_BASE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* Nozzle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	UWeaponData* WeaponData;

	UPROPERTY(BlueprintReadOnly)  
	FVector TargetForBullet = {0,0,0};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadOnly)
	bool Reloading = false;  

	UPROPERTY(BlueprintReadOnly)
	bool Firing = false;
	
	UPROPERTY()
	bool CanShoot = true;

	UPROPERTY()
	FTimerHandle ShotTimerHandle;

	UPROPERTY()
	FTimerHandle ReloadTimerHandle;

	UPROPERTY()
	FTimerHandle AutoReloadTimerHandle; 

	UFUNCTION(BlueprintCallable)
	virtual void Fire(); 

	UFUNCTION(BlueprintCallable)
	virtual  void StopFiring();

	UFUNCTION(BlueprintCallable)
	virtual void Reload();
	
	UFUNCTION()
	virtual void AllowShooting() ;

	UFUNCTION()
	virtual void IncrementAmmo() ;
	
};
