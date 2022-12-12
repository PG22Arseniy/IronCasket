// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

#include "Mech/MechPlayer.h"
#include "Weapon/WeaponData.h"  

// Sets default values
AWeapon::AWeapon()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SCENE"));
	SetRootComponent(SceneComponent);

	Nozzle = CreateDefaultSubobject<USceneComponent>(TEXT("Nozzle"));
	Nozzle->SetupAttachment(SceneComponent);
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Current Ammo Always = Magazines at the start
	WeaponData->CurrentAmmo = WeaponData->Magazine; 
}


void AWeapon::Fire()
{
	Firing = true;

	// Start Timer For Auto Reload 
	if (WeaponData->AutoReload)
	{
		GetWorldTimerManager().ClearTimer(AutoReloadTimerHandle);
		GetWorldTimerManager().SetTimer(AutoReloadTimerHandle, this, &AWeapon::Reload, WeaponData->AutoReloadTime, false);
	} 
}

void AWeapon::StopFiring()
{
	Firing = false;
}

void AWeapon::AllowShooting()
{
	CanShoot = true;
	
}

void AWeapon::IncrementAmmo()
{
	if (WeaponData->CurrentAmmo >= WeaponData->Magazine) 
	{
		GetWorldTimerManager().ClearTimer(ReloadTimerHandle); 

		Reloading = false;

		AllowShooting(); 
		
		return; 
	};
	
	WeaponData->CurrentAmmo ++; 

	if (WeaponData->AllowShootingWhileReload)
		AllowShooting();  
}

void AWeapon::Reload()
{
	if (Reloading) return;

	if (WeaponData->CurrentAmmo >= WeaponData->Magazine) return; 

	Reloading = true;

	// Increment Function repeatedly 
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AWeapon::IncrementAmmo, WeaponData->TimeBetweenReloads, true);
}





