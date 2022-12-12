// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyAIController.h"
#include "MinigunAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AMinigunAIController : public AEnemyAIController
{
	GENERATED_BODY()

public:
	
	virtual void OnTargetDetected(AActor* TargetActor, FAIStimulus const Stimulus) override;
	
	
};
