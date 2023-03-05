// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyDie.h"

#include "AIController.h"
#include "EnemyBase.h"

UBTT_EnemyDie::UBTT_EnemyDie()
{
	NodeName = TEXT("Death");
}

EBTNodeResult::Type UBTT_EnemyDie::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	enemy->DeathAction();
	return EBTNodeResult::Succeeded;
}
