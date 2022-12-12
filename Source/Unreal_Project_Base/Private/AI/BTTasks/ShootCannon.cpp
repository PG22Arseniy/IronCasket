// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/ShootCannon.h"

#include "AI/blackboard_keys.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Mech/CannonEnemy.h"
#include "Mech/MechEnemy.h"

UShootCannon::UShootCannon(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("SHOOT CANNON");
}

EBTNodeResult::Type UShootCannon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* const AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Succeeded;;

	
	ACannonEnemy* Enemy = AIController->GetPawn<ACannonEnemy>();
	if (!Enemy) return EBTNodeResult::Succeeded;


	if (!Enemy->CannonWeapon) return EBTNodeResult::Succeeded;     
	
	if ( Enemy->CanAttack ) Enemy->ShootCannonWeapon();  
	
	
	return EBTNodeResult::Succeeded;  
}
