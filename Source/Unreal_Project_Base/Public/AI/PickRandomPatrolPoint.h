// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "PickRandomPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UPickRandomPatrolPoint : public UEnvQueryTest
{
	GENERATED_BODY()

	UPickRandomPatrolPoint();
	
	// Run Test to find a Find and Score a Random Patrol point
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
