// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/DebugCamera.h"

// Sets default values
ADebugCamera::ADebugCamera()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADebugCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADebugCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADebugCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

