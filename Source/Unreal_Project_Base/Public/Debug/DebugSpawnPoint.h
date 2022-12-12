// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MechGameStateBase.h"
#include "Engine/TargetPoint.h"
#include "DebugSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API ADebugSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level", meta=(ClampMin = 1, ClampMax = 4)) 
	int32 Level; 
};
