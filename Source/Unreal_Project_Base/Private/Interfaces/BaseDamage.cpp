// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/BaseDamage.h"

float UBaseDamage::ProcessDamage(AActor* Owner, APawn* HitCharacter, FVector HitLocation) const
{
	return DamageMultiplier; 
}
