// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech/Mech.h"
#include "DrawDebugHelpers.h"
#include "NavigationTestingActor.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interfaces/BaseDamage.h"
#include "Kismet/GameplayStatics.h"
#include "Mech/CannonEnemy.h"
#include "Mech/MechPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/WeponTypes/ProjectileWeapon.h"
#include "Weapon/WeponTypes/MachineGun.h"


// Sets default values
AMech::AMech()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 
	 
	MechBody = GetMesh(); 
	
	// All Limb Actors Setup:
	LeftArm = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("LEFT HAND"), MechBody, TEXT("LeftHand"));
	RightArm = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("RIGHT HAND"), MechBody, TEXT("RightHand"));
	LeftLeg = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("LEFT LEG"), MechBody, TEXT("LeftLeg"));
	RightLeg = CreateDefaultMovableMesh<UChildActorComponent>(TEXT("RIGHT LEG"), MechBody, TEXT("RightLeg"));


	// Arrow Direction of upper body
	ArrowUpperBodyForwardDirection = CreateDefaultMovableMesh<UArrowComponent>(TEXT("UPPER BODY FORWARD DIRECTION ARROW"), MechBody, TEXT("Spine")); 

	CannonTimeLineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("CannonAnimation"));
	ReverseCannonTimeLineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("ReverseCannonAnimation")); 
	
}

// Called when the game starts or when spawned
void AMech::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetMovementComponent();  

	// Bind Take Damage Functions
	OnTakeRadialDamage.AddDynamic(this, &AMech::SetDamageRadial);
	OnTakePointDamage.AddDynamic(this, &AMech::SetDamagePoint);

	// Actor Collision Hit Bind
	OnActorHit.AddDynamic(this, &AMech::AMech::OnMechHit); 

	// Timelines setup:
	
	if (DashCurveFloat)
	{
		// Left Dash Timeline
		FOnTimelineFloat LeftDashTimeLineProgress; 

		LeftDashTimeLineProgress.BindUFunction(this, FName("LeftDashTimeLineProgress"));

		LeftDashTimeline.AddInterpFloat(DashCurveFloat, LeftDashTimeLineProgress);


		// Right Dash timeline
		FOnTimelineFloat RightDashTimeLineProgress; 

		RightDashTimeLineProgress.BindUFunction(this, FName("RightDashTimeLineProgress"));

		RightDashTimeline.AddInterpFloat(DashCurveFloat, RightDashTimeLineProgress);


		// Bind DashEnd Function to the end of both timelines
		FOnTimelineEvent TimeLineFinished;

		TimeLineFinished.BindUFunction(this, FName("DashEnd"));
		
		//LeftDashTimeline.SetTimelineFinishedFunc(TimeLineFinished);
		//RightDashTimeline.SetTimelineFinishedFunc(TimeLineFinished); 
		
	}


	if (CannonAnimCurveFloat)
	{

		// Animation Of Cannon Forward 
		CannonAnimTimeLineFunc.BindUFunction(this, FName("CannonTimelineUpdate"));
		CannonTimeLineComp->AddInterpFloat(CannonAnimCurveFloat, CannonAnimTimeLineFunc, FName("CannonAnim"));
	
		CannonTimeLineComp->SetLooping(false);
		CannonTimeLineComp->SetIgnoreTimeDilation(true);


		CannonAnimationTimelineFinished.BindUFunction(this, FName("FireCannon")); 
		CannonTimeLineComp->SetTimelineFinishedFunc(CannonAnimationTimelineFinished);


		
		// Animation Of Cannon Backwards
		
		ReverseCannonAnimTimeLineFunc.BindUFunction(this, FName("ReverseCannonTimelineUpdate"));
		ReverseCannonTimeLineComp->AddInterpFloat(CannonAnimCurveFloat, ReverseCannonAnimTimeLineFunc, FName("ReverseCannonAnim"));
	
		ReverseCannonTimeLineComp->SetLooping(false);
		ReverseCannonTimeLineComp->SetIgnoreTimeDilation(true); 

		
		ReverseCannonAnimationTimelineFinished.BindUFunction(this, FName("FinishFiringCannon"));
		ReverseCannonTimeLineComp->SetTimelineFinishedFunc(ReverseCannonAnimationTimelineFinished);
		        
	}
	
	Alive = true;
}




void AMech::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMech, Health);  
}


// DAMAGE SYSTEM
void AMech::SetDamagePoint(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
	AActor* DamageCauser)
{
	SetDamage(Damage, HitLocation, DamageType, DamageCauser);
}

