// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileTypes/Cannon.h"
#include "Kismet/GameplayStatics.h"



void ACannon::WorldHit(FHitResult Hit)
{
	Super::WorldHit(Hit);

	CannonHitWorld(Hit);
}

void ACannon::MechHit(FHitResult Hit)
{
	Super::MechHit(Hit);

	CannonHitMech(Hit);
}
 




