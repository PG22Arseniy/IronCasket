// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllers/MinigunAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/blackboard_keys.h"
#include "AI/GroupAISubsystem.h"
#include "Mech/MechPlayer.h"
#include "Mech/MinigunEnemy.h"

void AMinigunAIController::OnTargetDetected(AActor* TargetActor, FAIStimulus const Stimulus)
{
	if (AMechPlayer* MechPlayer = Cast<AMechPlayer>(TargetActor) )
	{

		PlayerInSight = Stimulus.WasSuccessfullySensed();

		GetBlackboard()->SetValueAsBool(bb_keys::CanSeePlayer, PlayerInSight);   
		
		AMinigunEnemy* EnemyMech = GetPawn<AMinigunEnemy>();
		
		if (!EnemyMech) return;
		
		if (PlayerInSight ) 
		{
			GetBlackboard()->SetValueAsBool(bb_keys::CanGenerateCircle, true);    
			GetWorldTimerManager().ClearTimer(SensingTimerHandle);
			
			GetBlackboard()->SetValueAsBool(bb_keys::PlayerDetected, true);
			

			for (AMechEnemy* EnemyInGroup : EnemyMech->CombatGroup)
			{
				if (!EnemyInGroup) return; 
				
				if (AEnemyAIController* cont = EnemyInGroup->GetController<AEnemyAIController>()) 
				{
					//cont->GetBlackboard()->SetValueAsVector(bb_keys::TargetLocation, Stimulus.StimulusLocation);    
					cont->GetBlackboard()->SetValueAsBool(bb_keys::PlayerDetected, true);    
				}
			}

			// add name to Main Player's list of attacking minigun AIs

			GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<UGroupAISubsystem>()->AddAttackingMinigun(EnemyMech);
			

			// get the number in the list 
		
			EnemyMech->NumberInAttack = GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<UGroupAISubsystem>()->GetMinigunNumberInAttack(EnemyMech);  
  
		}
		else
		{
			// remove from attacking list
			GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<UGroupAISubsystem>()->RemoveAttackingMinigun(EnemyMech);

			// Change Attacking Style
			EnemyMech->AttackChange();

			// Stop Shooting mini guns
			EnemyMech->StopShootingLeftArmGun(); 
			EnemyMech->StopShootingRightArmGun(); 

			GetWorldTimerManager().SetTimer(SensingTimerHandle, this, &AEnemyAIController::ForgetAboutLastKnownPosition, MaxAgeOfLastKnownPosition);
		}
	}
}
