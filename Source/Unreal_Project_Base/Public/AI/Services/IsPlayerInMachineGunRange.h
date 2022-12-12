// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "IsPlayerInMachineGunRange.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API UIsPlayerInMachineGunRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	
	UIsPlayerInMachineGunRange();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0, ClampMax= 50000, AllowPrivateAccess))
	float MachineGunRange = 6000.0f; 

	UPROPERTY()
	bool PreviousState = false;  
};
