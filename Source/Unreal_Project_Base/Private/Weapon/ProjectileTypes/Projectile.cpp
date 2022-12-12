// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileTypes/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "Interfaces/BaseDamage.h"
#include "Interfaces/DamagingInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Mech/MechEnemy.h"
#include "Mech/MechPlayer.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE COMPONENT"));
	SetRootComponent(SphereComponent);
	
	// Components Initialize
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	ProjectileMesh->SetupAttachment(SphereComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE MOVEMENT"));

	// Initial Projectile movement numbers
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->MaxSpeed = 5000.0f;
	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->Friction = 0;

	ProjectileMovement->bIsHomingProjectile = true;

	

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(4.0f); 

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileHit);

	APawn* instigator = GetInstigator();
	if (!instigator) return;

	if (AMechPlayer* PlayerMech = Cast<AMechPlayer>(instigator))
	{
		if (! PlayerMech->TargetActor) return;
		
		TargetEnemy = PlayerMech->TargetActor;
	}
	
}

void AProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetEnemy)
	{
		if (USceneComponent* Scene =  Cast<USceneComponent>(TargetEnemy->GetRootComponent()))
			ProjectileMovement->HomingTargetComponent = Scene;   
	}
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if (this == OtherActor) return;
	
	
	UWorld* world = GetWorld();
	if (!world) return; 

	APawn* instigator = GetInstigator();
	if (!instigator) return;

	AActor* owner = GetOwner();
	if (!owner) return;

	if (OtherActor == owner) return;    
	
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(instigator);
	ignoreActors.Add(owner); 

	
	FVector Direction = OtherActor->GetActorLocation() - owner->GetActorLocation(); 

	Direction.Normalize();
	
	
	if(OtherActor->GetClass()->ImplementsInterface(UDamagingInterface::StaticClass()))
	{
		
		UGameplayStatics::ApplyPointDamage(OtherActor, PointDamageForMech, Direction, HitResult,
			instigator->GetController(), owner, UBaseDamage::StaticClass());

		if (OtherActor == instigator) return;  
		MechHit(HitResult);
		DestroyEvent();


		if (AMechEnemy* Enemy = Cast <AMechEnemy>(OtherActor))
		{
			if (AMechPlayer* Player = Cast <AMechPlayer>(instigator))
			{
				Enemy->HitByPlayer();
			}

			Enemy->HitByAnyone(); 
		}
	}
	else 
	{
		if (OtherActor->GetName() != LastBuildingName)
		{
			CurrentPenetratedBuildings ++;
			LastBuildingName = OtherActor->GetName();
		
			UGameplayStatics::ApplyPointDamage(OtherActor, PointDamageForWorld, Direction, HitResult,   
				instigator->GetController(), owner, UBaseDamage::StaticClass());
			WorldHit(HitResult);


			// Add impulse to World 
			 
			// UActorComponent* ActorComponent = OtherActor->GetComponentByClass(UPrimitiveComponent::StaticClass());
	
			// if (!ActorComponent) return;
	
			// UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(ActorComponent);
			
			// if (!PrimitiveComponent) return;
	
			// PrimitiveComponent->AddImpulse(Direction * ImpulseStrength);    
		}
		
		
		if (CurrentPenetratedBuildings >= PenetrationDepth)   
			DestroyEvent(); 
	} 
	
}

void AProjectile::WorldHit(FHitResult Hit)
{
}

void AProjectile::MechHit(FHitResult Hit) 
{
}


