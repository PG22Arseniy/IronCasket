// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech/MechPlayer.h"
#include "DrawDebugHelpers.h"
 
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Debug/DebugSpawnPoint.h"
#include "Debug/LevelManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Unreal_Project_Base/MechGameModeBase.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeaponData.h"



AMechPlayer::AMechPlayer()
{
	
	PrimaryActorTick.bCanEverTick = true; 
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING ARM"));
	SpringArm->SetupAttachment(MechBody, "Head1");     

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));   
	Camera->SetupAttachment(SpringArm);

	DebugCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DEBUG_CAMERA"));    
	DebugCamera->SetupAttachment(MechBody);  

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DIRECTION"));
	Arrow->SetupAttachment(Camera);

	FirstCameraCollider = CreateDefaultSubobject<USphereComponent>(TEXT("CAMERA COLLIDER 1"));
	FirstCameraCollider->SetupAttachment(MechBody, "Head1");
	FirstCameraCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	FirstCameraCollider->SetGenerateOverlapEvents(true);

	SecondCameraCollider = CreateDefaultSubobject<USphereComponent>(TEXT("CAMERA COLLIDER 2"));
	SecondCameraCollider->SetupAttachment(MechBody, "Head1");
	SecondCameraCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SecondCameraCollider->SetGenerateOverlapEvents(true);

	UpperCameraCollider = CreateDefaultSubobject<USphereComponent>(TEXT("CAMERA COLLIDER UP")); 
	UpperCameraCollider->SetupAttachment(MechBody, "Head1");
	UpperCameraCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	UpperCameraCollider->SetGenerateOverlapEvents(true);
	
	
	
	// Weapons Setup
	MechBody = GetMesh();  

	
	// ALL Weapon Actors Set up
	LeftArmGun = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("ROCKETLAUNCHER LEFT"), MechBody, TEXT("LeftForeArm1"));  
	RightArmGun = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("MACHINE GUN RIGHT"), MechBody, TEXT("RightForeArm1"));  
	CannonWeapon = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("CANNON WEAPON"), MechBody, TEXT("RightShoulder_Gun"));        

	ZoomInTimeLineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomInTimelineComponent")); 
	ZoomOutTimeLineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomOutTimelineComponent"));

	CameraPositionStateChangeTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraPositionStateChangeTimelineComponent"));
	AimAssistTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimAssistTimelineComponent")); 
	
	SetUpStimulus();
}

void AMechPlayer::DisablePlayerInput()
{
	GameplayInputDisabled = false; 
}

void AMechPlayer::Tick(float DeltaSeconds)
{ 
	Super::Tick(DeltaSeconds);
	
	SetReticleTarget();

	if (DebugCameraLocked)
	{
		DebugCamera->SetWorldLocation(LockedPositionOfCamera);
		DebugCamera->SetWorldRotation(LockedRotationOfCamera);   
	}

	if (!AimAssistTimelineComp->IsPlaying())  
	{
		MouseDampeningRate =  FMath::Lerp(MouseDampeningRate, 1.0f, DeltaSeconds * AimAssistDampeningRate);  
	}

	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaSeconds, FColor::Blue, FText::AsNumber(CannonWeaponData->CurrentReloadTimeValue).ToString());    
	
}

