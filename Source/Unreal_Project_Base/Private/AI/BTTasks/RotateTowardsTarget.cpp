// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/RotateTowardsTarget.h"

#include "AI/blackboard_keys.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Mech/MechEnemy.h"

URotateTowardsTarget::URotateTowardsTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "ROTATE TOWARDS TARGET";
}

EBTNodeResult::Type URotateTowardsTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// Get Enemy Rotation And Position
	
	AEnemyAIController* const AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Succeeded;;
	
	AMechEnemy* Enemy = AIController->GetPawn<AMechEnemy>();
	if (!Enemy) return EBTNodeResult::Succeeded;

	const FVector Loc = Enemy->GetActorLocation();
	FRotator Rot = Enemy->GetActorRotation();
	const float YawRot = Enemy->YawRotation;
	

	
	// Get Target Location
	const FVector TargetLocation = AIController->GetBlackboard()->GetValueAsVector(bb_keys::PositionInCircle);
	
	
	// Calculate Wanted Direction
	const FRotator DirectionToTurn = (TargetLocation - Loc).Rotation(); 

	
	const float DeltaRot = UKismetMathLibrary::Lerp(Rot.Yaw, DirectionToTurn.Yaw, Enemy->CombatTurnSpeedTowardsPlayer);


	//Enemy->YawRotation -= DirectionToTurn.Yaw - Rot.Yaw; 
	
	Rot.Yaw = DirectionToTurn.Yaw;  
	 
	//Enemy->SetActorRotation(Rot);         
	
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, FString::FromInt(Rot.Yaw));
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, FString::FromInt(Enemy->YawRotation));

	// if (AIController->GetBlackboard()->GetValueAsBool(bb_keys::GoingBack))
	// {
	// 	//Enemy->YawRotation = -150;
	// }
	// else
	// {
	// 	//Enemy->YawRotation = 90;     
	// } 
 
	//Enemy->YawRotation = 90;    
	
	return EBTNodeResult::Succeeded; 


}
