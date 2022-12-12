// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"


class UTextBlock;


/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock * EnemiesKilled;

	UPROPERTY(meta=(BindWidget))
	UTextBlock * EnemiesToKill;


protected:

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	FText GetEnemiesKilled();

	UFUNCTION()
	FText GetEnemiesToKill();
};
