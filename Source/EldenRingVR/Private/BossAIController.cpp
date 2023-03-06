// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackBoardData.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Boss.h"
#include "BossAttackPattern.h"
#include "TPlayer.h"



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
		
		OwnerBoss = Cast<ABoss>(InPawn);
	
		
	}
}

void ABossAIController::TargetSet()
{
	BlackboardComp->SetValueAsObject(TEXT("TargetKey"), OwnerBoss->BossPattern->Target);
}