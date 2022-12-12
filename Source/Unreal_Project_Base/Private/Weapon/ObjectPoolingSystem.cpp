// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ObjectPoolingSystem.h"

void UObjectPoolingSystem::ReturnProjectile(AProjectile* Projectile)
{
	Projectile->SetHidden(true);
	
	ProjectileArray.Enqueue(Projectile);
}

AProjectile* UObjectPoolingSystem::GetProjectile()
{
	AProjectile* Projectile = nullptr;
	
	if (! ProjectileArray.IsEmpty())
	{
		ProjectileArray.Dequeue(Projectile);

		Projectile->SetHidden(false);
	}

	return Projectile;   
}
