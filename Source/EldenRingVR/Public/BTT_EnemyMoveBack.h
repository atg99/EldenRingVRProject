// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EnemyMoveBack.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API UBTT_EnemyMoveBack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_EnemyMoveBack();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
