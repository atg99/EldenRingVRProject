// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyBattleStart.h"

#include "AIController.h"
#include "EnemyBase.h"

UBTT_EnemyBattleStart::UBTT_EnemyBattleStart()
{
	NodeName = TEXT("Battle Start");
}

EBTNodeResult::Type UBTT_EnemyBattleStart::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	enemy->BattleStart();
	return EBTNodeResult::Succeeded;
}
