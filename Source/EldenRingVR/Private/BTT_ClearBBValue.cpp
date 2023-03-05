// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ClearBBValue.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_ClearBBValue::UBTT_ClearBBValue()
{
	NodeName = TEXT("Clear BB Value");
}

EBTNodeResult::Type UBTT_ClearBBValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//OwnerComp.GetBlackboardComponent()->ClearValue(BlackboardKey.SelectedKeyName);
	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
}