void AMechPlayer::BeginPlay()
{
	Super::BeginPlay();

	Team = TEAMS::PLAYER;

	FirstCameraCollider->OnComponentBeginOverlap.AddDynamic(this, &AMechPlayer::FirstColliderOnOverlapBegin); 
	SecondCameraCollider->OnComponentBeginOverlap.AddDynamic(this, &AMechPlayer::SecondColliderOnOverlapBegin);
	UpperCameraCollider->OnComponentBeginOverlap.AddDynamic(this, &AMechPlayer::UpperColliderOnOverlapBegin); 

	FirstCameraCollider->OnComponentEndOverlap.AddDynamic(this, &AMechPlayer::FirstColliderOnOverlapEnd);
	SecondCameraCollider->OnComponentEndOverlap.AddDynamic(this, &AMechPlayer::SecondColliderOnOverlapEnd); 
	UpperCameraCollider->OnComponentEndOverlap.AddDynamic(this, &AMechPlayer::UpperColliderOnOverlapEnd); 

	// Default Rotation from start
	YawRotation = -60.0f;

	// Default Max Speed
	DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed; 

	// Set Camera Default and Desired (for Zoom) positions
	CameraDefaultPosition = {SpringArm->TargetArmLength, SpringArm->SocketOffset.Y, SpringArm->SocketOffset.Z}; 
	CameraZoomedInPosition = {DesiredArmLength, DesiredSocketOffsetY, DesiredSocketOffsetZ}; 
	
	
	// Default Camera Distance 
	DefaultArmLength = SpringArm->TargetArmLength;

	DesiredSpringArmOffsetZ = SpringArm->SocketOffset.Z;

	DefaultSpringArmOffsetY = SpringArm->SocketOffset.Y; 

	RightSideCameraOffset = SpringArm->SocketOffset.Z;

	LeftSideCameraOffset = -RightSideCameraOffset;  
	
	DefaultCameraRotation = Camera->GetRelativeRotation();   

	
	if (ZoomInCurveFloat && ZoomInTimeLineComp)
	{
		// Zoom in Timeline Setup
		ZoomInTimeLineFunc.BindUFunction(this, FName("ZoomInTimeLineProgress"));
		ZoomInTimeLineComp->AddInterpFloat(ZoomInCurveFloat, ZoomInTimeLineFunc, FName("ZoomIn"));
	
		ZoomInTimeLineComp->SetLooping(false);
		ZoomInTimeLineComp->SetIgnoreTimeDilation(true);


		ZoomInTimelineFinished.BindUFunction(this, FName("ZoomedInFunc")); 
		ZoomInTimeLineComp->SetTimelineFinishedFunc(ZoomInTimelineFinished);
	}

	if (ZoomOutCurveFloat && ZoomOutTimeLineComp) 
	{
		// Zoom out Timeline Setup
		ZoomOutTimeLineFunc.BindUFunction(this, FName("ZoomOutTimeLineProgress"));
		ZoomOutTimeLineComp->AddInterpFloat(ZoomOutCurveFloat, ZoomOutTimeLineFunc, FName("ZoomOut"));
	
		ZoomOutTimeLineComp->SetLooping(false);
		ZoomOutTimeLineComp->SetIgnoreTimeDilation(true);


		ZoomOutTimelineFinished.BindUFunction(this, FName("ZoomedOutFunc")); 
		ZoomOutTimeLineComp->SetTimelineFinishedFunc(ZoomOutTimelineFinished);
	}

	if (CameraPositionStateChangeTimelineComp && CameraPositionStateChangeCurveFloat)
	{
		CameraPositionStateChangeTimelineFunc.BindUFunction(this, FName("CameraPositionStateChangeTimelineProgress"));
		CameraPositionStateChangeTimelineComp->AddInterpFloat(CameraPositionStateChangeCurveFloat, CameraPositionStateChangeTimelineFunc, FName("Position Change Of Camera"));
	}

	if (AimAssistTimelineComp && AimAssistCurveFloat)
	{
		AimAssistTimelineFunc.BindUFunction(this, FName("AimAssistTimeLineProgress"));
		AimAssistTimelineComp->AddInterpFloat(AimAssistCurveFloat, AimAssistTimelineFunc, FName("AimAssist"));

		FOnTimelineEvent AimAssistFinished;
		AimAssistFinished.BindUFunction(this, FName("DeactivateAimAssist"));
		AimAssistTimelineComp->SetTimelineFinishedFunc(AimAssistFinished);   
	}
	
	PlayerReset(); 
}



// ROTATION AND MOVEMENT

void AMechPlayer::LookUp_Implementation(float AxisValue)
{

	if (AxisValue == 0 ) return;
	
	if (DebugCameraMode)
	{

		FRotator Rot = DebugCamera->GetComponentRotation();
		Rot.Pitch -= AxisValue;
		DebugCamera->SetWorldRotation(Rot);  
		
		return;
	}
	
	if (!GameplayInputDisabled) return;
	
	
	const float PitchRotation = GetControlRotation().Pitch;

	if (ZoomedIn)
	{
		if (PitchRotation > MaxZoomedInLookUpAngle && AxisValue < 0 && PitchRotation < 180) return;      
		if (PitchRotation < 360 - MaxZoomedInLookDownAngle && AxisValue > 0 && PitchRotation > 180) return;
	}
	else
	{
		// Cap on Look up and Down
		if (PitchRotation > MaxLookUpAngle && AxisValue < 0 && PitchRotation < 180) return;      
		if (PitchRotation < 360 - MaxLookDownAngle && AxisValue > 0 && PitchRotation > 180) return;
	}
	
	// Look Up and Down  
	AddControllerPitchInput(AxisValue* BaseVerticalRotateRate * MouseDampeningRate);   
	
	AdjustTargetOffset();
}