void AMech::SetDamageRadial(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
	FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	SetDamage(Damage, HitInfo.Location, DamageType, DamageCauser);
}

void AMech::SetDamage(float Damage, FVector HitLocation, const UDamageType* DamageType, AActor* DamageCauser)
{

	if (Health <= 0 ) return;  
	const UBaseDamage* BaseDamageType = Cast<UBaseDamage>(DamageType);


	if (Health == -1) return; 
	
	if (BaseDamageType)
	{
		float DamageMultiplier = BaseDamageType->ProcessDamage(DamageCauser, this, HitLocation);
		 
		Health -= Damage * DamageMultiplier; 
	}

	if (Health <= 0)  
	{
		Death();
		Alive = false; 
	}
}

// Turn Mech Body towards target
void AMech::LookAtTarget(const FVector& TargetLocation)
{ 

	FRotator Rot = GetActorRotation();
	FVector LookAtDirection = TargetLocation - GetActorLocation();

	LookAtDirection.Normalize(); 

	FRotator R = FRotationMatrix::MakeFromX(LookAtDirection).Rotator(); 

	float YawRot = Rot.Yaw;

	if (YawRot > R.Yaw + LookAtSpeed)
		YawRot -= LookAtSpeed;
	else
		if (YawRot < R.Yaw - LookAtSpeed) YawRot += LookAtSpeed;   
	
	SetActorRotation({Rot.Pitch, YawRot , Rot.Roll});   

}



// DASHING FUNCTIONS:


void AMech::DashRight()
{
	if (!CanDash) return; 

	if (ZoomedIn) return;
	
	RightDashTimeline.PlayFromStart();

	Dashing = true;
	
	CanDash = false; 

	GetWorldTimerManager().SetTimer(NextDashTimerHandle, this ,&AMech::AllowDash, TimeBetweenDashes, false);
}




void AMech::DashLeft() 
{
	
	if (!CanDash) return;  

	if (ZoomedIn) return; 
	
	LeftDashTimeline.PlayFromStart();

	Dashing = true;
	
	CanDash = false;

	GetWorldTimerManager().SetTimer(NextDashTimerHandle, this ,&AMech::AllowDash, TimeBetweenDashes, false);
	
}


void AMech::DashUp()
{
	if (!JumpAllowed) return;
	
 	if (InAir() || !CanDash) return; 
	
	LaunchCharacter( GetActorUpVector()* JumpPower, true, true);
	
	CanDash = true;
	
}

void AMech::AllowDash()
{
	CanDash = true;
}

void AMech::LeftDashTimeLineProgress(float Value)
{

	const FVector Velocity  = MovementComponent->Velocity; 
	
	const FVector NewVelocity = FMath::Lerp(Velocity,  ArrowUpperBodyForwardDirection->GetForwardVector() * DashPower , Value); 
	
	MovementComponent->Velocity = NewVelocity;
	
}

void AMech::RightDashTimeLineProgress(float Value)
{

	const FVector Velocity  = MovementComponent->Velocity; 
	
	const FVector NewVelocity = FMath::Lerp(Velocity,  - ArrowUpperBodyForwardDirection->GetForwardVector() * DashPower , Value);   
	
	MovementComponent->Velocity = NewVelocity;
	
}

void AMech::DashEnd()
{
	GetMovementComponent()->Velocity = {0,0,0};

	Dashing = false; 
}

void AMech::CheckVelocityAfterCollision()
{
	const FVector CurLoc = GetActorLocation();

	const float Distance = FVector::Dist(CurLoc, CollisionPosition);
	
	
	 if (Distance < MaxDistanceTravelledForDashToStopAfterCollision)  
	 {
	 	Dashing = false;  
	 	RightDashTimeline.Stop();  
	 	LeftDashTimeline.Stop();
	 }
}

