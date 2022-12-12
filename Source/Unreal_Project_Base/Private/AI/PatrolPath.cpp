// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolPath.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

FVector APatrolPath::GetPatrolPoint(const int& index) const
{
	return PatrolPoints[index];
}

int APatrolPath::GetNum() const
{
	return PatrolPoints.Num(); 
}


