// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DamagingInterface.h"
#include "Limb.generated.h"

class UBoxComponent;
class USceneComponent; 

UCLASS()
class UNREAL_PROJECT_BASE_API ALimb : public AActor, public IDamagingInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALimb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UBoxComponent* BoxComp;

	UPROPERTY()
	USceneComponent* SceneComp; 

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Health") 
	float Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Health")
	float MaxHealth = 100;
	
	
	UFUNCTION()
	void SetDamagePoint(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void SetDamageRadial(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
	FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);

	void SetDamage(float Damage, FVector HitLocation, const UDamageType* DamageType, AActor* DamageCauser);
};
