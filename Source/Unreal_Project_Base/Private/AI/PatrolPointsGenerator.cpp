// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolPointsGenerator.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "AI/PatrolPoint.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "Mech/MechEnemy.h"

UPatrolPointsGenerator::UPatrolPointsGenerator(const FObjectInitializer& ObjectInitializer)
{
	ItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UPatrolPointsGenerator::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	UObject* queryOwner = QueryInstance.Owner.Get();
	if (queryOwner == nullptr)
	{
		return;
	}
	// Getting World and checking
	UWorld* world = GEngine->GetWorldFromContextObject(queryOwner, EGetWorldErrorMode::LogAndReturnNull);
	if (!world)
	{
		return;
	}

	AMechEnemy* Enemy = Cast<AMechEnemy> (queryOwner);
	
	TArray<APatrolPoint*> MatchingActors = Enemy->PatrolPoints;
	// We need the Generator to Accept a AActor type since we use UEnvQueryItemType_Actor  , it will not accept any subclass hence we store it in
	// a Array of AActor
	// for (TActorIterator<AActor> ItActor = TActorIterator<AActor>(world, APatrolPoint::StaticClass()); ItActor; ++ItActor)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("added new point"));
	//
	// 	MatchingActors.Add(*ItActor);
	// }
#if WITH_EDITOR
	
#endif
	
	
	QueryInstance.AddItemData<UEnvQueryItemType_Actor>(MatchingActors);

}

FText UPatrolPointsGenerator::GetDescriptionTitle() const
{
	return FText::FromString("Patrol Points");
}

FText UPatrolPointsGenerator::GetDescriptionDetails() const
{
	return FText::FromString("Generate a Query at Every Patrol Point");
}
