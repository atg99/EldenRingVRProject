// Fill out your copyright notice in the Description page of Project Settings.


#include "TEnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig.h"

ATEnemyAIController::ATEnemyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	aiPercep = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	
	//aiBehavior = CreateDefaultSubobject<UBehaviorTree>(TEXT("aiBehavior"));
}

void ATEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	aiPercep->OnTargetPerceptionUpdated.AddDynamic(this, &ATEnemyAIController::OnTargetUpdate);

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (aiBehavior) {
		RunBehaviorTree(aiBehavior); 
		//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), playerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("EnemyStartLocation"), GetPawn()->GetActorLocation());
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
	
	if(GetPawn()->GetDistanceTo(playerPawn) < 250)
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("bClose"), true);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("bClose"), false);
	}
}

void ATEnemyAIController::OnTargetUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	//감지한 자극의 종류를 문자열로 저장한다
	FString s =UKismetSystemLibrary::GetClassDisplayName(UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus));
	//Stimulus.SetExpirationAge(3);
	//Stimulus.AgeStimulus()
	if(s == "AISense_Sight")
	{
		if (Stimulus.WasSuccessfullySensed()&& Actor->ActorHasTag(TEXT("Player"))) {
			//플레이어 감지가 참
			bPlayerDetacted = true;
			//if (GEngine)
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Detect")));
		
		}
		//플레이어가 아닌것을 보아도 유지
		else if(Stimulus.WasSuccessfullySensed() && !Actor->ActorHasTag(TEXT("Player")))
		{
			ResetPlayerValue();
		}
		//Stimulus.IsExpired() == true
		else {
			//뒤로 움직이는 동안은 초기화 안함
			if(bNoClear)
			{
				return;
			}
			ResetPlayerValue();
			//플레이어의 값을 초기화한다
			//FTimerHandle TimerHandle_ResetPlayerValue;
			//GetWorldTimerManager().SetTimer(TimerHandle_ResetPlayerValue, this, &ATEnemyAIController::ResetPlayerValue, 2.0f, false);
			//감지가 안되면 거짓이 된다
			//if (GEngine)
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("else")));
			//bPlayerDetacted = false;
		}
	}
	
}

void ATEnemyAIController::MoveToPlayer()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("moveto")));
	EPathFollowingRequestResult::Type result = MoveToActor(playerPawn);
	UE_LOG(LogTemp, Warning, TEXT("%s aaaaa"), playerPawn);
}

void ATEnemyAIController::StopMoveTo()
{
	StopMovement();
}

void ATEnemyAIController::ResetPlayerValue()
{
	GetBlackboardComponent()->ClearValue(TEXT("Player"));
	bPlayerDetacted = false;
}

void ATEnemyAIController::SetbHitValue()
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("bHit"), true);
}

void ATEnemyAIController::ClearbHitValue()
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("bHit"), false);
}

void ATEnemyAIController::SetbDieValue()
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("bDie"), true);
}

void ATEnemyAIController::NoClearTimer()
{
	FTimerHandle TimerHandle_NoClear;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle_NoClear, FTimerDelegate::CreateLambda([this]()->void {bNoClear = true;}), 4, false);
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle_NoClear, FTimerDelegate::CreateLambda([this]()->void {bNoClear = true;}), 4, false);
	GetWorldTimerManager().SetTimer(TimerHandle_NoClear, FTimerDelegate::CreateLambda([this]()->void {bNoClear = false;}), 4, false);
}

void ATEnemyAIController::SetbCrawl()
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("bCrawl"), true);
	UE_LOG(LogTemp, Warning, TEXT("aaa"));
}

void ATEnemyAIController::DisableBT()
{
	if (aiBehavior)
	{
		//abort();
		FString reason;
		BrainComponent->StopLogic(reason);
		
	}
}




