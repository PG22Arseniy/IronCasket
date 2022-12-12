// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mech/Mech.h"
#include "GameFramework/Character.h" 
#include "MechPlayer.generated.h"

/**
 * 
 */

class USpringArmComponent;
class UCameraComponent;
class UAIPerceptionStimuliSourceComponent;
class UCurveFloat;
class AMechEnemy;
class ADebugSpawnPoint;
class USphereComponent;

UENUM()
enum class TARGET_TYPE 
{
	MECH_ENEMY = 0,
	WORLD = 1,
	DESTRUCTABLE = 2
};

UENUM()
enum class MOVEMENT_TYPE
{
	SPLITBODY = 0,
	TANK_ONLYFORWARD = 1,
	TANK_WITHBACKWARDS = 2
};

UENUM()
enum class CAN_MOVE_CONDITION
{
	ON_ZOOMOUT_TIMILNE_FINISH,
	ON_ZOOMOUT_TIMELINE_START,
	ON_CANNON_ANIMATION_FINISH,
	ALWAYS_CAN_MOVE
};

UENUM()
enum class AIM_ASSIST_TYPE
{
	MOUSE_SENSITIVITY_DAMPENING,
	TWO_COLLIDERS_ON_SIDES 
};

UCLASS()
class UNREAL_PROJECT_BASE_API AMechPlayer : public AMech
{
	GENERATED_BODY()

public:

	AMechPlayer();

