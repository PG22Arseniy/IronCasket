// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/PatrolLookAround.h"
#include "AI/EnemyAIController.h"
#include "Mech/MechEnemy.h"

UPatrolLookAround::UPatrolLookAround(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("PATROL LOOK AROUND");
}

EBTNodeResult::Type UPatrolLookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


	AEnemyAIController* const AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Succeeded;;

	
	AMechEnemy* Enemy = AIController->GetPawn<AMechEnemy>();
	if (!Enemy) return EBTNodeResult::Succeeded;


	Enemy->AddActorLocalRotation({0,LookAroundSpeed,0});   
	

	
	
	return EBTNodeResult::Succeeded;
}
