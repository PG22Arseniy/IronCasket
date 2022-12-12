// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeponTypes/ProjectileWeapon.h"
#include "Weapon/Weapon.h"
#include "Mech/MechPlayer.h"
#include "Weapon/WeaponData.h"

void AProjectileWeapon::Fire()
{
	Super::Fire();  
	
	UWorld* world = GetWorld();
	if (!world) return;

	
	// Get Instigator
	APawn* instigator = Cast<APawn>(GetParentActor()); 
	if (!instigator) return;

	// Set Target for Player 
	AMechPlayer* PlayerMech = Cast<AMechPlayer>(instigator);

	if (PlayerMech) 
	PlayerMech->SetWeaponTarget(this);

		// TODO: event for player shot
	 

	
	// PROJECTILE Based Weapon

	if (!CanShoot) return;  
	if (!WeaponData) return;

	if (WeaponData->CurrentAmmo <= 0) return;

	if (Reloading && !WeaponData->AllowShootingWhileReload) return;   

	// Set Spawn Parameters
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = instigator;
	SpawnParams.Owner = instigator;
	
	const FRotator ShotDirection  = ( TargetForBullet - GetActorLocation()).Rotation() ; 
	

	if (!WeaponData->Projectile) return; 

	// Spawn Projectile
	AProjectile* projectile = world->SpawnActor<AProjectile>(WeaponData->Projectile,Nozzle->GetComponentLocation() + instigator->GetControlRotation().Vector() * 50, ShotDirection, SpawnParams);
	if (!projectile) return;

	if (AMech* Mech = Cast<AMech>(instigator)) 
	{
		Mech->AddHeat( WeaponData->HeatCost);  
	} 

	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);  
	Reloading = false;  
	
	// Set Projectile parameters
	projectile->PointDamageForMech = WeaponData->DamageForMech;
	projectile->PointDamageForWorld = WeaponData->DamageForDestructible;
	projectile->PenetrationDepth = WeaponData->PenetrationDepth;
	projectile->ImpulseStrength = WeaponData->ImpulseStrength; 


	WeaponFired(); 

	if (PlayerMech)
	{
		PlayerShot();  
	}
 
	// Decrease Ammo

	
	AMechGameModeBase* MechMode = world->GetAuthGameMode<AMechGameModeBase>();
	
	if (!MechMode) return;

	if (!MechMode->InfiniteAmmo && !WeaponData->InfiniteAmmo) 
	{

		if (WeaponData->CurrentAmmo > 0)
			WeaponData->CurrentAmmo--;
		
		if (WeaponData->CurrentAmmo <= 0 )
		{
			Reload();
			return; 
		}
	}

	
	// Weapon Can't shoot on cooldown
	CanShoot = false;
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AWeapon::AllowShooting, WeaponData->TimeBetweenShots, false);

}

void AProjectileWeapon::AllowShooting()
{
	Super::AllowShooting();

	// If Player holds the automatic Weapon
	if (Firing && WeaponData->Automatic) 
	{
		Reloading = false;

		// Set Target Before Firing for this weapon
		if (AMech* Mech = Cast<AMech>(GetOwner())) 
		{
			Mech->SetWeaponTarget(this);
		}

		Fire();   
	}
}

void AProjectileWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	

	if (GetWorldTimerManager().TimerExists(ShotTimerHandle))
	{
		// Get Current Cooldown Value (Time Elapsed Since Timer Started)
		Cooldown = GetWorldTimerManager().GetTimerElapsed(ShotTimerHandle);

		
	}
	else
	{
		Cooldown = -1;   
	}
	
	WeaponData->CurrentReloadTimeValue = GetCooldown();
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaSeconds, FColor::Blue, FText::AsNumber(WeaponData->CurrentReloadTimeValue).ToString());   
}

float AProjectileWeapon::GetCooldown() 
{
	// Convert to 0 - 1 Value
	
	if (Cooldown == -1) return 1; 
	return Cooldown / WeaponData->TimeBetweenShots;
}
 




