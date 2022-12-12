// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "SetRightRotation.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API USetRightRotation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:

	USetRightRotation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Hiding = false;


	UPROPERTY()
	FVector TargetLoc;  
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
