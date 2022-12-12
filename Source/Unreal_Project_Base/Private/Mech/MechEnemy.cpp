// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech/MechEnemy.h"
#include "MechGameStateBase.h"
#include "AI/blackboard_keys.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Mech/MechPlayer.h" 
#include "Unreal_Project_Base/MechGameModeBase.h"


AMechEnemy::AMechEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AimAssistArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ARROW FOR AIM ASSIST"));
	AimAssistArrow->SetupAttachment(MechBody);

	
	LeftTargetForAimAssist = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftAimAssist"));
	LeftTargetForAimAssist->SetupAttachment(AimAssistArrow);

	RightTargetForAimAssist = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightAimAssist"));    
	RightTargetForAimAssist->SetupAttachment(AimAssistArrow);

	AimAssistCenterTarget = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CenterAimAssist"));
	AimAssistCenterTarget->SetupAttachment(MechBody);  
}



void AMechEnemy::BeginPlay()
{

	Super::BeginPlay();

	// Assign team enemy
	Team = TEAMS::ENEMY; 
	
	// Some Enemies can know about player location from start 
	if (PlayerDetected)
	{
		if (this) 
			DetectPlayer(); 
	}

	// Get Current Number of Alive Enemies
	AMechGameStateBase* MechGameState =  GetWorld()->GetGameState<AMechGameStateBase>();
	if (MechGameState)
	{
		MechGameState->IncrementAliveEnemies(); 
	}

	// Change between Targets Continuously
	GetWorldTimerManager().SetTimer(AttackTargetChangeHandle, this, &AMechEnemy::ChangeAttackTarget, TimeBetweenAttackTargetChange, true);

	YawRotation = 0;

	// Get Player Location
	UWorld* const World = GetWorld();
	if (!World) return;

	if ( !World->GetFirstPlayerController())  return; 
	if ( !World->GetFirstPlayerController()->GetPawn())  return;
	
	PlayerMech = World->GetFirstPlayerController()->GetPawn();
}

void AMechEnemy::Tick(float DeltaSeconds) 
{
	if (!Alive) return;
	 
	Super::Tick(DeltaSeconds);

	if (DisabledAI) return; 
	
	RotateCorrectly(DeltaSeconds);

	
	RotateAimAssistTowardPlayer(); 
	
}

void AMechEnemy::SetDamage(float Damage, FVector HitLocation, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (Health <= 0 ) return;
	
	Super::SetDamage(Damage, HitLocation, DamageType, DamageCauser);

	// Dash When attacked
	if (DashingWhenHit)
	{
		if ( UKismetMathLibrary::RandomBool())   DashLeft();
		
		else DashRight(); 
	}
	
	const AMechGameModeBase* MechMode =  GetWorld()->GetAuthGameMode<AMechGameModeBase>();

	if (!MechMode ) return;

	if (MechMode->OneShotOneKill) Death(); 
	
}

void AMechEnemy::RotateCorrectly(float DeltaSeconds)
{
}

void AMechEnemy::Death()
{

	Alive = false; 
	// Fire Enemy death Event to Blueprints
	OnEnemyDeath();

	StopShootingLeftArmGun();
	StopShootingRightArmGun();
	StopShootingCannonWeapon(); 
	
	// Turn Off AI
	if (AEnemyAIController* EnemyAIController = GetController<AEnemyAIController>())
	{
		EnemyAIController->Destroy();  
	}

	AMechGameModeBase* MechMode =  GetWorld()->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode ) return;

	// Increase Counter in Game Mode
	MechMode->EnemyKill();


	// Update GameState Alive enemies
	AMechGameStateBase* MechGameState =  GetWorld()->GetGameState<AMechGameStateBase>();
	if (MechGameState)
	{
		MechGameState->DecrementAliveEnemies();

		// No event if wave is in progress
		if (MechGameState->WaveInProgress) return;
			
		if (MechGameState->GetNumberOfAliveEnemies() <=0)
		{
			// Not active Waves
			if (MechGameState->WaveNum == -1) return;

			MechGameState->WaveInProgress = true;
			MechGameState->WaveNum ++; 
			MechGameState->NewWave(MechGameState->WaveNum);   
		}
	}
}

void AMechEnemy::SwitchDirectionInCircle()
{

	if (!CanSwitch) return;

	// Turn 180 to go in different opposite in circle

	
	RightDirectionCircle = !RightDirectionCircle;
	
	FRotator Rot = GetActorRotation(); 
	Rot.Yaw +=180;
	
	SetActorRotation(Rot);

	CanSwitch = false;
	
	GetWorldTimerManager().SetTimer(SwitchDirectionTimerHandle, this, &AMechEnemy::AllowSwitch, TimeBetweenSwitches, false); 
}

void AMechEnemy::AllowSwitch()
{
	CanSwitch = true;
}


void AMechEnemy::ChangeAttackTarget()
{
	// Shoot At Random Limb
	MACHINEGUN_ATTACK_TARGETS Target = static_cast<MACHINEGUN_ATTACK_TARGETS>(rand() % 5);

	AttackTarget = Target; 
}


void AMechEnemy::StopOutline()
{
	GetWorldTimerManager().SetTimer(OutlineStopHandle, this, &AMechEnemy::RenderCustomDepthOff, TimeToStopOutline, false);
}

void AMechEnemy::RenderCustomDepthOff()
{
	if (Alive)
	GetMesh()->SetRenderCustomDepth(false);   
}

void AMechEnemy::DetectPlayer()
{

	if (!this) return;  
	const AEnemyAIController* EnemyAIController =  GetController<AEnemyAIController>();

	if (EnemyAIController)
	EnemyAIController->GetBlackboard()->SetValueAsBool(bb_keys::PlayerDetected, true);   
}

void  AMechEnemy::RotateAimAssistTowardPlayer()
{

	FVector const Loc = GetActorLocation();


	// Get Player Location
	UWorld* const World = GetWorld();
	if (!World) return;

	if ( !World->GetFirstPlayerController())  return; 
	if ( !World->GetFirstPlayerController()->GetPawn())  return;  
	FVector const PlayerLoc =  World->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	// Find Vector Toward Player
	FRotator Rot = (PlayerLoc - Loc).Rotation();
	
	float const MechYaw =  GetActorRotation().Yaw;

	// Rotate Arrow Relatively to Mech Body 
	Rot.Yaw = Rot.Yaw - MechYaw + 90;   
	AimAssistArrow->SetRelativeRotation(Rot); 
}