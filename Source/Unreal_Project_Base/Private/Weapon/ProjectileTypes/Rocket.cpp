// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileTypes/Rocket.h"
#include "Kismet/GameplayStatics.h"
#include "Chaos/ChaosEngineInterface.h" 
#include "Interfaces/BaseDamage.h"


void ARocket::WorldHit(FHitResult Hit)
{
	Super::WorldHit(Hit);

	RocketHitWorld(Hit);

	UActorComponent* ActorComponent = Hit.Actor->GetComponentByClass(UPrimitiveComponent::StaticClass());

	if (!ActorComponent) return;
			
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(ActorComponent);

	if (!PrimitiveComponent) return;
			
	PrimitiveComponent->AddRadialImpulse(Hit.Location, RadiusForImpulse, StrengthForImpulse, ERadialImpulseFalloff::RIF_Constant, false );

	const TArray<AActor*> ActorsToIgnore;
	
	UGameplayStatics::ApplyRadialDamage(this, StrengthForImpulse, Hit.Location,  RadiusForImpulse, UBaseDamage::StaticClass(),ActorsToIgnore); 
	
}

void ARocket::MechHit(FHitResult Hit)
{
	Super::MechHit(Hit);

	RocketHitMech(Hit);
}

void ARocket::Destroyed() 
{
	Super::Destroyed();

	RocketExplosionInAir(); 
}


