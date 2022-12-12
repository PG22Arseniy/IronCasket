// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "AI/blackboard_keys.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"


//enum class MACHINEGUN_ATTACK_TYPES; 

AEnemyAIController::AEnemyAIController(FObjectInitializer const & ObjectInitializer)
{
	bAttachToPawn = true;
	 
	AIBehaviorTreeComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BEHAVIOR TREE COMPONENT"));
	AIBlackBoardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BLACK BOARD COMPONENT"));

	SetupPerceptionSystem();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AIBlackBoardComp && AIBehaviourTree) 
	{
		Blackboard->InitializeBlackboard(*AIBehaviourTree->BlackboardAsset); 
	}
}

UBlackboardComponent* AEnemyAIController::GetBlackboard() const
{
	return AIBlackBoardComp; 
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!AIBehaviourTree)
	{
		return;
	}
	// Runs our AI Behaviour Tree 
	RunBehaviorTree(AIBehaviourTree);
	AIBehaviorTreeComp->StartTree(*AIBehaviourTree);
	
}

void AEnemyAIController::ForgetAboutLastKnownPosition()
{
	GetBlackboard()->SetValueAsBool(bb_keys::PlayerDetected, false);
	
}

void AEnemyAIController::StopAIBehaviour()
{
	AIBehaviorTreeComp->DestroyComponent(); 
}

void AEnemyAIController::ContinueAIBehaviour()
{
	//AIBehaviorTreeComp = FObjectInitializer::Get().CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BEHAVIOR TREE COMPONENT"));  
} 

void AEnemyAIController::SetupPerceptionSystem()
{
	// Create Sight Configuration
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT CONFIG"));
	SetPerceptionComponent(* CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	// Set Sight Configuration Values
	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.0f;
	SightConfig->PeripheralVisionAngleDegrees = 360.0f; 
	SightConfig->SetMaxAge(5.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	
	// Add Sight Configuration to perception component
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	
}

void AEnemyAIController::OnTargetDetected(AActor* TargetActor, FAIStimulus const Stimulus)  
{
	
}