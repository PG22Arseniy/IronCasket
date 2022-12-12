// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech/MinigunEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/blackboard_keys.h"
#include "AI/EnemyAIController.h"
#include "AI/AIControllers/MinigunAIController.h"
#include "Mech/MechController.h"
#include "Mech/MechPlayer.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeponTypes/MachineGun.h"

AMinigunEnemy::AMinigunEnemy()
{
	// Weapons Setup
	MechBody = GetMesh();

	
	// ALL Weapon Actors Set up
	LeftArmGun = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("MACHINE GUN LEFT"), MechBody, TEXT("LeftForeArm1"));   
	RightArmGun = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("MACHINE GUN RIGHT"), MechBody, TEXT("RightForeArm1"));   

}

void AMinigunEnemy::BeginPlay()
{
	Super::BeginPlay();


	AIController = GetController<AMinigunAIController>();

	GetWorldTimerManager().SetTimer(AttackTypeChangeHandle, this , &AMinigunEnemy::AttackChange, TimeBetweenAttackTypeChange, false);
	 
	
	// Get Reference to guns
	
	if (LeftArmGun) 
	{
		AMachineGun* Weapon = Cast<AMachineGun>(LeftArmGun->GetChildActor());

		if (!Weapon) return;
		
		if (LeftArmWeaponData) return;

		Weapon->WeaponData = LeftArmWeaponData;

	}

	if (RightArmGun) 
	{
		AMachineGun* Weapon = Cast<AMachineGun>(RightArmGun->GetChildActor());

		if (!Weapon) return;
		
		if (RightArmWeaponData) return;

		Weapon->WeaponData = RightArmWeaponData;

	}
	
}

void AMinigunEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AMinigunEnemy::AttackChange()
{

	if (AttackType == MACHINEGUN_ATTACK_TYPES::NONE)
	{
		MACHINEGUN_ATTACK_TYPES Type = static_cast<MACHINEGUN_ATTACK_TYPES>(rand() % 3);
		AttackType = Type;
		
		TimeBetweenAttackTypeChange = FMath::RandRange(MinRandomTimeOfAttacking, MaxRandomTimeOfAttacking);
	}
	else
	{
		AttackType = MACHINEGUN_ATTACK_TYPES::NONE;
		TimeBetweenAttackTypeChange = FMath::RandRange(MinRandomTimeOfNotAttacking, MaxRandomTimeOfNotAttacking); 
	}
	if (!GetWorld()) return;
	if (GetWorldTimerManager().TimerExists(AttackTypeChangeHandle))
		GetWorldTimerManager().ClearTimer(AttackTypeChangeHandle); 
	GetWorldTimerManager().SetTimer(AttackTypeChangeHandle, this , &AMinigunEnemy::AttackChange, TimeBetweenAttackTypeChange, false);
}

void AMinigunEnemy::AttackPlayer()
{
	if (AttackType == MACHINEGUN_ATTACK_TYPES::BOTH_ATTACK)
	{
		ShootLeftArmGun();
		ShootRightArmGun();
	}
	if (AttackType == MACHINEGUN_ATTACK_TYPES::LEFT_ATTACK)
	{
		ShootLeftArmGun();
		StopShootingRightArmGun();  
	}
	if (AttackType == MACHINEGUN_ATTACK_TYPES::RIGHT_ATTACK)
	{
		ShootRightArmGun();
		StopShootingLeftArmGun();
	}
	if (AttackType == MACHINEGUN_ATTACK_TYPES::NONE)
	{
		StopShootingLeftArmGun();
		StopShootingRightArmGun();
	}
	
}

void AMinigunEnemy::RotateCorrectly(float DeltaSeconds)
{

	if (DisabledAI) return; 

	
	if (!AIController)
	{
		AIController = GetController<AMinigunAIController>();
		return; 
	} 
	
	// Deal With Enemy Rotation 
	
	if (AIController->PlayerInSight)
	{

		if (CanAttack)
			AttackPlayer();

		if (!SawBefore) 
		{
			GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AMinigunEnemy::AllowAttack, TimeToAttack, false);
		}


		// Current Location, Rotation 
		FRotator Rot = GetActorRotation();
		const FVector Loc = GetActorLocation(); 

		// Player Loc
		FVector const PlayerLoc =  PlayerMech->GetActorLocation();
		
		// Rotate Upper body
		

		const FRotator DirectionToRotate = (PlayerLoc - Loc).Rotation(); 

		const float DesiredYawRot = DirectionToRotate.Yaw;

		//const float DeltaYawRot = FMath::Lerp(YawRotation, DesiredYawRot, DeltaSeconds); 

		YawRotation = DesiredYawRot - Rot.Yaw;  
		
	
	}
	else
	{
		CanAttack = false;
	}

	SawBefore = AIController->PlayerInSight;
}

void AMinigunEnemy::SetWeaponTarget(AWeapon* Weapon)
{

	const AMechPlayer* MainPlayer =  Cast<AMechPlayer>(PlayerMech); 
	if (!MainPlayer) return;

	// Get Location Of Target

	FVector TargetLocation;
	
	switch (AttackTarget)
	{

		case MACHINEGUN_ATTACK_TARGETS::LEFT_ARM:
			TargetLocation = MainPlayer->LeftArm->GetComponentLocation();
			break;

		case MACHINEGUN_ATTACK_TARGETS::LEFT_LEG:
			TargetLocation = MainPlayer->LeftLeg->GetComponentLocation();
			break;

		case MACHINEGUN_ATTACK_TARGETS::RIGHT_ARM:
			TargetLocation = MainPlayer->RightArm->GetComponentLocation();
			break;

		case MACHINEGUN_ATTACK_TARGETS::RIGHT_LEG:
			TargetLocation = MainPlayer->RightLeg->GetComponentLocation();
			break;

		case MACHINEGUN_ATTACK_TARGETS::BODY_CENTER:
			TargetLocation = MainPlayer->GetActorLocation();
			break;
	
		default:
			break;
	}
	 
	
	// Assign Target For Weapon
	Weapon->TargetForBullet = TargetLocation;
	
} 

void AMinigunEnemy::Death()
{
	Super::Death();

	// Stop Shooting machine Guns 
	StopShootingLeftArmGun();
	StopShootingRightArmGun();  
}

void AMinigunEnemy::AllowAttack()
{
	CanAttack = true;  
}
 



