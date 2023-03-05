// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EnemyOnDamaged.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API UBTT_EnemyOnDamaged : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_EnemyOnDamaged();
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