void AMech::OnMechHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

	// Hit Register only when Dashing
	if (!Dashing) return;
	
	// ignore collision double collision (only register first)
	if (OtherActor->GetName() != PreviousHitActor)
	{
		PreviousHitActor = OtherActor->GetName();

		// Forget about collided actor
		GetWorldTimerManager().ClearTimer(ForgetCollisionTimer);
		GetWorldTimerManager().SetTimer(ForgetCollisionTimer, this, &AMech::ForgetCollision, TimeToForgetCollision, false); 
	}

	else return;   
	
	
	// Check Travelled Distance After Collision 
	if (!GetWorldTimerManager().TimerExists(VelocityCheckAfterCollisionTimerHandle))
	{
		GetWorldTimerManager().SetTimer(VelocityCheckAfterCollisionTimerHandle, this, &AMech::CheckVelocityAfterCollision, TimeToCheckVelocityAfterCollision, false);
		CollisionPosition = GetActorLocation();
		
	}
	
	
	UWorld* world = GetWorld();
	if (!world) return;

	APawn* instigator = GetInstigator();
	if (!instigator) return;

	AActor* owner = GetOwner();
	if (!owner) return;

	// ignore Instigator
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(instigator);
	ignoreActors.Add(owner);

	// Damage Mech
	if(OtherActor->GetClass()->ImplementsInterface(UDamagingInterface::StaticClass()))
	{
	
		UGameplayStatics::ApplyPointDamage(OtherActor, DashDamageToMech, NormalImpulse, Hit,
				instigator->GetController(), owner, UBaseDamage::StaticClass());
	}

	// Damage World 
	else
	{
		TArray<AActor*> IgnoredActors;

		IgnoredActors.Add(SelfActor);   
	
		UGameplayStatics::ApplyRadialDamage(OtherActor, DashDamageToWorld, Hit.Location, 
			DistanceDashDamage, UBaseDamage::StaticClass(), IgnoredActors, owner, instigator->GetController()); 
	}

	
}

void AMech::ForgetCollision()
{
	PreviousHitActor = ""; 
}


bool AMech::InAir()
{
	return 	(MovementComponent->IsFlying() || MovementComponent->IsFalling());
}


void AMech::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); 
	
	// Dashing Timelines 
	
	RightDashTimeline.TickTimeline(DeltaSeconds);  
	
	LeftDashTimeline.TickTimeline(DeltaSeconds);

	ConstantlyDecreaseHeat(DeltaSeconds); 
}

void AMech::ContinueShooting() 
{
	if (!CanShootWeapons) return; 

	// if Weapons Are being fired Shoot
	if (FiringRightArm)
	{
		if (AWeapon* Weapon = Cast<AWeapon>(RightArmGun->GetChildActor())) 
		{ 
			Weapon->Fire(); 
		
		}
	}

	if (FiringLeftArm)
	{
		if (AMachineGun* Weapon = Cast<AMachineGun>(LeftArmGun->GetChildActor()))
		{
			Weapon->Fire();  
		}
	} 
}


void AMech::CannonTimelineUpdate(float Value)
{
	const float NewRollRot = FMath::Lerp(CannonRollRotation,  DesiredCannonRollRotation , Value);
	
	CannonRollRotation = NewRollRot;

	CannonAnimPercentage = CannonRollRotation/DesiredCannonRollRotation;   
	
}

void AMech::ReverseCannonTimelineUpdate(float Value)
{ 
	const float NewRollRot = FMath::Lerp(CannonRollRotation, 0.0f , Value);
	
	CannonRollRotation = NewRollRot;

	CannonAnimPercentage = CannonRollRotation/DesiredCannonRollRotation; 
	
}


void AMech::AddHeat(float HeatToAdd)
{
	// Check Max Cap
	if (HeatBar + HeatToAdd >= 100)
	{
		HeatBar = 100;
	}
	else

	// Check Min Cap
	if (HeatBar + HeatToAdd <= 0)
	{
		HeatBar = 0; 
	}
	else
	
	HeatBar += HeatToAdd; 

	if (HeatBar >= ThreshHoldToStopShooting)
	{
		CanShootWeapons = false;
		StopShootingCannonWeapon();
		StopShootingLeftArmGun();
		StopShootingRightArmGun();
	}
	if (!CanShootWeapons)  
	if (HeatBar <= ThreshHoldToStartShooting)
	{
		CanShootWeapons = true;
		ContinueShooting();  
	}

	if (HeatBar >= ThreshHoldToStartLosingHealth)
	{
		LosingHealth = true;
	}

	if (HeatBar <= ThreshHoldToStopShooting)
	{
		LosingHealth = false;
	}
}

void AMech::ConstantlyDecreaseHeat(float DeltaSeconds)
{
	float HeatToLose = FMath::Lerp(HeatBar, 0.0f, DeltaSeconds * SpeedOfReducingHeat);

	AddHeat(HeatToLose - HeatBar);   

	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaSeconds, FColor::Silver, FText::AsNumber(HeatBar).ToString());  

	if (LosingHealth)
	{
		Health = FMath::Lerp(Health, 0.0f, DeltaSeconds * SpeedOfLosingHealth);
		if (Health <= 0) Death(); 
	}
}

void AMech::ConstantlyIncreaseHeatDuringMovement(float DeltaSeconds)
{
	float HeatToAdd = FMath::Lerp(HeatBar, 100.0f , DeltaSeconds * SpeedOfAddingHeatDuringMovement);
	AddHeat(HeatToAdd - HeatBar); 
}

