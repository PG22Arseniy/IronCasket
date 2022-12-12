// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class UNREAL_PROJECT_BASE_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

	FVector GetPatrolPoint(const int& index) const;

	int GetNum() const;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(MakeEditWidget = "true", AllowPrivateAccess))
	TArray<FVector> PatrolPoints; 
	

};
