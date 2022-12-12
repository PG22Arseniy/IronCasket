// Fill out your copyright notice in the Description page of Project Settings.


#include "MechHUD.h"

#include "Kismet/GameplayStatics.h"
#include "Mech/MechController.h"
#include "Widgets/AmmoWidget.h"
#include "Widgets/DebugWidget.h"
#include "Widgets/LossScreenWidget.h"
#include "Widgets/WinScreenWidget.h" 
#include "Widgets/ObjectiveWidget.h"

void AMechHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if ( ! GetOwningPlayerController()) return;
	
	if (! AmmoWidgetClass) return;
	
	
	if (!AmmoWidget) 
	{
		AmmoWidget = CreateWidget<UAmmoWidget>( GetOwningPlayerController(), AmmoWidgetClass, TEXT ("AmmoWidget"));
		AmmoWidget->AddToViewport(5); 
	}

	if (! ObjectiveWidgetClass) return;
	
	if (!ObjectiveWidget) 
	{
		ObjectiveWidget = CreateWidget<UObjectiveWidget>( GetOwningPlayerController(), ObjectiveWidgetClass, TEXT ("ObjectiveWidget")); 
		ObjectiveWidget->AddToViewport(4);   
	}
}

void AMechHUD::ToggleDebug()
{
	UWorld * world = GetWorld();
	if (!world) return;

	APlayerController * PlayerController = world->GetFirstPlayerController();
	if (!PlayerController) return;

	if (!DebugWidget) 
	{
		
		DebugWidget = CreateWidget<UDebugWidget>(GetOwningPlayerController(), DebugWidgetClass, TEXT ("DEBUGS"));

		DebugWidget->AddToViewport(3);   
	}
	else
	{
		DebugWidget->RemoveFromParent(); 

		DebugWidget = nullptr; 
	}
}

void AMechHUD::OpenLossScreen()
{
	UWorld * world = GetWorld();
	if (!world) return;

	APlayerController * PlayerController = world->GetFirstPlayerController();
	if (!PlayerController) return;

	if (!LossScreenWidget) 
	{
		
		LossScreenWidget = CreateWidget<ULossScreenWidget>(GetOwningPlayerController(), LossScreenWidgetClass, TEXT ("LOSS"));

		LossScreenWidget->AddToViewport(3);   
	}
}

void AMechHUD::OpenWinScreen()
{
	UWorld * world = GetWorld();
	if (!world) return;

	APlayerController * PlayerController = world->GetFirstPlayerController();
	if (!PlayerController) return;

	if (!WinScreenWidget) 
	{
		
		WinScreenWidget = CreateWidget<UWinScreenWidget>(GetOwningPlayerController(), WinScreenWidgetClass, TEXT ("WIN"));

		WinScreenWidget->AddToViewport(3);   
	} 
}

