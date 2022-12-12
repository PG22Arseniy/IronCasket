// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mech/Mech.h"
#include "MechEnemy.generated.h" 

class APatrolPoint;
class UCapsuleComponent;
class UArrowComponent;
class AEnemyAIController;

UENUM()
enum class MACHINEGUN_ATTACK_TYPES 
{
	LEFT_ATTACK = 0,
	RIGHT_ATTACK = 1,
	BOTH_ATTACK = 2,
	NONE = 3
};


UENUM()
enum class MACHINEGUN_ATTACK_TARGETS
{
	LEFT_ARM = 0,
	RIGHT_ARM = 1,
	BODY_CENTER = 2,
	LEFT_LEG = 3,
	RIGHT_LEG = 4
};

/**
 * 
 */
UCLASS()
class UNREAL_PROJECT_BASE_API AMechEnemy : public AMech
{
	GENERATED_BODY()

public:


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UCapsuleComponent* LeftTargetForAimAssist;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UCapsuleComponent* RightTargetForAimAssist;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UArrowComponent* AimAssistArrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UCapsuleComponent* AimAssistCenterTarget;
	
	AMechEnemy();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TArray<APatrolPoint*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TArray<AMechEnemy*> CombatGroup;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0, ClampMax = 10)) 
	float CombatTurnSpeedTowardsPlayer = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category="AI") 
	MACHINEGUN_ATTACK_TYPES AttackType = MACHINEGUN_ATTACK_TYPES::LEFT_ATTACK;

	UPROPERTY(BlueprintReadWrite, Category="AI") 
	MACHINEGUN_ATTACK_TARGETS AttackTarget = MACHINEGUN_ATTACK_TARGETS::LEFT_ARM;

	UFUNCTION() 
	virtual void RotateCorrectly(float DeltaSeconds); 

	UPROPERTY()
	int32 NumberInCombat; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0, ClampMax = 360))
	float MaxAngleToStartShooting = 30.0f;

	UPROPERTY(BlueprintReadWrite)
	int32 NumberInAttack = 0;
	
	UPROPERTY(BlueprintReadWrite)  
	bool RightDirectionCircle = true;

	UPROPERTY(BlueprintReadWrite)   
	bool InCircle = false; 

	virtual void Death() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	bool PlayerDetected = false;

	UPROPERTY()
	bool CanAttack = false;

	UFUNCTION(BlueprintImplementableEvent)
	void HitByPlayer(); 

	UFUNCTION(BlueprintImplementableEvent)
	void HitByAnyone();
	
	UFUNCTION()
	void DetectPlayer();

	UPROPERTY()
	bool DisabledAI = false;
	
	
protected:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetDamage(float Damage, FVector HitLocation, const UDamageType* DamageType, AActor* DamageCauser) override;
	

	UFUNCTION(BlueprintImplementableEvent, Category="Death")
	void OnEnemyDeath();

	UPROPERTY(EditAnywhere, Category=AI)
	bool DashingWhenHit = true;

	UFUNCTION(BlueprintCallable)
	void SwitchDirectionInCircle();

	UFUNCTION()
	void AllowSwitch();

	UPROPERTY()
	FTimerHandle SwitchDirectionTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0, ClampMax = 10))
	float TimeBetweenSwitches = 2;

	UPROPERTY(BlueprintReadOnly) 
	bool CanSwitch = true;
	
	UPROPERTY()
	bool SawBefore = false;
	
	UPROPERTY()
	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0, ClampMax = 10))
	float TimeToAttack = 2.0f;

	UFUNCTION(BlueprintCallable)
	void ChangeAttackTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(ClampMin = 0))
	float TimeBetweenAttackTargetChange = 2.0f;

	UPROPERTY()
	FTimerHandle AttackTargetChangeHandle;
	
	UFUNCTION(BlueprintCallable)
	void StopOutline();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(ClampMin = 0))
	float TimeToStopOutline = 5;

	UFUNCTION(BlueprintCallable)
	void RenderCustomDepthOff();

	UPROPERTY()
	FTimerHandle OutlineStopHandle;

	UFUNCTION()
	void RotateAimAssistTowardPlayer();

	
	UPROPERTY(BlueprintReadWrite) 
	AEnemyAIController* AIController;

	UPROPERTY()
	APawn* PlayerMech;
};



