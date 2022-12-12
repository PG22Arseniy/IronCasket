// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GroupAISubsystem.h"

#include "Mech/MinigunEnemy.h"

void UGroupAISubsystem::AddAttackingMinigun(AMinigunEnemy* MinigunEnemy)
{
	AttackingMiniguns.AddUnique(MinigunEnemy);
}

void UGroupAISubsystem::RemoveAttackingMinigun(AMinigunEnemy* MinigunEnemy)
{
	AttackingMiniguns.Remove(MinigunEnemy); 
}

int32 UGroupAISubsystem::GetMinigunNumberInAttack(AMinigunEnemy* MinigunEnemy)
{
	return (AttackingMiniguns.Find(MinigunEnemy)+1); 
}

TArray<AMinigunEnemy*> UGroupAISubsystem::GetAttackingMinigunArray() const
{
	return AttackingMiniguns; 
}

void UGroupAISubsystem::AddAttackingCannon(ACannonEnemy* CannonEnemy)
{
	AttackingCannons.AddUnique(CannonEnemy);
}

void UGroupAISubsystem::RemoveAttackingCannon(ACannonEnemy* CannonEnemy)
{
	AttackingCannons.Remove(CannonEnemy);
}

int32 UGroupAISubsystem::GetCannonNumberInAttack(ACannonEnemy* CannonEnemy)
{
	return (AttackingCannons.Find(CannonEnemy)+1); 
}


TArray<ACannonEnemy*> UGroupAISubsystem::GetAttackingCannonArray() const
{
	return  AttackingCannons; 
}

AMinigunEnemy* UGroupAISubsystem::GetMinigunEnemyToCutOff()
{

	if (AttackingMiniguns.Num() > 0)
	{
		if (AttackingCannons.Num() > 0)
		{
			return AttackingMiniguns[0];
		}

		if (AttackingMiniguns.Num()>1)
		{
			return AttackingMiniguns[1];
		}
	}

	return nullptr;
	
}
