// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "CanEnemySeePlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UCanEnemySeePlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UCanEnemySeePlayer();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
