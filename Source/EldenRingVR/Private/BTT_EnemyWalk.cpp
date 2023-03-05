
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyWalk.h"
#include "EnemyBase.h"
#include "AIController.h"
#include "EditorCategoryUtils.h"
#include "FrameTypes.h"

UBTT_EnemyWalk::UBTT_EnemyWalk()
{
	NodeName = TEXT("Walk");
}

EBTNodeResult::Type UBTT_EnemyWalk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	enemy->ChangeSpeed(300);
	
	
	//x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
	return EBTNodeResult::Succeeded;
}
