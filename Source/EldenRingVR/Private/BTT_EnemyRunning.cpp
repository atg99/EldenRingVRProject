// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyRunning.h"

#include "AIController.h"
#include "EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_EnemyRunning::UBTT_EnemyRunning()
{
	NodeName = TEXT("Run");
}

EBTNodeResult::Type UBTT_EnemyRunning::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if(OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	enemy->ChangeSpeed(600);

	return EBTNodeResult::Succeeded;
}
