// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttack2.h"

#include "AIController.h"
#include "EnemyBase.h"

UBTT_EnemyAttack2::UBTT_EnemyAttack2()
{
	NodeName = TEXT("Attack2");
}

EBTNodeResult::Type UBTT_EnemyAttack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	enemy->AttackPatten2();
	return EBTNodeResult::Succeeded;
}