	// COMPONENTS
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category="Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category="Camera") 
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category="Debug")  
	UCameraComponent* DebugCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category="Camera")
	USphereComponent* FirstCameraCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category="Camera")
	USphereComponent* SecondCameraCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,Category="Camera")
	USphereComponent* UpperCameraCollider;
	

	
	// CAMERA

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Camera", meta=(ClampMin = -10000, ClampMax = 10000))
	float DesiredArmLength = 500;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Camera", meta=(ClampMin = -10000, ClampMax = 10000))
	float DesiredSocketOffsetY = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Camera", meta=(ClampMin = -10000, ClampMax = 10000))
	float DesiredSocketOffsetZ = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin = 0, ClampMax = 100))
	float CameraOffsetRateLookingUp = 20;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin = 0, ClampMax = 100))
	float CameraOffsetRateLookingDown = 5;   

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin = 0, ClampMax = 2000))
	float CameraDistanceToHidePlayer = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin = 0, ClampMax = 2000))
	float CameraDistanceToUnHidePlayer = 550;

	UPROPERTY()
	float DefaultArmLength;
	
	UPROPERTY(BlueprintReadOnly)
	FVector CameraZoomedInPosition;

	UPROPERTY(BlueprintReadOnly) 
	FVector CameraDefaultPosition; 

	UPROPERTY()
	bool BodyHidden = false;

	UFUNCTION()
	void AdjustTargetOffset();

	UPROPERTY()
	FRotator DefaultCameraRotation;

	UPROPERTY(BlueprintReadWrite) 
	bool DebugCameraMode = false;

	UPROPERTY(BlueprintReadOnly)
	bool FirstColliderOverlapping = false;

	UPROPERTY(BlueprintReadOnly)
	bool SecondColliderOverlapping = false;

	UPROPERTY(BlueprintReadOnly)
	bool UpperColliderOverlapping = false;

	UFUNCTION()
	void FirstColliderOnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SecondColliderOnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void UpperColliderOnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	void UpperColliderOnOverlapEnd(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
	void FirstColliderOnOverlapEnd(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SecondColliderOnOverlapEnd(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void CameraChangeState();

	UPROPERTY()
	float DesiredSpringArmOffsetZ = 300;

	UPROPERTY()
	float DesiredSpringArmOffsetY = 10;

	UPROPERTY()
	float DefaultSpringArmOffsetY = 10;  
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin = -1000, ClampMax = 1000)) 
	float SpringArmOffsetYInCenter = -300; 
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	UTimelineComponent* CameraPositionStateChangeTimelineComp;

	FOnTimelineFloat CameraPositionStateChangeTimelineFunc;

	UFUNCTION()
	virtual void CameraPositionStateChangeTimelineProgress(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	UCurveFloat* CameraPositionStateChangeCurveFloat;
	
	UPROPERTY()
	float LeftSideCameraOffset = -300;

	UPROPERTY()
	float RightSideCameraOffset = 300;

	UPROPERTY()  
	float CenterCameraOffset = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(ClampMin = 0, ClampMax = 2));   
	float CameraDampeningRate = 1;
	
	
	
	// SHOOTING
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponTarget", meta=(ClampMin = 0));  
	float MaxTargetDistance = 25000; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting", meta=(ClampMin = 0, ClampMax = 10.0f)) 
	float InitialCannonShotCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting") 
	bool OnLeftClickCannonShot = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting") 
	bool ZoomOutAfterShot = true;

	UPROPERTY()
	FVector End; 

	UPROPERTY()
	FHitResult ReticleHit;

	UPROPERTY()
	FTimerHandle CannonShotTimerHandle;
	
	UPROPERTY(BlueprintReadOnly)
	AMechEnemy*	TargetActor;
	 
	UPROPERTY()
	FVector CannonTarget = {0,0,0}; 
	
	UFUNCTION()
	void SetReticleTarget(); 

	UFUNCTION(BlueprintCallable)
	void Reload();
	
	virtual void SetWeaponTarget(AWeapon* Weapon) override;
	
	virtual void ShootCannonWeapon() override;

	virtual void ShootRightArmGun() override;

	virtual void ShootLeftArmGun() override;

	UPROPERTY() 
	bool FocusedOnTarget = false;
	
	
	// ROTATION AND MOVEMENT

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = -1, ClampMax = 180)) 
	float MaxSideRotation = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = 0, ClampMax = 1)) 
	float TankMovementTurnSpeed = 0.01;  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = 0, ClampMax = 360))
	float MaxLookDownAngle = 50.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = 0, ClampMax = 360))
	float MaxLookUpAngle = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = 0, ClampMax = 360))
	float MaxZoomedInLookDownAngle = 10.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = 0, ClampMax = 360))
	float MaxZoomedInLookUpAngle = 20.0f;

	UPROPERTY()
	int32 AimAxisValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = 0, ClampMax = 10 )) 
	float BaseTurnRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = 0, ClampMax = 10 )) 
	float BaseHorizontalRotateRate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation", meta=(ClampMin = 0, ClampMax = 10 )) 
	float BaseVerticalRotateRate = 1.0f;

	UFUNCTION(BlueprintCallable) 
	void  Turn(float AxisValue);

	UFUNCTION(Server, Reliable, BlueprintCallable) 
	void  Rotate(float AxisValue);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void  LookUp(float AxisValue);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void  MoveForward(float AxisValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	MOVEMENT_TYPE MovementType = MOVEMENT_TYPE::TANK_WITHBACKWARDS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(ClampMin = 0))
	float TimeToAccelerate = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(ClampMin = 0, ClampMax = 3000))
	float NewMaxSpeed = 1500;

	UPROPERTY()
	FTimerHandle AccelerateHandle;

	UFUNCTION()
	void MechAccelerate();

	UFUNCTION()
	void MechDeaccelerate();

	UPROPERTY()
	float DefaultMaxSpeed;

	UPROPERTY()
	bool YawRotationInput = false;

	UPROPERTY()
	bool GoesRight = false;

	UPROPERTY()
	bool GoesBackwards = false; 


	// AIM ASSIST


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AimAssist")
	AIM_ASSIST_TYPE AimAssistType = AIM_ASSIST_TYPE::MOUSE_SENSITIVITY_DAMPENING;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AimAssit")
	UTimelineComponent* AimAssistTimelineComp;

	FOnTimelineFloat AimAssistTimelineFunc;

	UFUNCTION()
	virtual void AimAssistTimelineProgress(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AimAssist")
	UCurveFloat* AimAssistCurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AimAssist", meta=(ClampMin = 0, ClampMax = 1000)) 
	float AimAssistOffset = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AimAssist", meta=(ClampMin = 0, ClampMax = 2))  
	float AimAssistDampeningRate = 1;
	
	UPROPERTY()
	float AimDesiredYaw; 

	UFUNCTION()
	void DeactivateAimAssist();

	UPROPERTY(BlueprintReadOnly)
	bool AimAssistActive = false;

	UPROPERTY()
	bool CanAssist = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AimAssist", meta=(ClampMin = 0, ClampMax = 5))
	float AllowAssistRate = 1;

	UFUNCTION()
	void AllowAssist();

	FTimerHandle AllowAssistHandle;  

	UPROPERTY() 
	float MouseDampeningRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AimAssist", meta=(ClampMin = 0, ClampMax = 10 )) 
	float MouseDampenedRate = 0.3f;

	
	
	
	

	// DEBUG FUNCTIONS
	
	UFUNCTION()
	void Suicide();

	UFUNCTION() 
	void SpawnEnemy();

	UFUNCTION() 
	void NextSpawnPoint();

	UFUNCTION() 
	void PreviousSpawnPoint();

	UFUNCTION(BlueprintCallable) 
	void KillAllEnemies();

	UFUNCTION()
	void RestoreHealth();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMechOpacity(float OpacityValue);

	UFUNCTION(BlueprintCallable)
	void LockDebugCamera();

	UPROPERTY(BlueprintReadOnly)
	bool DebugCameraLocked = false;

	UPROPERTY(BlueprintReadOnly)
	FVector LockedPositionOfCamera;

	UPROPERTY(BlueprintReadOnly)
	FRotator LockedRotationOfCamera;


	// Death

	UPROPERTY(BlueprintReadOnly)
	bool GameplayInputDisabled = true;

	UFUNCTION(BlueprintCallable)
	void DisablePlayerInput();
	
	
protected:

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override; 
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void FinishFiringCannon() override;

	virtual void StopShootingCannonWeapon() override;
	
public:
	
	virtual void Death() override;
	

	UFUNCTION(BlueprintCallable)
	void AmmoRestore();
	
	UFUNCTION()
	void PlayerReset();
	
	UFUNCTION()
	void AllowCannonFire();

	UPROPERTY()
	bool CanFireCannon = false; 

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerDeath();
	
private:

	UPROPERTY()
	UAIPerceptionStimuliSourceComponent * Stimulus;

	UFUNCTION()
	void SetUpStimulus();
	
	int32 CurrentSpawnPoint = 0;
	
	int32 LastSpawnPoint = 0;   

public:
	
	UPROPERTY()
	TARGET_TYPE CurrentTarget = TARGET_TYPE::WORLD;

	UPROPERTY()
	AActor* PreviousActorHit = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void ReticleColorSwap(FHitResult HitResult, bool HitDone);

	UPROPERTY()
	bool DisableYawRot = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SpawnPoint")
	TArray<ADebugSpawnPoint*> SpawnPoints;

	UFUNCTION(BlueprintCallable)
	void AddHealth(float HealthToAdd);

	UFUNCTION(BlueprintCallable, BlueprintPure) 
	FRotator GetUpperBodyRotation();
	
	virtual void FireCannon() override;

	// Zooming In
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zooming") 
	UTimelineComponent* ZoomInTimeLineComp;

	FOnTimelineFloat ZoomInTimeLineFunc;

	FOnTimelineEvent ZoomInTimelineFinished;

	UFUNCTION()
	virtual void ZoomInTimeLineProgress(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zooming")
	UCurveFloat* ZoomInCurveFloat;

	UFUNCTION()
	void ZoomedInFunc();

	

	// Zooming Out
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zooming") 
	UTimelineComponent* ZoomOutTimeLineComp;

	FOnTimelineFloat ZoomOutTimeLineFunc;
	
	FOnTimelineEvent ZoomOutTimelineFinished;
	
	UFUNCTION()
	virtual void ZoomOutTimeLineProgress(float Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zooming")
	UCurveFloat* ZoomOutCurveFloat;
	
	UFUNCTION()
	void ZoomedOutFunc();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	CAN_MOVE_CONDITION CanMoveCondition = CAN_MOVE_CONDITION::ON_ZOOMOUT_TIMILNE_FINISH;

	
	
};

