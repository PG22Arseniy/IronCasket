// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PickRandomPatrolPoint.h"

#include "AI/PatrolPoint.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

UPickRandomPatrolPoint::UPickRandomPatrolPoint()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UPickRandomPatrolPoint::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}
	// Query Iterator
	
	for (FEnvQueryInstance::ItemIterator PatrolPointsIterator(this, QueryInstance); PatrolPointsIterator; ++PatrolPointsIterator)
	{
		if (AActor* ItemActor = GetItemActor(QueryInstance, PatrolPointsIterator.GetIndex()))
		{
			// Check if the QueryInstance Actor is a patrolPoint
			if (ItemActor) 
			{
				// Giving Random Scores to different Patrol Points , you can add Distance Based Test
				PatrolPointsIterator.SetScore(TestPurpose, FilterType, FMath::RandRange(0,100), 0, 100);
				continue;
			}
		}
		
		// Setting the Score per Patrol Point
		PatrolPointsIterator.SetScore(TestPurpose, FilterType, 0, 0, 500);
	}
}