void AMechPlayer::Turn(float AxisValue)  
{
	
	if (DebugCameraMode)
	{
		// Only Move Debug Camera
		DebugCamera->SetWorldLocation(DebugCamera->GetComponentLocation() + DebugCamera->GetRightVector()*AxisValue*40);  

		return;
	}
	if (AxisValue == 0)
	{ 
		return;
	}

	if (!GameplayInputDisabled) return;

	// Mech Loses Accelerated Mode
	//MechDeaccelerate();  
	//GetWorldTimerManager().ClearTimer(AccelerateHandle); 

	// Can't Turn if Zoomed In and You Can't Always Move
	if (ZoomedIn && CanMoveCondition != CAN_MOVE_CONDITION::ALWAYS_CAN_MOVE) return;


	// Get Current Rotation And Desired Rotation for the next Frame
	const FRotator CurRotation = GetActorRotation();

	const float DeltaRotYaw = AxisValue * BaseTurnRate;
	
	const FRotator Rotation = {CurRotation.Pitch, CurRotation.Yaw + DeltaRotYaw ,CurRotation.Roll};


	// Split Body And Tank Movement Are different in Turning
	
	if (MovementType == MOVEMENT_TYPE::SPLITBODY)  
	{
		// Adjust Rotation Of Upper Body
		if ( YawRotation - DeltaRotYaw < MaxSideRotation   && YawRotation - DeltaRotYaw > - MaxSideRotation )     
			YawRotation -= DeltaRotYaw;  

	
		// Set Rotation Of the whole body
		SetActorRotation(Rotation);
	}
	
	else  // TANK MOVEMENTS
		
	{
		// Disable Lock on Side Rotation
		MaxSideRotation = -1;  

		const FRotator Rot = GetActorRotation();

		float DesiredYaw;

		// Desired Yaw follows the YawRotation (upper body)
		if (AxisValue > 0)
		{
			if (!GoesBackwards)
				DesiredYaw  = YawRotation  + Rot.Yaw + 90;
			else
				DesiredYaw  = YawRotation  + Rot.Yaw - 90;
			
			GoesRight = true;
		}
		else
		{
			if (!GoesBackwards)
				DesiredYaw  = YawRotation  + Rot.Yaw - 90;
			else
				DesiredYaw  = YawRotation  + Rot.Yaw + 90;
			
			GoesRight = false; 
		}
		
		// Lerp toward desired Rot
		const float DeltaTurn = FMath::Lerp(Rot.Yaw , DesiredYaw, TankMovementTurnSpeed ); 

		const float Dif = DeltaTurn - Rot.Yaw; 
		
		const FRotator DesiredRot = {Rot.Pitch, DeltaTurn, Rot.Roll};

		// Set Rotation Of the whole body
		SetActorRotation(DesiredRot);

		// Adjust Yaw Rotation based on rotation
		YawRotation -= Dif;

		// always move forward
		if (!GoesBackwards)
		AddMovementInput(GetActorForwardVector() * FMath::Abs(AxisValue)); 
	}

	
}

void AMechPlayer::Rotate_Implementation(float AxisValue)
{

	if (DebugCameraMode)
	{
		// ROTATE ONLY DEBUG CAMERA
		FRotator Rot = DebugCamera->GetComponentRotation();
		Rot.Yaw += AxisValue;
		DebugCamera->SetWorldRotation(Rot);
		
		return;
	}

	
	if (AxisValue == 0)
	{
		// No Mouse Movement
		YawRotationInput = false;  
		return;
	}
	
	if (!GameplayInputDisabled) return;
	
	YawRotationInput = true; 
	
	// Max Left and Right Rotation as constraints
	if (MaxSideRotation >= 0) 
	{
		if (YawRotation > (MaxSideRotation ) && AxisValue > 0) return;
	
		if (YawRotation < (-MaxSideRotation ) && AxisValue < 0) return;
	}


	// Rotate The Upper Body 
	YawRotation += AxisValue * BaseHorizontalRotateRate * MouseDampeningRate;

	// Yaw Rotation varies between -360 and 360
	if (FMath::Abs(YawRotation)>360) YawRotation = 0;  
} 


