// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/SetRightRotation.h"

#include "AI/EnemyAIController.h"
#include "AI/AIControllers/CannonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USetRightRotation::USetRightRotation()
{
	NodeName = TEXT("ROTATE");  
}

void USetRightRotation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACannonAIController* const AIController = Cast <ACannonAIController>(OwnerComp.GetAIOwner());
	AMechEnemy* Enemy = AIController->GetPawn<AMechEnemy>();

	const FVector Loc = Enemy->GetActorLocation();
	FRotator Rot = Enemy->GetActorRotation();
	
	
	TargetLoc = AIController->GetBlackboard()->GetValueAsVector("TargetLocation");
	 
	
	const FRotator DesiredRot = ( TargetLoc- Loc).Rotation(); 

	
	const FRotator DeltaRot = FMath::Lerp(Rot, DesiredRot, DeltaSeconds);

	Rot.Yaw = DeltaRot.Yaw;  
	
	Enemy->SetActorRotation(Rot);

}