void AMech::FinishFiringCannon_Implementation()
{ 
	// This is fired When Cannon Reverse Animation Is over
	if (AMechPlayer* Player = Cast<AMechPlayer>(this))
	{
		if (Player->CanMoveCondition == CAN_MOVE_CONDITION::ON_CANNON_ANIMATION_FINISH)
			Player->ZoomedIn = false; 
	} 
	
}

void AMech::FireCannon_Implementation()
{
	if (AMechPlayer* Player = Cast<AMechPlayer>(this))
	{
		if (AMechEnemy* Enemy = Cast<AMechEnemy>(Player->ReticleHit.Actor))
		{
			Player->TargetActor = Enemy;
		}

		else
		{
			Player->TargetActor = nullptr;  
		}
	}
	if (!CannonWeapon) return; 
	if (AWeapon* Weapon = Cast<AWeapon>(CannonWeapon->GetChildActor()))
	{
		SetWeaponTarget(Weapon);
		
		Weapon->Fire();

		if (ACannonEnemy* CannonEnemy = Cast <ACannonEnemy>(this))
		{
			//StopShootingCannonWeapon();  
		}
	}
}


void AMech::StopShootingRightArmGun_Implementation()
{
	
	if (!RightArmGun) return;
	if (AWeapon* Weapon = Cast<AWeapon>(RightArmGun->GetChildActor()))
	{
		Weapon->StopFiring();  
	}
	FiringRightArm = false;  
}

void AMech::ShootRightArmGun_Implementation()
{
	FiringRightArm = true;
	
	if (!CanShootWeapons) return;
	
	if (!RightArmGun) return;
	if (AWeapon* Weapon = Cast<AWeapon>(RightArmGun->GetChildActor()))  
	{
		SetWeaponTarget(Weapon);
		Weapon->Fire();
	}

}

void AMech::StopShootingLeftArmGun_Implementation()
{
	if (!LeftArmGun) return;
	if (AWeapon* Weapon = Cast<AWeapon>(LeftArmGun->GetChildActor()))
	{
		Weapon->StopFiring(); 
	}
	FiringLeftArm = false;
}

void AMech::ShootLeftArmGun_Implementation()
{
	FiringLeftArm = true;
	
	if (!CanShootWeapons) return;
	
	if (!LeftArmGun) return;
	
	AWeapon* Weapon = Cast<AWeapon>(LeftArmGun->GetChildActor());

	if (!Weapon) return; 
	SetWeaponTarget(Weapon);
	
	Weapon->Fire();
	
}

void AMech::ShootCannonWeapon_Implementation()
{
	
	
	FiringCannon = true;

	if (!CanShootWeapons) return; 
	
	ReverseCannonTimeLineComp->Stop();
	if (!CannonTimeLineComp->IsPlaying())
	{
		CannonTimeLineComp->PlayFromStart();
	}
	
}
void AMech::StopShootingCannonWeapon_Implementation() 
{
	if (!CannonWeapon) return; 
	if (AWeapon* Weapon = Cast<AWeapon>(CannonWeapon->GetChildActor()))
	{
		Weapon->StopFiring();  
	}
	FiringCannon = false; 

	CannonTimeLineComp->Stop();
	if (!ReverseCannonTimeLineComp->IsPlaying())
	{
		ReverseCannonTimeLineComp->PlayFromStart(); 
	}
} 

// LIMB LOSING FUNCTIONS:
void AMech::LoseLeftLeg_Implementation()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Black, "Lose Left Leg");
	MechBody->HideBoneByName("LeftLeg", PBO_None);  
}

void AMech::LoseRightLeg_Implementation()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Black, "Lose Right Arm");
	MechBody->HideBoneByName("RightLeg", PBO_None);  
}

void AMech::LoseLeftArm_Implementation()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Black, "Lose Left Arm");
	MechBody->HideBoneByName("LeftArm", PBO_None);   
}

void AMech::LoseRightArm_Implementation()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Black, "Lose Right Arm");
	MechBody->HideBoneByName("RightArm", PBO_None);
	
}

// Create + Attach Component to Skeletal Mesh
template <typename T>
T* AMech::CreateDefaultMovableMesh(const FName& Name, class USceneComponent* InParent, const FName& InSocketName)
{
	T* Ptr = CreateDefaultSubobject<T>(Name);
	Ptr->SetupAttachment(InParent, InSocketName);
	Ptr->SetMobility(EComponentMobility::Movable);
	return Ptr;
}
 