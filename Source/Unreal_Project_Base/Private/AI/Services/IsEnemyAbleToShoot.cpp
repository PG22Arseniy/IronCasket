// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/IsEnemyAbleToShoot.h"
#include "AI/blackboard_keys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyAIController.h"
#include "AI/AIControllers/CannonAIController.h"
#include "Mech/CannonEnemy.h"
#include "Mech/MechEnemy.h"
#include "Weapon/WeponTypes/ProjectileWeapon.h"

UIsEnemyAbleToShoot::UIsEnemyAbleToShoot()
{

	NodeName = TEXT("IS ENEMY ABLE TO SHOOT");  
}


void UIsEnemyAbleToShoot::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// get NPC
	ACannonAIController* const AIController = Cast <ACannonAIController>(OwnerComp.GetAIOwner());
	ACannonEnemy* Enemy = AIController->GetPawn<ACannonEnemy>(); 

	
	// if no Weapon , the cannot attack
	if (!Enemy->CannonWeapon)
	{
		AIController->GetBlackboard()->SetValueAsBool(bb_keys::UnableToShoot, false);
	} 


	// check if weapon can shoot  
	if (AProjectileWeapon* Weapon = Cast<AProjectileWeapon>(Enemy->CannonWeapon->GetChildActor())) 
	{
		AIController->GetBlackboard()->SetValueAsBool(bb_keys::UnableToShoot, !Weapon->CanShoot) ;  
	}
	else
	{
		AIController->GetBlackboard()->SetValueAsBool(bb_keys::UnableToShoot, false);
	}


	// Check If In shooting Angle 
	if (AIController->PlayerInSight)
	{
		if (Enemy->InCircle)
		AIController->GetBlackboard()->SetValueAsBool(bb_keys::InShootingAngle, true);

		else
		{
			const FRotator Rot = Enemy-> GetActorRotation();
			const FVector Loc = Enemy->GetActorLocation();

			const FVector PlayerLoc = AIController->GetBlackboard()->GetValueAsVector(bb_keys::TargetLocation);

			const FRotator NeededRot = (PlayerLoc-Loc).Rotation();

			if(FMath::Abs(NeededRot.Yaw - Rot.Yaw) < 30)
			{
				AIController->GetBlackboard()->SetValueAsBool(bb_keys::InShootingAngle, true);
			}
			else 
			{
				AIController->GetBlackboard()->SetValueAsBool(bb_keys::InShootingAngle, false);
			}
		}
	}
	else  
	{
		AIController->GetBlackboard()->SetValueAsBool(bb_keys::InShootingAngle, false);
	}

	
}


