// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MechPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AMechPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AMechPlayerState();

	virtual void BeginPlay() override;


private:

	
	
};
