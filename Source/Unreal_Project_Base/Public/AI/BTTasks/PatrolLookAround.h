// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Components/TimelineComponent.h"
#include "PatrolLookAround.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UPatrolLookAround : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UPatrolLookAround(FObjectInitializer const & ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0, ClampMax = 25))
	float LookAroundSpeed = 1.0f; 
// 	UFUNCTION()
// 	void TurnLineProgress(float Value);
//
//
// private:
//
// 	FTimeline ZoomInTimeline; 
};
