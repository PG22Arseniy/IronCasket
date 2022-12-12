// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RotateTowardsTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API URotateTowardsTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	URotateTowardsTarget(FObjectInitializer const & ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 
	
};
