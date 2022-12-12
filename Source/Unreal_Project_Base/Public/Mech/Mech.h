// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/MovementComponent.h"
#include "Interfaces/DamagingInterface.h"
#include "Unreal_Project_Base/MechGameModeBase.h"
#include "Weapon/Weapon.h"
#include "Components/TimelineComponent.h"  
#include "Mech.generated.h"


class UWeaponData;
class UArrowComponent;
class UChildActorComponent;
class USkeletalMeshComponent;
class AMechEnemy;
class UPawnMovementComponent;
class UTimelineComponent;



UCLASS() 


class UNREAL_PROJECT_BASE_API AMech : public ACharacter, public IDamagingInterface 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMech();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category="Camera") 
	UArrowComponent* Arrow; 


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category="Camera")  
	UArrowComponent* ArrowUpperBodyForwardDirection;
	
	// LIMBS:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LimbComponents")
	UChildActorComponent* LeftArm;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LimbComponents")
	UChildActorComponent* RightArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LimbComponents")
	UChildActorComponent* LeftLeg;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LimbComponents")
	UChildActorComponent* RightLeg; 


	// WEAPON ACTORS:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponents") 
	UChildActorComponent* LeftArmGun; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponents")
	UChildActorComponent* RightArmGun;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponents")
	UChildActorComponent* CannonWeapon;

	// WEAPON DATA:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	UWeaponData* LeftArmWeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data") 
	UWeaponData* RightArmWeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	UWeaponData* CannonWeaponData;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	
	UFUNCTION()  
	virtual void SetWeaponTarget(AWeapon* Weapon) PURE_VIRTUAL(AMech::SetWeaponTarget);

	
	// HEALTH And DAMAGE :
	
	UPROPERTY(Replicated,  EditAnywhere, BlueprintReadWrite, Category= "Health")
	float Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Health")
	float MaxHealth = 100;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void SetDamagePoint(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void SetDamageRadial(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
	FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);

	virtual void SetDamage(float Damage, FVector HitLocation, const UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION() 
	virtual void Death() PURE_VIRTUAL(AMech::Death);   
	
	UFUNCTION(BlueprintCallable)
	void LookAtTarget (const FVector& TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = 0)) 
	float LookAtSpeed = 1.0f;

	

	// Jumping
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jumping", meta=(ClampMin = 0, ClampMax = 100000))
	float JumpPower = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Jumping")
	bool JumpAllowed = false;


	
	// DASHING
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dashing", meta=(ClampMin = 0, ClampMax = 10))
	float TimeBetweenDashes = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dashing", meta=(ClampMin = 0, ClampMax = 10))
	float TimeToCheckVelocityAfterCollision = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dashing", meta=(ClampMin = 0, ClampMax = 10000)) 
	float MaxDistanceTravelledForDashToStopAfterCollision = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dashing", meta=(ClampMin = 0, ClampMax = 10000)) 
	float DashPower = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dashing", meta=(ClampMin = 0, ClampMax = 100000))
	float DashDamageToMech = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dashing", meta=(ClampMin = 0, ClampMax = 100000))
	float DashDamageToWorld = 100.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dashing", meta = (ClampMin = 0, ClampMax = 100000))
	float DistanceDashDamage = 100.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dashing")
	UCurveFloat* DashCurveFloat;
	
	UPROPERTY()
	FTimerHandle NextDashTimerHandle;

	UPROPERTY()
	FTimerHandle VelocityCheckAfterCollisionTimerHandle; 
	
	UPROPERTY()
	bool CanDash = true;

	UPROPERTY(BlueprintReadOnly)
	bool Dashing = false;

	UPROPERTY()
	FVector CollisionPosition;   

	UPROPERTY() 
	FTimeline LeftDashTimeline;

	UPROPERTY() 
	FTimeline RightDashTimeline;
	
	UFUNCTION(BlueprintCallable)
	void DashRight ();

	UFUNCTION(BlueprintCallable)
	void DashLeft ();
	
	UFUNCTION(BlueprintCallable)
	void DashUp();

	UFUNCTION()
	void AllowDash();
	
	UFUNCTION()
	void LeftDashTimeLineProgress(float Value);

	UFUNCTION()
	void RightDashTimeLineProgress(float Value);

	UFUNCTION()
	void DashEnd();
	
	UFUNCTION()
	void CheckVelocityAfterCollision ();
	
	

	// COLLISION
	
	UFUNCTION()
	void OnMechHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit); 

	UPROPERTY()
	FString PreviousHitActor = "";

	UPROPERTY()
	FTimerHandle ForgetCollisionTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision", meta=(ClampMin = 0, ClampMax = 10))
	float TimeToForgetCollision = 2.0f;

	UFUNCTION()
	void ForgetCollision();

	
	// Create And Attach Function
	template<typename T> 
	T* CreateDefaultMovableMesh(const FName& Name, class USceneComponent* InParent, const FName& InSocketName = NAME_None);


	// LIMBS
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void LoseRightArm();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void LoseLeftArm();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void LoseRightLeg();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void LoseLeftLeg();

	// Components
	UPROPERTY()
	USkeletalMeshComponent* MechBody;

	UPROPERTY(EditAnywhere)   
	TSubclassOf<AMechEnemy> EnemyMechPrefab;

	UPROPERTY()
	UPawnMovementComponent* MovementComponent; 

