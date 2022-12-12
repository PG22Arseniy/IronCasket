// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugWidget.generated.h"


class UTextBlock; 

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UDebugWidget : public UUserWidget
{
	GENERATED_BODY()


	
public:

	UPROPERTY(meta=(BindWidget))
	UTextBlock * GodMode;

	UPROPERTY(meta=(BindWidget))
	UTextBlock * InfiniteHealth;

	UPROPERTY(meta=(BindWidget))
	UTextBlock * InfiniteAmmo;

	UPROPERTY(meta=(BindWidget))
	UTextBlock * OneShotOneKill;

protected:

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	FText GetGodMode();

	UFUNCTION()
	FText GetInfiniteHealth();

	UFUNCTION()
	FText GetInfiniteAmmo();
	
	UFUNCTION() 
	FText GetOneShotOneKill();

};
