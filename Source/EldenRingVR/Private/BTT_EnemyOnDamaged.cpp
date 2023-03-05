// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyOnDamaged.h"

#include "AIController.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_EnemyOnDamaged::UBTT_EnemyOnDamaged()
{
	NodeName = TEXT("OnDamaged");
}

EBTNodeResult::Type UBTT_EnemyOnDamaged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//예비용
	
	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	enemy->DamageReact_F();
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bHit"), false);
	return EBTNodeResult::Succeeded;
}
