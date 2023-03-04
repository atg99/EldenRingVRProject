// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackBoardData.h"
#include "BehaviorTree/BlackBoardComponent.h"


const FName ABossAIController::TargetLocKey(TEXT("TargetLoc"));

ABossAIController::ABossAIController()
{
	ConstructorHelpers::FObjectFinder<UBlackboardData>BBD(TEXT("/Script/AIModule.BlackboardData'/Game/TW/BehaviorTree/BB_Boss.BB_Boss'"));
	if (BBD.Succeeded())
	{
		BBBoss = BBD.Object;
	}
	ConstructorHelpers::FObjectFinder<UBehaviorTree>BT(TEXT("/Script/AIModule.BehaviorTree'/Game/TW/BehaviorTree/BT_Boss.BT_Boss'"));
	if (BT.Succeeded())
	{
		BTBoss = BT.Object;
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBBoss, BlackboardComp))
	{
		RunBehaviorTree(BTBoss);
		//BlackboardComp->SetValueAsVector(TargetLocKey, InPawn->GetActorLocation());
	}
	
}