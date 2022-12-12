// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/IsPlayerInMachineGunRange.h"
#include "Mech/MechPlayer.h"
#include "AI/blackboard_keys.h"
#include "AI/EnemyAIController.h"
#include "AI/GroupAISubsystem.h"
#include "AI/AIControllers/MinigunAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Mech/MechController.h"
#include "Mech/MechEnemy.h"
#include "Mech/MinigunEnemy.h"

UIsPlayerInMachineGunRange::UIsPlayerInMachineGunRange()
{
	NodeName = TEXT("IS PLAYER IN ATTACKING RANGE"); 
}

void UIsPlayerInMachineGunRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UWorld * World = GetWorld();

	if (!World) return;
	
	// get NPC and location
	AEnemyAIController* const AIController = Cast <AEnemyAIController>(OwnerComp.GetAIOwner()); 
	AMechEnemy* Enemy = AIController->GetPawn<AMechEnemy>();

	const FVector EnemyLoc = Enemy->GetActorLocation(); 
	
	
	// get player character location
	
	const FVector PlayerLocation = AIController->GetBlackboard()->GetValueAsVector(bb_keys::TargetLocation); 
	
	const float DistanceToPlayer = FVector::Dist(PlayerLocation, Enemy->GetActorLocation());

	int32 NumberInAttack;
	
	if (AMinigunEnemy* MinigunEnemy = Cast<AMinigunEnemy>(Enemy))
	{
		NumberInAttack =  World->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<UGroupAISubsystem>()->GetMinigunNumberInAttack(MinigunEnemy);
	}
	else
	{
		NumberInAttack = 0; 
	}

	MachineGunRange = NumberInAttack * 1000 + 1200;          
	
	
	const bool InMinRange = DistanceToPlayer <= MachineGunRange; 
	

	if (InMinRange != PreviousState) 
	{
		if (InMinRange)
		{
			FRotator DirectionToRotate = (PlayerLocation - EnemyLoc).Rotation();
			Enemy->RightDirectionCircle = FMath::RandBool();  
		//	Enemy->RightDirectionCircle = false;     
			
		 	if (Enemy->RightDirectionCircle) 
	
		 	{
		 		DirectionToRotate.Yaw += 90;
		 		Enemy->YawRotation = -150; 
		 		
		 	}
		 	else
		 	{ 
		 		DirectionToRotate.Yaw -= 90;
		 		Enemy->YawRotation = 110;       
		 	} 

			Enemy->SetActorRotation(DirectionToRotate); 

			Enemy->InCircle = true; 
		}
		else
		{
			Enemy->YawRotation = 0;
			Enemy->InCircle = false; 
		}
	}
	PreviousState = InMinRange;
	
	// check if within range
	AIController->GetBlackboard()->SetValueAsBool(bb_keys::PlayerInRange, InMinRange);   
} 
