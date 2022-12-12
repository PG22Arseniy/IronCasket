// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Character.h"
#include "Mech/Mech.h"
#include "BaseDamage.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UBaseDamage : public UDamageType
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=( ClampMin=0 ))
	float DamageMultiplier = 1;

	/**
	 * Process the damage and return the damage multiplier from the hit
	 * @param Owner			Character that caused this attack
	 * @param HitCharacter	Player Character that was hit
	 * @param HitLocation	Location the player was hit
	 */
	
	UFUNCTION()
	virtual float ProcessDamage(AActor* Owner, APawn* HitCharacter, FVector HitLocation) const;
};