protected:

	virtual void Tick(float DeltaSeconds) override;


public:
	
	UFUNCTION()
	bool InAir();

	UPROPERTY()
	TEAMS Team = TEAMS::NEUTRAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = -360, ClampMax = 360)) 
	float YawRotation = 0;

	
	// SHOOTING

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual  void ShootCannonWeapon(); 

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void StopShootingCannonWeapon(); 

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ShootLeftArmGun();

	UFUNCTION(Server, Reliable, BlueprintCallable) 
	void StopShootingLeftArmGun();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ShootRightArmGun(); 

	UFUNCTION(Server, Reliable, BlueprintCallable) 
	void StopShootingRightArmGun();

	UPROPERTY()
	bool FiringRightArm = false;

	UPROPERTY()
	bool FiringLeftArm = false;

	UPROPERTY()
	bool FiringCannon = false;
	
	UFUNCTION()
	void ContinueShooting(); 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cannon") 
	UCurveFloat* CannonAnimCurveFloat;

	UPROPERTY(BlueprintReadOnly)
	float CannonRollRotation = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Shooting", meta=(ClampMin = -360, ClampMax = 360)) 
	float DesiredCannonRollRotation = 240;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void FireCannon();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void FinishFiringCannon();

	UPROPERTY(BlueprintReadOnly)
	bool ZoomedIn = false; 

	// Cannon Animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cannon") 
	UTimelineComponent* CannonTimeLineComp;

	FOnTimelineFloat CannonAnimTimeLineFunc;

	UFUNCTION()
	void CannonTimelineUpdate(float Value);

	UPROPERTY(BlueprintReadOnly)
	float CannonAnimPercentage = 0; 



	// Reverse Cannon Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cannon") 
	UTimelineComponent* ReverseCannonTimeLineComp;

	FOnTimelineFloat ReverseCannonAnimTimeLineFunc;
	
	UFUNCTION()
	void ReverseCannonTimelineUpdate(float Value);

	FOnTimelineEvent CannonAnimationTimelineFinished;
	
	FOnTimelineEvent ReverseCannonAnimationTimelineFinished;



	// HEATBAR

	UPROPERTY(BlueprintReadOnly)
	float HeatBar = 0;

	UFUNCTION(BlueprintCallable)
	void AddHeat (float HeatToAdd);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeatBar", meta=(ClampMin = 0, ClampMax = 100))
	float ThreshHoldToStopShooting = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeatBar", meta=(ClampMin = 0, ClampMax = 100))
	float ThreshHoldToStartShooting = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeatBar", meta=(ClampMin = 0, ClampMax = 100))
	float ThreshHoldToStartLosingHealth = 80;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeatBar", meta=(ClampMin = 0, ClampMax = 100))
	float ThreshHoldToStartStopHealth = 70;


	UPROPERTY(BlueprintReadOnly)
	bool CanShootWeapons = true;

	UPROPERTY(BlueprintReadOnly)
	bool LosingHealth = false;

	UFUNCTION()
	void ConstantlyDecreaseHeat(float DeltaSeconds);

	UFUNCTION()
	void ConstantlyIncreaseHeatDuringMovement(float DeltaSeconds); 
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeatBar", meta=(ClampMin = 0, ClampMax = 10)) 
	float SpeedOfReducingHeat = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeatBar", meta=(ClampMin = 0, ClampMax = 10)) 
	float SpeedOfAddingHeatDuringMovement = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeatBar", meta=(ClampMin = 0, ClampMax = 10)) 
	float SpeedOfLosingHealth = 1;

	UPROPERTY(BlueprintReadOnly)
	bool Alive = true; 
	
};

