// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttack1.h"

#include "AIController.h"
#include "EnemyBase.h"
#include "TEnemyAIController.h"

UBTT_EnemyAttack1::UBTT_EnemyAttack1()
{
	NodeName = "Enemy Attack1";
}

EBTNodeResult::Type UBTT_EnemyAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	enemy->AttackPatten1();
	
	auto con = Cast<ATEnemyAIController>(OwnerComp.GetAIOwner());
	con->MoveToPlayer();
	
	return EBTNodeResult::Succeeded;
}