void AMechPlayer::MoveForward_Implementation(float AxisValue) 
{
	if (DebugCameraMode)
	{
		// Move only Debug Camera
		DebugCamera->SetWorldLocation(DebugCamera->GetComponentLocation() + DebugCamera->GetForwardVector()*AxisValue*40); 

		return;
	}
	if (ZoomedIn && CanMoveCondition != CAN_MOVE_CONDITION::ALWAYS_CAN_MOVE) return;

	if (AxisValue > 0)
	{
		if (!GetWorldTimerManager().TimerExists(AccelerateHandle))
		{
			GetWorldTimerManager().SetTimer(AccelerateHandle, this, &AMechPlayer::MechAccelerate, TimeToAccelerate, false);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(AccelerateHandle);
		MechDeaccelerate();   
	} 
	
	if (AxisValue==0)
	{
		// No Move Forward Input
		GoesBackwards = false;
		return;
	}

	if (!GameplayInputDisabled) return;
	

	// All Three Movement Types Are Different In Move Forward Implementation
	
	if (MovementType == MOVEMENT_TYPE::SPLITBODY)
	{
		GoesBackwards = false;
		AddMovementInput(GetActorForwardVector() * AxisValue);
	}

	if (MovementType == MOVEMENT_TYPE::TANK_ONLYFORWARD)
	{
		GoesBackwards = false;
		
		// Disable Lock on Side Rotation
		MaxSideRotation = -1;   

		// Current Rotation
		const FRotator Rot = GetActorRotation();

		// Calculate Desired Yaw
		float DesiredYaw;
		
		if (AxisValue > 0)
			DesiredYaw  = YawRotation  + Rot.Yaw;
		else
		{
			if (GoesRight)
				DesiredYaw  = YawRotation + Rot.Yaw + 180; 
			else
				DesiredYaw  = YawRotation + Rot.Yaw - 180;  
		}

		// Lerp Yaw Rotation towards DesiredYaw 
		const float DeltaTurn = FMath::Lerp(Rot.Yaw , DesiredYaw, TankMovementTurnSpeed );  
		
		const FRotator DesiredRot = {Rot.Pitch, DeltaTurn, Rot.Roll};
		
		SetActorRotation(DesiredRot);

		// Yaw Rotation has to be always Offset By the Difference from DeltaTurn Yaw and Current Yaw so that Upper Body Rotates along With Legs
		const float Dif = DeltaTurn - Rot.Yaw;
		
		YawRotation -= Dif;

		// Always Forward, that's why Absolute Value
		AddMovementInput(GetActorForwardVector() * FMath::Abs(AxisValue)); 
		
	}

	if (MovementType == MOVEMENT_TYPE::TANK_WITHBACKWARDS)
	{
		// Check If Backward Movement
		if (AxisValue > 0)
			GoesBackwards = false;
		else
			GoesBackwards = true;
		
		// Disable Lock on Side Rotation
		MaxSideRotation = -1;  

		// Get Current And Desired Rot
		const FRotator Rot = GetActorRotation();
		
		const float DesiredYaw  = YawRotation + Rot.Yaw;
		
		const float DeltaTurn = FMath::Lerp(Rot.Yaw , DesiredYaw, TankMovementTurnSpeed );  
		
		const FRotator DesiredRot = {Rot.Pitch, DeltaTurn, Rot.Roll};
		
		SetActorRotation(DesiredRot); 

		// Offset Yaw Rotation For UpperBody
		const float Dif = DeltaTurn - Rot.Yaw; 
		YawRotation -= Dif;

		// Move Mech
		AddMovementInput(GetActorForwardVector() * AxisValue);  
	}

}

void AMechPlayer::AdjustTargetOffset()
{
	const float PitchRotation = GetControlRotation().Pitch;

	// Ignore adjusting when in zoomed in state
	if (ZoomedIn && CanMoveCondition != CAN_MOVE_CONDITION::ALWAYS_CAN_MOVE) return;  
	
	// Adjust Camera distance based on Look Up Rotation
	if (PitchRotation < 180) 
	{
		SpringArm->TargetArmLength = DefaultArmLength - PitchRotation * CameraOffsetRateLookingDown ;  
	}

	if (PitchRotation > 180)
	{
		const float OffsetArmLength = 360 - PitchRotation;

		SpringArm->TargetArmLength = DefaultArmLength + OffsetArmLength * CameraOffsetRateLookingUp ;  
	}

	// Current Camera position is the default 
	CameraDefaultPosition.X = SpringArm->TargetArmLength;


	// Get Distance between camera and player
	const FVector CurrentCameraLoc = Camera->GetComponentLocation();

	const float Dist = FVector::Dist(CurrentCameraLoc, GetActorLocation());
	

	// Toggle visibility of player based on position of camera
	if (Dist < CameraDistanceToHidePlayer && PitchRotation > 0 && !BodyHidden) 
	{
		BodyHidden = true; 
		UpdateMechOpacity(0.0f); 
	}
	else if (Dist > CameraDistanceToUnHidePlayer && BodyHidden)    
	{
		BodyHidden = false;
		UpdateMechOpacity(1.0f);
	}
}



// DEBUG FUNCTIONS:

void AMechPlayer::DeactivateAimAssist()
{
	AimAssistActive = false;
}

void AMechPlayer::AllowAssist()
{
	CanAssist = true;
}

void AMechPlayer::Suicide()
{
	Death(); 
}

void AMechPlayer::SpawnEnemy()
{
	if (!EnemyMechPrefab) return;  

	FTransform SpawnTransfom = GetTransform();
	FActorSpawnParameters SpawnParameters;
	
	GetWorld()->SpawnActor<AMechEnemy>(EnemyMechPrefab, SpawnTransfom, SpawnParameters);  
}

void AMechPlayer::NextSpawnPoint()
{

	// Get Level Manager from the Level
	ALevelManager* LevelManager = Cast<ALevelManager>(UGameplayStatics::GetActorOfClass(GetWorld(),ALevelManager::StaticClass()));

	if (!LevelManager) return;
	
	SpawnPoints = LevelManager->DebugSpawnPoints;  

	// Check For Out Of range Exceptions
	if (SpawnPoints.Num() == 0) return;
	if (!SpawnPoints[CurrentSpawnPoint]) return;


	// Change Player Location to DebugSpawnPoint
	FVector Loc = SpawnPoints[CurrentSpawnPoint]->GetActorLocation();
	SetActorLocation({Loc.X, Loc.Y, Loc.Z + 1000});
	

	// Change To Next Level or Previous Level
	if (SpawnPoints[LastSpawnPoint]->Level < SpawnPoints[CurrentSpawnPoint]->Level )
	{
		GetWorld()->GetGameState<AMechGameStateBase>()->NextLevel();
	}
	
	if (SpawnPoints[LastSpawnPoint]->Level > SpawnPoints[CurrentSpawnPoint]->Level)
	{
		GetWorld()->GetGameState<AMechGameStateBase>()->PreviousLevel(); 
	}

	// Change Current Spawn Point
	LastSpawnPoint = CurrentSpawnPoint;
		
	if (SpawnPoints.Num() - 1 > CurrentSpawnPoint)
		CurrentSpawnPoint ++;
	else
	{
		CurrentSpawnPoint = 0;
	} 
	
}

void AMechPlayer::PreviousSpawnPoint()
{
	if (SpawnPoints.Num() == 0) return;
	
	if (0 < CurrentSpawnPoint) 
		CurrentSpawnPoint --;
	else
	{
		CurrentSpawnPoint = SpawnPoints.Num() - 1;
	}

	FVector Loc = SpawnPoints[CurrentSpawnPoint]->GetActorLocation(); 

	
	SetActorLocation({Loc.X, Loc.Y, Loc.Z + 1000}); 
}

void AMechPlayer::KillAllEnemies()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMechEnemy::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		if (AMechEnemy* Enemy = Cast<AMechEnemy>(FoundActor))
		{
			Enemy->Death(); 
		}
	}
}

