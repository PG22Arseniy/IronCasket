// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Mech/MechEnemy.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"



class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAISenseConfig_Sight; 

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	
	AEnemyAIController(FObjectInitializer const & ObjectInitializer = FObjectInitializer::Get()); 

	void OnPossess(APawn* InPawn) override;

	UBlackboardComponent* GetBlackboard() const; 
	
	virtual void BeginPlay() override;


	UPROPERTY()
	FTimerHandle SensingTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float MaxAgeOfLastKnownPosition = 15.0f;

	UFUNCTION()
	void ForgetAboutLastKnownPosition();

	UPROPERTY(BlueprintReadOnly)
	bool PlayerInSight = false;

	UFUNCTION(BlueprintCallable)
	void StopAIBehaviour();

	UFUNCTION(BlueprintCallable)
	void ContinueAIBehaviour();


private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta = (AllowPrivateAccess= "true"))  
	UBehaviorTree* AIBehaviourTree;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite, Category="AI", meta = (AllowPrivateAccess= "true"))
	UBlackboardComponent* AIBlackBoardComp;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite, Category="AI", meta = (AllowPrivateAccess= "true"))
	UBehaviorTreeComponent* AIBehaviorTreeComp;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	virtual void OnTargetDetected(AActor* TargetActor, FAIStimulus const Stimulus); 
	
	void SetupPerceptionSystem();
	
	
};

