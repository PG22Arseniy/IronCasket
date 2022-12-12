// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "IsEnemyCloseToItemInCircle.generated.h"


class AEnemyAIController;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UIsEnemyCloseToItemInCircle : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	
	UIsEnemyCloseToItemInCircle();

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AI", meta=(ClampMin = 0, ClampMax = 500))
	float MinDistToItemInCircle = 50.0f;
	

	UPROPERTY()
	AEnemyAIController* AIController;   
	
	
};
