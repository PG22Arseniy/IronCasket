// Fill out your copyright notice in the Description page of Project Settings.


#include "MechGameStateBase.h" 
#include "Mech/CannonEnemy.h"
#include "Mech/MinigunEnemy.h"
#include "AI/GroupAISubsystem.h"
#include "AI/AIControllers/CannonAIController.h"
#include "AI/AIControllers/MinigunAIController.h"
#include "Debug/LevelManager.h"
#include "Debug/SpawnPoint.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Mech/MechEnemy.h"
#include "Mech/MechPlayer.h"
#include "Mech/MinigunEnemy.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/ObjectPoolingSystem.h"

AMechGameStateBase::AMechGameStateBase()
{
}

void AMechGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevelInt = 0;
	CurrentLevelEnum = GAME_LEVEL::LEVEL1; 

	UWorld* World = GetWorld();
	if (!World) return;

	UObjectPoolingSystem* ObjectPool = GetGameInstance()->GetSubsystem<UObjectPoolingSystem>();

	for (int i = 0; i< MaxProjectileNumber; ++i)
	{
		AProjectile* projectile = World->SpawnActor<AProjectile>(ProjectileBP);

		ObjectPool->ReturnProjectile(projectile);  
	}

	LevelManager = Cast<ALevelManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelManager::StaticClass()));
}

void AMechGameStateBase::LoadLevel()
{

	const FName CurrentLevelName = LevelNames[CurrentLevelInt];

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Blue, CurrentLevelName.ToString()); 
	
	LevelLoad(CurrentLevelInt); 
}


void AMechGameStateBase::ReloadLevel()
{

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMechEnemy::StaticClass(), FoundActors);
	
	for (AActor* FoundActor : FoundActors)
	{
		if (AMechEnemy* Enemy = Cast<AMechEnemy>(FoundActor))
		{
			//Enemy->Destroy();   
		} 
	}
	
	LevelReload(CurrentLevelInt);   

	ChangePlayerLocation();   
}

void AMechGameStateBase::UnloadLevel()
{
	if (CurrentLevelInt>0)
	{
		LevelUnload(CurrentLevelInt-1); 
	}
}

void AMechGameStateBase::NextLevel()
{
	if (CurrentLevelInt >= 3) return;
	
	CurrentLevelInt ++;

	ChangeCurrentLevel();  

	LoadLevel();
}

void AMechGameStateBase::PreviousLevel()
{
	
	if (CurrentLevelInt <= 0) return;

	LevelUnload(CurrentLevelInt); 
	
	CurrentLevelInt--;

	ChangeCurrentLevel();

	LoadLevel(); 
}

void AMechGameStateBase::ChangeCurrentLevel()
{
	// Change Level Enum
	switch (CurrentLevelInt)
	{
	case 1:

		CurrentLevelEnum = GAME_LEVEL::LEVEL1;
		
		break; 

	case 2:

		CurrentLevelEnum = GAME_LEVEL::LEVEL2;
		
		break; 

	case 3:
		
		CurrentLevelEnum = GAME_LEVEL::LEVEL3;
		
		break; 

	case 4:

		CurrentLevelEnum = GAME_LEVEL::LEVEL4;
		
		break; 
	default: break; 
	}
}

void AMechGameStateBase::ChangePlayerLocation()
{

// Player to spawn point

	LevelManager = Cast<ALevelManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelManager::StaticClass()));

	AMechPlayer* Player = Cast<AMechPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	Player->Alive = true; 
	Player->PlayerReset();  
	
	if (!LevelManager) return; 

	if (LevelManager->SpawnPoints.Num()<= CurrentLevelInt )return; 
	if (LevelManager->SpawnPoints[CurrentLevelInt])
		
	Player->SetActorLocation(LevelManager->SpawnPoints[CurrentLevelInt]->GetActorLocation());   

	
}

void AMechGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FName CurrentLevelName = LevelNames[CurrentLevelInt];

