// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ObjectiveWidget.h"
#include "Components/TextBlock.h"
#include "Unreal_Project_Base/MechGameModeBase.h"


void UObjectiveWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	EnemiesKilled->TextDelegate.BindDynamic(this, &UObjectiveWidget::GetEnemiesKilled);
	EnemiesToKill->TextDelegate.BindDynamic(this, &UObjectiveWidget::GetEnemiesToKill);
}

FText UObjectiveWidget::GetEnemiesKilled()
{
	UWorld* World = GetWorld();
	if (!World) return FText();

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return FText();

	return FText::AsNumber(MechMode->CurrentEnemiesKilled);
}

FText UObjectiveWidget::GetEnemiesToKill()
{
	UWorld* World = GetWorld();
	if (!World) return FText();

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return FText();

	return FText::AsNumber(MechMode->EnemiesToKill);  
}
