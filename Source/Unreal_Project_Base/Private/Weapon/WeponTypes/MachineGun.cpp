// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeponTypes/MachineGun.h"
#include "Interfaces/BaseDamage.h"
#include "Interfaces/DamagingInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Mech/MechEnemy.h"
#include "Weapon/WeaponData.h"   

AMachineGun::AMachineGun()
{
	PrimaryActorTick.bCanEverTick = true;  
}

void AMachineGun::Fire() 
{
	Super::Fire(); 
}


void AMachineGun::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
	
	
	// Once cooldown reaches 0 or below Start Reloading
	if (WeaponData->CurrentAmmo <= 0 &&  WeaponData->CurrentAmmo > -1)
	{
		if (!Reloading) Reload();   
		return; 
	}


	// if no firing or gun is reloading return
	if (!Firing || Reloading) return;

	
	// Decrease Cooldown When Shooting
	if (WeaponData->CurrentAmmo != -1)
	{

		if (WeaponData->ShootingRate == 0) return; 

		FiringTime ++;
		
		if (FiringTime % WeaponData->ShootingRate == 0)  
			WeaponData->CurrentAmmo -= 1;
	}  

	
	// Get Target Location and Start Location
	const FVector Start = GetActorLocation();
	const FVector End = TargetForBullet; 
	

	// Get Direction for LineTrace
	FVector Direction = (End - Start);
	Direction.Normalize();
	

	
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	
	// Get Instigator
	APawn* instigator = Cast<APawn>(GetParentActor()); 
	if (!instigator) return;
	
	// ignore Instigator
	ActorsToIgnore.Add(instigator); 
	
	// Do Line Trace 
	bool bHit = UKismetSystemLibrary::LineTraceSingle(  this, Start, End,  
											UEngineTypes::ConvertToTraceType(ECC_Camera),     
											false, ActorsToIgnore, EDrawDebugTrace::ForDuration, 
											HitResult, true, FLinearColor::Yellow,
											FLinearColor::White, 0.04);    

	
	// if something was hit
	if (bHit) {

		ShootingTime ++;

		// Apply any damage based on Damage Rate
		if (ShootingTime % DamageRate == 0)
		{
			
			AActor* HitActor = Cast<AActor>(HitResult.Actor);

			if (!HitActor) return;
		
			if (!WeaponData ) return;
			
			if (!instigator->Controller ) return;

			
			if(HitResult.Actor->GetClass()->ImplementsInterface(UDamagingInterface::StaticClass()))
			{

				// Check If Hit Mech is on the same team
				AMech* MechOwner = Cast<AMech>(instigator);
				AMech* MechTarget = Cast<AMech>(HitActor);

				if (MechOwner->Team == MechTarget->Team) return;  
				
				
				//GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow, "HIT MECH");

				
				// Apply Damage to the opposing Mech
				UGameplayStatics::ApplyPointDamage(HitActor, WeaponData->DamageForMech,Direction, HitResult,
													instigator->Controller, instigator, 
													UBaseDamage::StaticClass());
					
				MachineGunHitMech(HitResult);    
			}
			else 
			{
				// Apply Damage to the World
				UGameplayStatics::ApplyPointDamage(HitActor, WeaponData->DamageForDestructible, Direction, HitResult,
										instigator->Controller, instigator, 
										UBaseDamage::StaticClass());
				
				//GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow, "HIT WORLD"); 
				
				MachineGunHitWorld(HitResult);

				
				// Apply Force to World

				UActorComponent* ActorComponent = HitActor->GetComponentByClass(UPrimitiveComponent::StaticClass());

				if (!ActorComponent) return;
			
				UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(ActorComponent);

				if (!PrimitiveComponent) return;
			
				PrimitiveComponent->AddImpulse(Direction * WeaponData->ImpulseStrength);  
			}
			
		}
	}
	
}

void AMachineGun::BeginPlay()
{
	Super::BeginPlay();
 
	// Reset Weapon Data to 100 
	WeaponData->CurrentAmmo = 100; 
	WeaponData->Magazine = 100;  
}




