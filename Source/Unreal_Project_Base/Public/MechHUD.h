// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MechHUD.generated.h"


class UAmmoWidget;
class UDebugWidget;
class UObjectiveWidget;
class ULossScreenWidget; 
class UWinScreenWidget;


/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AMechHUD : public AHUD 
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	UAmmoWidget* AmmoWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	TSubclassOf<UAmmoWidget> AmmoWidgetClass;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDebugWidget* DebugWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	TSubclassOf<UDebugWidget> DebugWidgetClass;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UObjectiveWidget* ObjectiveWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	TSubclassOf<UObjectiveWidget> ObjectiveWidgetClass;
	
	UFUNCTION(BlueprintCallable)
	void ToggleDebug();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULossScreenWidget* LossScreenWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	TSubclassOf<ULossScreenWidget> LossScreenWidgetClass;

	UFUNCTION(BlueprintCallable)
	void OpenLossScreen();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UWinScreenWidget* WinScreenWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	TSubclassOf<UWinScreenWidget> WinScreenWidgetClass;

	UFUNCTION(BlueprintCallable)
	void OpenWinScreen();
	

};
