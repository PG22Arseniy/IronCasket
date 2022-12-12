// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"



class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public: 

	UPROPERTY(meta=(BindWidget))
	UTextBlock * CurrentHealthText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock * MaxHealthText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock * LeftMachineGunCooldownText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock * RightMachineGunCooldownText; 


protected:
	
	virtual  void NativeOnInitialized() override;
	

	UFUNCTION()
	FText GetCurrentHealth();

	UFUNCTION()
	FText GetMaxHealth();

	UFUNCTION()
	FText GetLeftMachineGunCooldown();

	UFUNCTION()
	FText GetRightMachineGunCooldown();
	
	
};
