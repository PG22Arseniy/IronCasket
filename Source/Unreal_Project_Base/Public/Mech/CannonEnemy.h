// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mech/MechEnemy.h"
#include "CannonEnemy.generated.h"

class AWeapon;
class ACannonAIController;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API ACannonEnemy : public AMechEnemy
{
	GENERATED_BODY()

public:

	ACannonEnemy();
	
	virtual void SetWeaponTarget(AWeapon* Weapon) override;

	virtual void RotateCorrectly(float DeltaSeconds) override;
 
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void SetDamage(float Damage, FVector HitLocation, const UDamageType* DamageType, AActor* DamageCauser) override;

	virtual void Death() override;
	
	UPROPERTY()
	bool UnderAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin =0, ClampMax = 100))
	float HidingTime = 10;

	UPROPERTY()
	FTimerHandle HideTimerHandle;

	UFUNCTION()
	void StopHiding();
	
	UFUNCTION()
	void AllowAttack ();


};
