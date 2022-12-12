// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DebugWidget.h"
#include "Components/TextBlock.h"
#include "Unreal_Project_Base/MechGameModeBase.h"

void UDebugWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	GodMode->TextDelegate.BindDynamic(this, &UDebugWidget::GetGodMode);
	InfiniteHealth->TextDelegate.BindDynamic(this, &UDebugWidget::GetInfiniteHealth);
	InfiniteAmmo->TextDelegate.BindDynamic(this, &UDebugWidget::GetInfiniteAmmo);  
	OneShotOneKill->TextDelegate.BindDynamic(this, &UDebugWidget::GetOneShotOneKill);
}

FText UDebugWidget::GetGodMode()
{
	UWorld* World = GetWorld();
	if (!World)  return FText();

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return FText();
	
	if (MechMode->GodMode)
		return FText::FromString("YES");
	
	return FText::FromString("NO"); 
}

FText UDebugWidget::GetInfiniteHealth()
{
	UWorld* World = GetWorld();
	if (!World)  return FText();

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return FText();


	if (MechMode->InfiniteHealth)
		return FText::FromString("YES");
	
	return FText::FromString("NO");
}

FText UDebugWidget::GetInfiniteAmmo()
{
	UWorld* World = GetWorld();
	if (!World)  return FText();

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return FText();


	if (MechMode->InfiniteAmmo)
		return FText::FromString("YES");
	
	return FText::FromString("NO");
}

FText UDebugWidget::GetOneShotOneKill()
{
	UWorld* World = GetWorld();
	if (!World)  return FText();

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return FText();


	if (MechMode->OneShotOneKill)
		return FText::FromString("YES");
	
	return FText::FromString("NO");
}
