// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/IsEnemyCloseToItemInCircle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/blackboard_keys.h"
#include "AI/EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Mech/MechEnemy.h"

UIsEnemyCloseToItemInCircle::UIsEnemyCloseToItemInCircle()
{
	NodeName = TEXT("Is Enemy Close to Item In Circle");

	bNotifyBecomeRelevant = true;
}

void UIsEnemyCloseToItemInCircle::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AIController = Cast <AEnemyAIController>(OwnerComp.GetAIOwner());

	if (!AIController) return;
	
	AMechEnemy* Enemy = AIController->GetPawn<AMechEnemy>();

	if (!Enemy) return;  

	const FVector EnemyLoc = Enemy->GetActorLocation();
	
	FRotator EnemyRot = Enemy->GetActorRotation();

	const FVector ItemLoc = AIController->GetBlackboard()->GetValueAsVector(bb_keys::PositionInCircle); 

	const float DistanceToItem = FVector::Dist(EnemyLoc, ItemLoc);
 

//	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Orange, FString::FromInt(DistanceToItem));   
	
	
	AIController->GetBlackboard()->SetValueAsBool(bb_keys::CloseToItemInCircle, DistanceToItem < MinDistToItemInCircle);

	

	
}

