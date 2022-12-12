// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/CanEnemySeePlayer.h"

UCanEnemySeePlayer::UCanEnemySeePlayer()
{
	NodeName = TEXT("Can See Player");
}

void UCanEnemySeePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
}