void AMechPlayer::RestoreHealth()
{
	Health = MaxHealth;
}

void AMechPlayer::LockDebugCamera()
{
	DebugCameraLocked = !DebugCameraLocked;

	if (DebugCameraLocked) 
	{
		LockedPositionOfCamera = DebugCamera->GetComponentLocation();
		LockedRotationOfCamera = DebugCamera->GetComponentRotation();
	}
}


// SHOOTING FUNCTIONS


// Set Firing Target 
void AMechPlayer::SetWeaponTarget(AWeapon* Weapon)
{
	
	// Get Inaccuracy Value Horizontally and Vertically

	if (!Weapon->WeaponData) return;
	
	float xInaccuracy = FMath::RandRange(Weapon->WeaponData->MinHorizontalInaccuracy,  Weapon->WeaponData->MaxHorizontalInaccuracy);
	float zInaccuracy = FMath::RandRange(Weapon->WeaponData->MinVerticalInaccuracy,  Weapon->WeaponData->MaxVerticalInaccuracy); 


	// Get Random Direction for inaccuracy
	if (FMath::RandBool()) xInaccuracy *= -1;
	if (FMath::RandBool()) zInaccuracy *= -1;

	// Adjust Endpoint with inaccuracy
	End = {End.X + xInaccuracy, End.Y, End.Z + zInaccuracy};

	Weapon->TargetForBullet = End;
	
}

void AMechPlayer::SetReticleTarget()
{
	// Get Target Position for Weapons:

	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	// Get Transform of Center Screen Arrow 
	Loc = Arrow->GetComponentLocation(); 
	Rot = Arrow->GetComponentRotation(); 
	
	FVector Start = Loc;
	End = Start + (Rot.Vector() * MaxTargetDistance);  
	
	
	FCollisionQueryParams TraceParams;

	TraceParams.AddIgnoredActor(this); 
	
	// SET TARGET FOR WEAPON: 

	// Trace for target from center screen 
	bool HitDone = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Camera, TraceParams); 
	
	if (HitDone)
	{
		End = Hit.Location;
		
	}
	
	ReticleHit = Hit; 


	// Reticle Color Swap Event
	if (Hit.Actor != PreviousActorHit) 
	{
		PreviousActorHit = Cast<AActor>(Hit.Actor);  

		ReticleColorSwap(Hit, HitDone); 
	}

	// AIM ASSIST

	if (AimAssistType == AIM_ASSIST_TYPE::TWO_COLLIDERS_ON_SIDES)
	{
		// Get The Capsule that we Aim At
		UCapsuleComponent* AimAssistComp = Cast<UCapsuleComponent> (Hit.GetComponent());
		
		if (!AimAssistComp) // If we don't aim at Capsule Deactivate Aim Assist
		{
			if (!AimAssistActive) return;    
			AimAssistActive = false;
			AimAssistTimelineComp->Stop(); 
			return;
		}

		// Allow Assist At Certain Rate
		if (!CanAssist) return;
		CanAssist = false;
		GetWorldTimerManager().SetTimer(AllowAssistHandle, this, &AMechPlayer::AllowAssist, AllowAssistRate, false);   


		// If We Hit Hit Left Aim Assist AxisValue = -1 so that UpperBody Rotates To the right
		
		if (!AimAssistActive) 
		{
			if (AimAssistComp->GetName() == "LeftAimAssist") 
			{
				AimAxisValue = -1;
				
			}
			else
			if (AimAssistComp->GetName() == "RightAimAssist") 
			{
				AimAxisValue = 1;
			}
			else
			{
				AimAxisValue = 0; 
				return;
			}

			// Get The Desired YawRotation and Lerp Towards it with timeline
			AimDesiredYaw = YawRotation + AimAssistOffset*AimAxisValue;   
			AimAssistTimelineComp->PlayFromStart(); 
			AimAssistActive = true;
			
	
		}
		
	}

	if (AimAssistType == AIM_ASSIST_TYPE::MOUSE_SENSITIVITY_DAMPENING)
	{
		UCapsuleComponent* AimAssistComp = Cast<UCapsuleComponent> (Hit.GetComponent());
		if (!AimAssistComp)
		{
			if (!AimAssistActive) return;    
			AimAssistActive = false;
			AimAssistTimelineComp->Stop(); 
			return;
		}

		// AimAssist is allowed at certain rate
		
		if (!CanAssist) return;
		CanAssist = false;
		GetWorldTimerManager().SetTimer(AllowAssistHandle, this, &AMechPlayer::AllowAssist, AllowAssistRate, false);   
	
		if (!AimAssistActive) 
		{
			// Aim assist is active and it dampens Mouse Sensitivity on timeline
			if (AimAssistComp->GetName() == "CenterAimAssist")
			{
				AimAssistTimelineComp->PlayFromStart(); 
				AimAssistActive = true;
			}
	
		}
		
	}
}


