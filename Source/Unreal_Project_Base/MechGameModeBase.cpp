// Fill out your copyright notice in the Description page of Project Settings.


#include "MechGameModeBase.h"

#include "MechHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Mech/MechController.h"
#include "Mech/MechPlayer.h"
#include "Mech/MechPlayerState.h"

AMechGameModeBase::AMechGameModeBase()
{
	PlayerControllerClass = AMechController::StaticClass();
	DefaultPawnClass = AMechPlayer::StaticClass(); 
	PlayerStateClass = AMechPlayerState::StaticClass();
}

void AMechGameModeBase::EnemyKill()
{
	CurrentEnemiesKilled ++;
	if (CurrentEnemiesKilled >= EnemiesToKill) GameOver(true);
}

void AMechGameModeBase::GameOver(bool Victory)
{
	if (Victory)
	{
		WinEvent(); 
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Emerald, "VICTORY");
		//UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	}
	
	else
	{
		LoseEvent(); 
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Emerald, "YOU LOST"); 
		//UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");   
	}
}

void AMechGameModeBase::OpenGameOverScreen(bool Victory) 
{
	AMechController* MechController = GetWorld()->GetFirstPlayerController<AMechController>();
	if (!MechController) return;

	AMechHUD* MechHUD = MechController->GetHUD<AMechHUD>();
	if (!MechHUD) return;

	if (Victory)
	{
		MechHUD->OpenWinScreen();
	}
	else
	{
		MechHUD->OpenLossScreen(); 
	}
}

void AMechGameModeBase::GodModeToggle()
{
	GodMode = !GodMode; 
}

void AMechGameModeBase::OneShotOneKillToggle()
{
	OneShotOneKill =!OneShotOneKill;
}

void AMechGameModeBase::InfiniteHealthToggle()
{
	InfiniteHealth =! InfiniteHealth;
}

void AMechGameModeBase::InfiniteAmmoToggle()
{
	InfiniteAmmo=! InfiniteAmmo;   
}
