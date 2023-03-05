// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveBack.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_MoveBack::UBTT_MoveBack()
{
	NodeName = TEXT("Step Back");
}

EBTNodeResult::Type UBTT_MoveBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("player"));
	return  EBTNodeResult::Succeeded;
}