//	GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaSeconds, FColor::Blue, CurrentLevelName.ToString());     

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaSeconds, FColor::Blue, FString::FromInt(CurrentLevelInt)); 

	
	UWorld* World = GetWorld();

	if (!World) return;


	
   

	// Find The Minigun Enemy To Cut Off the Player

	AMinigunEnemy* MinigunEnemy = World->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<UGroupAISubsystem>()->GetMinigunEnemyToCutOff();

	if (!MinigunEnemy) return; 
	
	//UKismetSystemLibrary::DrawDebugSphere(World, MinigunEnemy->GetActorLocation(), 500,12,FLinearColor::Green, 1,1);


	

}

void AMechGameStateBase::IncrementAliveEnemies()
{
	CurrentAliveEnemies ++;
}

void AMechGameStateBase::DecrementAliveEnemies()
{
	if (CurrentAliveEnemies <= 0) return;
	
	CurrentAliveEnemies --;
}

int32 AMechGameStateBase::GetNumberOfAliveEnemies()
{
	return CurrentAliveEnemies; 
}

void AMechGameStateBase::SpawnEnemies(int32 MinigunNumber, int32 CannonNumber, bool PlayerDetected)
{
	LevelManager = Cast<ALevelManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelManager::StaticClass()));
	
	for (int i =0; i< MinigunNumber; ++i)
	{
		if (LevelManager)
			LevelManager->SpawnMinigunEnemyAtRandomLoc(CurrentLevelInt, PlayerDetected);
	}

	for (int i =0; i< CannonNumber; ++i)
	{
		if (LevelManager) 
			LevelManager->SpawnCannonEnemyAtRandomLoc(CurrentLevelInt, PlayerDetected);  
	}
}


void AMechGameStateBase::FinishWaves()
{
	WaveNum = -1;
}

void AMechGameStateBase::StartWaves()
{
	WaveNum = 0;
	if (GetNumberOfAliveEnemies() == 0)
	{
		WaveNum ++;
		NewWave(WaveNum);   
	}
}

void AMechGameStateBase::FinishWave()
{
	WaveInProgress = false;
}

void AMechGameStateBase::ResetNumberOfAliveEnemies()
{
	CurrentAliveEnemies = 0;  
}

void AMechGameStateBase::GamePause(bool Paused)
{
	
	// If Game Is Paused All Enemies Are disabled

	if (!CannonEnemySubClass || !MinigunEnemySubClass) return;


	TArray<AActor*> CannonEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CannonEnemySubClass, CannonEnemies); 
	
	for (AActor* EnemyActor : CannonEnemies)  
	{
 		if (AMechEnemy* Enemy = Cast <AMechEnemy>(EnemyActor))
		{
 			
			ACannonAIController* CannonAIController =  Enemy->GetController<ACannonAIController>() ;

 			if (!CannonAIController) return;

 			if (Paused)
 			{
 				Enemy->GetCharacterMovement()->MaxWalkSpeed = 0;
 				Enemy->DisabledAI = true;  
 				CannonAIController->StopAIBehaviour(); 
 			}
 			else
 				CannonAIController->ContinueAIBehaviour();
		}
	}

	TArray<AActor*> MinigunEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), MinigunEnemySubClass, MinigunEnemies); 
	
	for (AActor* EnemyActor : MinigunEnemies)  
	{
		if (AMechEnemy* Enemy = Cast <AMechEnemy>(EnemyActor))
		{
			AMinigunAIController* MinigunAIController = Enemy->GetController<AMinigunAIController>() ;
			
			if (!MinigunAIController) return;
			
			if (Paused)
			{
				MinigunAIController->StopAIBehaviour();
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 0;
				Enemy->DisabledAI = true;   
			}
			else
				MinigunAIController->ContinueAIBehaviour();
		}
	}
}

	



void AMechGameStateBase::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	DOREPLIFETIME( AMechGameStateBase, WeaponCollection );
}
