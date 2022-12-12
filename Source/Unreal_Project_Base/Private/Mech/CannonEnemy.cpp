// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech/CannonEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/blackboard_keys.h"
#include "AI/AIControllers/CannonAIController.h"
#include "Components/ArrowComponent.h"
#include "Mech/MechController.h"
#include "Mech/MechPlayer.h"
#include "Weapon/Weapon.h"

ACannonEnemy::ACannonEnemy()
{
	// Weapons Setup
	MechBody = GetMesh();
	
	CannonWeapon = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("CANNON WEAPON"), MechBody, TEXT("RightShoulder_Gun"));    
}

void ACannonEnemy::SetWeaponTarget(AWeapon* Weapon)
{
	
	const AMechPlayer* MainPlayer =  Cast<AMechPlayer>(PlayerMech); 
	if (!MainPlayer) return;

	// Get Location Of Player As Target 
	const FVector TargetLocation = MainPlayer->GetActorLocation();  
	
	
	Weapon->TargetForBullet = TargetLocation; 
	
}

void ACannonEnemy::RotateCorrectly(float DeltaSeconds)
{
	
	if (!AIController)
	{
		AIController = GetController<ACannonAIController>();
		return;  
	}

	if (AIController->PlayerInSight)
	{
		
		if (!SawBefore) 
		{
			GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ACannonEnemy::AllowAttack, TimeToAttack, false);
		}

		// Current Location, Rotation 
		FRotator Rot = GetActorRotation();
		const FVector Loc = GetActorLocation(); 

		FVector const PlayerLoc =  PlayerMech->GetActorLocation(); 
		
		// Rotate Upper body

		const FRotator DirectionToRotate = (PlayerLoc - Loc).Rotation(); 

		const float DesiredYawRot = DirectionToRotate.Yaw;

		//const float DeltaYawRot = FMath::Lerp(YawRotation, DesiredYawRot, DeltaSeconds); 

		YawRotation = DesiredYawRot  - Rot.Yaw  + 30;       
	
	}
	else 
	{
		CanAttack = false;
		
		if (SawBefore) 
		StopShootingCannonWeapon(); 
	}

	SawBefore = AIController->PlayerInSight; 
	
}

void ACannonEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACannonEnemy::BeginPlay() 
{
	
	Super::BeginPlay();

	AIController = GetController<ACannonAIController>();
	
}

void ACannonEnemy::SetDamage(float Damage, FVector HitLocation, const UDamageType* DamageType, AActor* DamageCauser)
{
	Super::SetDamage(Damage, HitLocation, DamageType, DamageCauser);
	
	
	if (AMechPlayer* Player = Cast<AMechPlayer> (DamageCauser))
	{

		if (UnderAttack) 
		{
			GetWorldTimerManager().ClearTimer(HideTimerHandle);
		}
		else
		{
			
			if (ACannonAIController* CannonAIController = GetController<ACannonAIController>())
			{
				CannonAIController->GetBlackboard()->SetValueAsBool(bb_keys::UnderAttack, true); 
			} 
			
		}
		
		UnderAttack = true;

		GetWorldTimerManager().SetTimer(HideTimerHandle, this, &ACannonEnemy::StopHiding, HidingTime, false);
	}
	
}

void ACannonEnemy::Death()
{
	Super::Death();

	StopShootingCannonWeapon(); 
}

void ACannonEnemy::StopHiding() 
{
	UnderAttack = false;
	
	if (ACannonAIController* CannonAIController = GetController<ACannonAIController>())
	{
		CannonAIController->GetBlackboard()->SetValueAsBool(bb_keys::UnderAttack, false);
	}   
}

void ACannonEnemy::AllowAttack()
{
	CanAttack = true; 
}


