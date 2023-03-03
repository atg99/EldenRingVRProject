// Fill out your copyright notice in the Description page of Project Settings.


#include "TEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"

ATEnemyAIController::ATEnemyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	aiPercep = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	
}

void ATEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	aiPercep->OnTargetPerceptionUpdated.AddDynamic(this, &ATEnemyAIController::OnTargetUpdate);

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (aiBehavior) {
		RunBehaviorTree(aiBehavior); 
		//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), playerPawn->GetActorLocation());
		//GetBlackboardComponent()->SetValueAsVector(TEXT("EnemyStartLocation"), GetPawn()->GetActorLocation());
		
	}
}

void ATEnemyAIController::OnTargetUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed()&& Actor->ActorHasTag(TEXT("Player"))) {
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), Actor->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("lastSeenPlayerLoc"), Actor->GetActorLocation());
	}
	else {
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	}
}


