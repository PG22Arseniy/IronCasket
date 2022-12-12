// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent; 
class USphereComponent;
class AMechEnemy;

UCLASS()
class UNREAL_PROJECT_BASE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* ProjectileMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	virtual void Tick(float DeltaSeconds) override;

public:
	
	UFUNCTION()     
	void OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);  

	UPROPERTY() 
	int PenetrationDepth = 2;

	UPROPERTY()   
	int32 ImpulseStrength = 5;   

protected:

	UPROPERTY()
	int CurrentPenetratedBuildings = 0;

	UPROPERTY()
	FString LastBuildingName = "";

public:

	UPROPERTY()
	float PointDamageForMech = 5.0f;
	
	UPROPERTY()
	float PointDamageForWorld = 10.0f;

	UPROPERTY()
	float RadialDamageForMech = 5.0f;
	
	UPROPERTY()
	float RadialDamageForWorld = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage", meta=(ClampMin = 0)) 
	float ExplosionRadius = 10.0f;

	UFUNCTION()
	virtual void WorldHit(FHitResult Hit);

	UFUNCTION()
	virtual void MechHit(FHitResult Hit);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyEvent();


	UPROPERTY(BlueprintReadOnly)
	AMechEnemy* TargetEnemy;
	
};
