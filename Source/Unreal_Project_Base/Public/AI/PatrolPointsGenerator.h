// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "PatrolPointsGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UPatrolPointsGenerator : public UEnvQueryGenerator
{
	GENERATED_BODY()
	
public:
	UPatrolPointsGenerator(const FObjectInitializer& ObjectInitializer);
	
	// Function that Generates Query Points
	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;
	//virtual bool IsValidGenerator() const override;


	// Functions that Creates the Title and Details section of the Query
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
	
};