void AMechPlayer::Reload() 
{
	if (AWeapon* RightWeapon = Cast<AWeapon>(RightArmGun->GetChildActor()))
	{
		RightWeapon->Reload();
	}

	if (AWeapon* LeftWeapon = Cast<AWeapon>(LeftArmGun->GetChildActor()))
	{
		LeftWeapon->Reload(); 
	}
}

void AMechPlayer::ShootCannonWeapon()
{

	if (!GameplayInputDisabled) return;
	
	// Stop Aim Assist
	MouseDampeningRate = 1;
	AimAssistTimelineComp->Stop();
	
	Super::ShootCannonWeapon();

	// Take Target and Zoom in
	if (AWeapon* MyWeapon = Cast<AWeapon>(CannonWeapon->GetChildActor())) 
	{

		FocusedOnTarget = true;
		ZoomedIn = true; 
		
		SetWeaponTarget(MyWeapon);
		CannonTarget = MyWeapon->TargetForBullet; 
	}
	
	ZoomOutTimeLineComp->Stop();
	if (ZoomInTimeLineComp->IsPlaying()) return;
	ZoomInTimeLineComp->PlayFromStart();
}


void AMechPlayer::ShootRightArmGun() 
{

	if (!GameplayInputDisabled) return;

	MouseDampeningRate = 1;
	AimAssistTimelineComp->Stop();
	
	if (ZoomedIn) return; 
	
	if (AMechEnemy* Enemy = Cast<AMechEnemy>(ReticleHit.Actor))
	{
		TargetActor = Enemy;
	}

	else 
	{
		TargetActor = nullptr; 
	}
	
	Super::ShootRightArmGun();
	
}

void AMechPlayer::ShootLeftArmGun()
{

	if (!GameplayInputDisabled) return;

	MouseDampeningRate = 1;
	AimAssistTimelineComp->Stop();
	 
	if (CanFireCannon && OnLeftClickCannonShot) 
	{
		FireCannon();
		return; 
	}
	
	if (ZoomedIn) return;
	Super::ShootLeftArmGun();

	
}


void AMechPlayer::FinishFiringCannon()
{
	Super::FinishFiringCannon();

	if (CanMoveCondition == CAN_MOVE_CONDITION::ON_CANNON_ANIMATION_FINISH) ZoomedIn = false; 
	
}

void AMechPlayer::StopShootingCannonWeapon()
{
	Super::StopShootingCannonWeapon(); 

	CanFireCannon = false;

	ZoomInTimeLineComp->Stop(); 
	if (ZoomOutTimeLineComp->IsPlaying()) return; 
	ZoomOutTimeLineComp->PlayFromStart();

	if (CanMoveCondition == CAN_MOVE_CONDITION::ON_ZOOMOUT_TIMELINE_START) ZoomedIn = false;
}


// Player Death
void AMechPlayer::Death()
{ 
	 
	UWorld * World = GetWorld();
	if (!World) return;

	AMechGameModeBase* MechMode = World->GetAuthGameMode<AMechGameModeBase>();
	if (!MechMode) return;


	PlayerDeath();
	
	MechMode->GameOver(false);    
}

void AMechPlayer::AmmoRestore()
{
	// Get Reference to guns and Restore Ammo
	
	if (LeftArmGun) 
	{
		AWeapon* Weapon = Cast<AWeapon>(LeftArmGun->GetChildActor()); 

		if (!Weapon) return;
		
		if (!LeftArmWeaponData) return;

		Weapon->WeaponData = LeftArmWeaponData;

		Weapon->WeaponData->CurrentAmmo = Weapon->WeaponData->Magazine; 

	}

	if (RightArmGun) 
	{
		AWeapon* Weapon = Cast<AWeapon>(RightArmGun->GetChildActor());

		if (!Weapon) return;
		if (!Weapon) return;
		
		if (!RightArmWeaponData) return;

		Weapon->WeaponData = RightArmWeaponData;

		Weapon->WeaponData->CurrentAmmo = Weapon->WeaponData->Magazine; 

	}

	if (CannonWeapon)
	{
		AWeapon* Weapon = Cast<AWeapon>(CannonWeapon->GetChildActor());

		if (!Weapon) return;
		
		if (!CannonWeaponData) return; 

		Weapon->WeaponData = CannonWeaponData;

		Weapon->WeaponData->CurrentAmmo = Weapon->WeaponData->Magazine; 

	}
}

