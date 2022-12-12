// Fill out your copyright notice in the Description page of Project Settings.


#include "Limb/Limb.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h" 
#include "Interfaces/BaseDamage.h"

// Sets default values
ALimb::ALimb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SCENE"));
	SetRootComponent(SceneComp);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	BoxComp->SetupAttachment(SceneComp); 
	
	
}

// Called when the game starts or when spawned
void ALimb::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALimb::SetDamagePoint(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
	AActor* DamageCauser)
{
	SetDamage(Damage, HitLocation, DamageType, DamageCauser);
}

void ALimb::SetDamageRadial(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
	FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	SetDamage(Damage, HitInfo.Location, DamageType, DamageCauser);
}

void ALimb::SetDamage(float Damage, FVector HitLocation, const UDamageType* DamageType, AActor* DamageCauser)
{
	const UBaseDamage* BaseDamageType = Cast<UBaseDamage>(DamageType);
	if (BaseDamageType)
	{
		//float DamageMultiplier = BaseDamageType->ProcessDamage(DamageCauser, this->GetOwner(), HitLocation);
		Health -= Damage; 
	}

	if (Health <= 0)
	{
		Destroy();
		
	}
}

