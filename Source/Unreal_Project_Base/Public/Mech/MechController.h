// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MechController.generated.h"


class ADebugCamera;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AMechController : public APlayerController
{
	GENERATED_BODY()

	virtual void SetupInputComponent() override;

public:
	 
	UFUNCTION()
	void ToggleDebug();
	
	UFUNCTION()
	void ToggleGodMode();

	UFUNCTION()
	void ToggleInfiniteHealth();

	UFUNCTION()
	void ToggleInfiniteAmmo();

	UFUNCTION()
	void ToggleOneShotOneKill();
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	TSubclassOf<APawn> DebugCamera;
	
	
};