void AMechPlayer::FirstColliderOnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	FirstColliderOverlapping = true;
	
	CameraChangeState(); 
}

void AMechPlayer::SecondColliderOnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	 SecondColliderOverlapping = true;
	
	CameraChangeState();
}

void AMechPlayer::UpperColliderOnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UpperColliderOverlapping = true;
	
	CameraChangeState();
}

void AMechPlayer::UpperColliderOnOverlapEnd(UPrimitiveComponent* newComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UpperColliderOverlapping = false;
	
	CameraChangeState(); 
}

void AMechPlayer::FirstColliderOnOverlapEnd(UPrimitiveComponent* newComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	FirstColliderOverlapping = false;

	CameraChangeState();
}

void AMechPlayer::SecondColliderOnOverlapEnd(UPrimitiveComponent* newComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	SecondColliderOverlapping = false;  

	CameraChangeState();
}

void AMechPlayer::CameraChangeState()
{
	// If Both colliders overlap then Camera in center
	if (FirstColliderOverlapping && SecondColliderOverlapping)
	{
		DesiredSpringArmOffsetZ = CenterCameraOffset ;
		DesiredSpringArmOffsetY = SpringArmOffsetYInCenter;
	}
	
	
	// If only SECOND  collider DOESNT overlap then Camera in Left Side
	if (FirstColliderOverlapping && !SecondColliderOverlapping && UpperColliderOverlapping)
	{
		DesiredSpringArmOffsetZ = LeftSideCameraOffset;
		DesiredSpringArmOffsetY = DefaultSpringArmOffsetY;
	}
	

	if (FirstColliderOverlapping  && !UpperColliderOverlapping)
	{
		DesiredSpringArmOffsetZ = CenterCameraOffset ;
		DesiredSpringArmOffsetY = SpringArmOffsetYInCenter; 
	}


	if (!FirstColliderOverlapping )	
		// Any other case camera goes to default right side
	{
		DesiredSpringArmOffsetZ = RightSideCameraOffset; 
		DesiredSpringArmOffsetY = DefaultSpringArmOffsetY;


    }
	
	if (SpringArm->SocketOffset.Z== DesiredSpringArmOffsetZ) return;

	
	// Change Offset of Spring arm with timeline
	CameraPositionStateChangeTimelineComp->PlayFromStart();  

	
 }

void AMechPlayer::CameraPositionStateChangeTimelineProgress(float Value) 
{
	SpringArm->SocketOffset.Z = FMath::Lerp(SpringArm->SocketOffset.Z, DesiredSpringArmOffsetZ,  CameraDampeningRate * Value); 
	SpringArm->SocketOffset.Y = FMath::Lerp(SpringArm->SocketOffset.Y, DesiredSpringArmOffsetY,CameraDampeningRate * Value );   
}

void AMechPlayer::MechAccelerate()
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxSpeed;
}

void AMechPlayer::MechDeaccelerate()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
}

void AMechPlayer::AimAssistTimelineProgress(float Value)
{
	if (AimAssistType == AIM_ASSIST_TYPE::TWO_COLLIDERS_ON_SIDES)
	{
		// Rotate Player in Yaw towards Target 
		YawRotation = FMath::Lerp(YawRotation, AimDesiredYaw, Value * AimAssistDampeningRate);
	}

	if (AimAssistType == AIM_ASSIST_TYPE::MOUSE_SENSITIVITY_DAMPENING)
	{
		// Dampen Mouse Movement 	
		MouseDampeningRate = FMath::Lerp(MouseDampeningRate, MouseDampenedRate, Value * AimAssistDampeningRate);  
	}
}

void AMechPlayer::PlayerReset()
{
	 Alive = true; 
	 RestoreHealth();
	 AmmoRestore(); 
}

void AMechPlayer::AllowCannonFire()
{
	CanFireCannon = true;

	if (!OnLeftClickCannonShot)
	{
		FireCannon(); 
	}
}


// Perception Stimulus For AI
void AMechPlayer::SetUpStimulus() 
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PERCEPTION STIMULUS"));

	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem(); 
}



void AMechPlayer::AddHealth(float HealthToAdd)
{
	if (Health + HealthToAdd >= MaxHealth)
	{
		Health = MaxHealth;
		return;
	}

	if (Health + HealthToAdd <= 0)
	{
		Health = 0;
		Death();
		return; 
	}
	

	Health += HealthToAdd;  
}

FRotator AMechPlayer::GetUpperBodyRotation()
{
	FRotator Rot = GetActorRotation();

	Rot.Yaw += YawRotation;  
	
	return (Rot); 
}

void AMechPlayer::FireCannon()
{
	Super::FireCannon();
	
	if (ZoomOutAfterShot)
	{
		ZoomInTimeLineComp->PlayFromStart();
	}
}

