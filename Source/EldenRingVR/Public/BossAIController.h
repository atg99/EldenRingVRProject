// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API ABossAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ABossAIController();
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UBlackboardData* BBBoss;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UBehaviorTree* BTBoss;
	UPROPERTY()
		class UBlackboardComponent* BlackboardComp;

	static const FName TargetLocKey;
};
