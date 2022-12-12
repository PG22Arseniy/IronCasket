// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllers/CannonAIController.h"

#include "AI/blackboard_keys.h"
#include "AI/GroupAISubsystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Mech/CannonEnemy.h"
#include "Mech/MechPlayer.h"

void ACannonAIController::OnTargetDetected(AActor* TargetActor, FAIStimulus const Stimulus)
{
	if (AMechPlayer* MechPlayer = Cast<AMechPlayer>(TargetActor) )
	{
		
		PlayerInSight = Stimulus.WasSuccessfullySensed();

		GetBlackboard()->SetValueAsBool(bb_keys::CanSeePlayer, PlayerInSight);
	
		
		ACannonEnemy* EnemyMech = GetPawn<ACannonEnemy>(); 
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

			// add name to Main Player's list of attacking Cannon AIs

			GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<UGroupAISubsystem>()->AddAttackingCannon(EnemyMech);
		}
		else
		{
			// Remove cannon from attacking Enemies 
			GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<UGroupAISubsystem>()->RemoveAttackingCannon(EnemyMech);

			// Stop Shooting 
			EnemyMech->StopShootingCannonWeapon(); 
			
			GetWorldTimerManager().SetTimer(SensingTimerHandle, this, &AEnemyAIController::ForgetAboutLastKnownPosition, MaxAgeOfLastKnownPosition);
		}
	}
}