void AMechPlayer::ZoomInTimeLineProgress(float Value)
{
	// Camera Position change
	
	const FVector CurrentCameraPosition = {SpringArm->TargetArmLength, SpringArm->SocketOffset.Y , SpringArm->SocketOffset.Z};
	
	const FVector NewCameraDistance = FMath::Lerp(CurrentCameraPosition, CameraZoomedInPosition, Value); 
	
	SpringArm->TargetArmLength = NewCameraDistance.X;
	SpringArm->SocketOffset.Y = NewCameraDistance.Y;
	SpringArm->SocketOffset.Z = NewCameraDistance.Z; 


	// Camera Rotation change
	
	const FVector CameraLoc = Camera->GetComponentLocation();
	const FRotator DesiredRot = (CannonTarget - CameraLoc).Rotation();
		
	// Stop The Focus if the mouse is moved
	if (YawRotationInput) FocusedOnTarget = false;
	
	if (FocusedOnTarget)  
		Camera->SetWorldRotation(DesiredRot);     
	
}

void AMechPlayer::ZoomOutTimeLineProgress(float Value)
{
	// Camera Position change
	
	const FVector CurrentCameraPosition = {SpringArm->TargetArmLength, SpringArm->SocketOffset.Y , SpringArm->SocketOffset.Z};
	
	const FVector NewCameraDistance = FMath::Lerp(CurrentCameraPosition,CameraDefaultPosition,  Value);  

	
	SpringArm->TargetArmLength = NewCameraDistance.X;
	SpringArm->SocketOffset.Y = NewCameraDistance.Y;  
	SpringArm->SocketOffset.Z = NewCameraDistance.Z;


	// Camera Rotation change
	
	const FRotator CameraRot = Camera->GetRelativeRotation();
	
	const FRotator CameraDeltaRot = FMath::Lerp(CameraRot, DefaultCameraRotation, Value);
	
	Camera->SetRelativeRotation(CameraDeltaRot); 
}


void AMechPlayer::ZoomedInFunc()
{
	ZoomedIn = true; 
}

void AMechPlayer::ZoomedOutFunc()
{
	if (CanMoveCondition == CAN_MOVE_CONDITION::ON_ZOOMOUT_TIMILNE_FINISH)
		ZoomedIn = false;
}


// INPUT SYSTEM
void AMechPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMechPlayer::MoveForward);
	PlayerInputComponent->BindAxis("LookUp", this,  &AMechPlayer::LookUp);
	PlayerInputComponent->BindAxis("RotateRight", this,&AMechPlayer::Rotate);
	PlayerInputComponent->BindAxis("TurnRight", this,&AMechPlayer::Turn);
	PlayerInputComponent->BindAction("FireCannonWeapon", IE_Pressed, this, &AMech::ShootCannonWeapon); 
	PlayerInputComponent->BindAction("FireCannonWeapon", IE_Released, this, &AMech::StopShootingCannonWeapon); 
	PlayerInputComponent->BindAction("FireLeftWeapon", IE_Pressed, this, &AMech::ShootLeftArmGun);
	PlayerInputComponent->BindAction("FireLeftWeapon", IE_Released, this, &AMech::StopShootingLeftArmGun);
	PlayerInputComponent->BindAction("FireRightWeapon", IE_Pressed, this, &AMech::ShootRightArmGun);
	PlayerInputComponent->BindAction("FireRightWeapon", IE_Released, this, &AMech::StopShootingRightArmGun);   
	PlayerInputComponent->BindAction("Suicide", IE_Pressed, this, &AMechPlayer::Suicide); 
	PlayerInputComponent->BindAction("SpawnEnemy", IE_Pressed, this, &AMechPlayer::SpawnEnemy);  
	PlayerInputComponent->BindAction("NextSpawnPoint", IE_Pressed, this, &AMechPlayer::NextSpawnPoint);  
	PlayerInputComponent->BindAction("PreviousSpawnPoint", IE_Pressed, this, &AMechPlayer::PreviousSpawnPoint);   
	PlayerInputComponent->BindAction("RestoreHealth", IE_Pressed, this, &AMechPlayer::RestoreHealth);    
	PlayerInputComponent->BindAction("KillAll", IE_Pressed, this, &AMechPlayer::KillAllEnemies);   
	PlayerInputComponent->BindAction("LoseLeftArm", IE_Pressed, this, &AMech::LoseLeftArm); 
	PlayerInputComponent->BindAction("LoseRightArm", IE_Pressed, this, &AMech::LoseRightArm);
	PlayerInputComponent->BindAction("LoseLeftLeg", IE_Pressed, this, &AMech::LoseLeftLeg);  
	PlayerInputComponent->BindAction("LoseRightLeg", IE_Pressed, this, &AMech::LoseRightLeg);
	PlayerInputComponent->BindAction("DashLeft", IE_Pressed, this, &AMech::DashLeft); 
	PlayerInputComponent->BindAction("DashRight", IE_Pressed, this, &AMech::DashRight);   
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMech::DashUp);    
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMechPlayer::Reload);     
}



