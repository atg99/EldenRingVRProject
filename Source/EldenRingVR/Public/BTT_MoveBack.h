// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MoveBack.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API UBTT_MoveBack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_MoveBack();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
