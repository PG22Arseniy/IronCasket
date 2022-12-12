// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "IsEnemyAbleToShoot.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UIsEnemyAbleToShoot : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	
	UIsEnemyAbleToShoot(); 
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
