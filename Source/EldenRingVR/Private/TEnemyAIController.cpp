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

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (aiBehavior) {
		RunBehaviorTree(aiBehavior); 
		//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), playerPawn->GetActorLocation());
		//GetBlackboardComponent()->SetValueAsVector(TEXT("EnemyStartLocation"), GetPawn()->GetActorLocation());
		
	}
}

void ATEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// if (GEngine)
	 //	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Detect")));
	//플레이어를 감지 했다면
	if(bPlayerDetacted)
	{
		//플레이어의 마지막 위치를 업데이트한다
		GetBlackboardComponent()->SetValueAsVector(TEXT("lastSeenPlayerLoc"), playerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject(FName("Player"), playerPawn);
		GetPawn()->SetActorRotation(FRotator(0,(playerPawn->GetActorLocation()-GetPawn()->GetActorLocation()).Rotation().Yaw,0));
	}
	

}

void ATEnemyAIController::OnTargetUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed()&& Actor->ActorHasTag(TEXT("Player"))) {
		//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), Actor->GetActorLocation());
		//플레이어 감지가 참
		bPlayerDetacted = true;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Detect")));
		
	}
	else {
		//플레이어의 값을 초기화한다
		GetBlackboardComponent()->ClearValue(TEXT("Player"));
		//감지가 안되면 거짓이 된다
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("else")));
		bPlayerDetacted = false;
	}
}


