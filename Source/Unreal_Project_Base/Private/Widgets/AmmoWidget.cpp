// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AmmoWidget.h"
#include "Components/TextBlock.h"
#include "Weapon/WeaponData.h" 
#include "Mech/MechPlayer.h"
#include "Weapon/WeponTypes/MachineGun.h"
#include "Weapon/WeponTypes/ProjectileWeapon.h"

void UAmmoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CurrentHealthText->TextDelegate.BindDynamic(this, &UAmmoWidget::GetCurrentHealth);
	MaxHealthText->TextDelegate.BindDynamic(this, &UAmmoWidget::GetMaxHealth); 
	
	LeftMachineGunCooldownText->TextDelegate.BindDynamic(this, &UAmmoWidget::GetLeftMachineGunCooldown);
	RightMachineGunCooldownText->TextDelegate.BindDynamic(this, &UAmmoWidget::GetRightMachineGunCooldown); 
}

// PLAYER HEALTH

FText UAmmoWidget::GetCurrentHealth()
{
	if (AMechPlayer* Player = GetOwningPlayerPawn<AMechPlayer>())
	{
		return  FText::AsNumber(Player->Health);
	}

	return FText();

} 

FText UAmmoWidget::GetMaxHealth()
{
	if (AMechPlayer* Player = GetOwningPlayerPawn<AMechPlayer>())
	{
		return  FText::AsNumber(Player->MaxHealth);
	}

	return FText();
}

// MACHINE GUNs COOLDOWN

FText UAmmoWidget::GetLeftMachineGunCooldown()
{
	if (AMechPlayer* Player = GetOwningPlayerPawn<AMechPlayer>())
	{

		if (!Player->LeftArmGun) return FText();


		AWeapon* Weapon = Cast<AWeapon>(Player->LeftArmGun->GetChildActor()); 


		if (!Weapon) return FText();
		
		return  FText::AsNumber(Weapon->WeaponData->CurrentAmmo);     
		
	}

	return FText();
}



FText UAmmoWidget::GetRightMachineGunCooldown()
{
	if (AMechPlayer* Player = GetOwningPlayerPawn<AMechPlayer>())
	{

		if (!Player->RightArmGun) return FText();
		
		AProjectileWeapon* Weapon = Cast<AProjectileWeapon>(Player->RightArmGun->GetChildActor());
		
		if (!Weapon) return FText();
		
		if (!Weapon->WeaponData) return FText(); 
		
		return  FText::AsNumber(Weapon->WeaponData->CurrentAmmo);    
		
	}

	return FText();
}










