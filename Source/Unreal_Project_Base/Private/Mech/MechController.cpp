// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech/MechController.h"
#include "MechHUD.h"
#include "Debug/DebugCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Mech/MechPlayer.h"
#include "Unreal_Project_Base/MechGameModeBase.h"
#include "Weapon/WeponTypes/MachineGun.h"

void AMechController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ToggleDebug", IE_Pressed, this, &AMechController::ToggleDebug); 
	InputComponent->BindAction("GodMode", IE_Pressed, this, &AMechController::ToggleGodMode); 
	InputComponent->BindAction("InfiniteHealth", IE_Pressed, this, &AMechController::ToggleInfiniteHealth); 
	InputComponent->BindAction("InfiniteAmmo", IE_Pressed, this, &AMechController::ToggleInfiniteAmmo); 
	InputComponent->BindAction("OneShotOneKill", IE_Pressed, this, &AMechController::ToggleOneShotOneKill);    
}

void AMechController::ToggleDebug()
{
	if (AMechHUD* hud = GetHUD<AMechHUD>())
	{
		hud ->ToggleDebug(); 
	}
}



void AMechController::ToggleGodMode()
{
	UWorld* World = GetWorld();
	if (!World) return;

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return;

	MechMode->GodModeToggle();
	
	AMechPlayer* MechPlayer = GetPawn<AMechPlayer>();
	if (!MechPlayer) return;


	
	if (MechMode->GodMode)
	{
		MechMode->InfiniteHealth = false;
		MechMode->OneShotOneKill = false;

		ToggleOneShotOneKill();
		ToggleInfiniteHealth();
	}
	else
	{
		MechMode->InfiniteHealth = true;  
		MechMode->OneShotOneKill = true;

		ToggleOneShotOneKill(); 
		ToggleInfiniteHealth();
	}
}

void AMechController::ToggleInfiniteHealth()
{
	UWorld* World = GetWorld();
	if (!World) return;

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return;

	MechMode->InfiniteHealthToggle();


	AMechPlayer* MechPlayer = GetPawn<AMechPlayer>();
	if (!MechPlayer) return;

	if (MechMode->InfiniteHealth)
	{
		MechPlayer->Health = -1;
	}

	else
	{
		MechPlayer->RestoreHealth(); 
	} 
}

void AMechController::ToggleInfiniteAmmo()
{
	UWorld* World = GetWorld();
	if (!World) return;

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return;

	AMechPlayer* MechPlayer = GetPawn<AMechPlayer>();
	if (!MechPlayer) return;

	MechMode->InfiniteAmmoToggle();

	if (AMachineGun* Weapon = Cast<AMachineGun>(MechPlayer->LeftArmGun->GetChildActor()))
	{
		if (MechMode->InfiniteAmmo)
		Weapon->Cooldown = -1;

		else Weapon->Cooldown = 100;
		
	} 
	if (AMachineGun* Weapon = Cast<AMachineGun>(MechPlayer->RightArmGun->GetChildActor())) 
	{ 
		if (MechMode->InfiniteAmmo)
			Weapon->Cooldown = -1;

		else Weapon->Cooldown = 100;
	}
}

void AMechController::ToggleOneShotOneKill()
{
	UWorld* World = GetWorld();
	if (!World) return;

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return;

	MechMode->OneShotOneKillToggle();
}

