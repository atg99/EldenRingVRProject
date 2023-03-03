// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API ATEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATEnemyAIController();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere) 
	class UBehaviorTree* aiBehavior;

	UPROPERTY(EditAnywhere)
	class UAIPerceptionComponent* aiPercep;

public:
	UFUNCTION()
	void OnTargetUpdate(AActor* Actor, FAIStimulus Stimulus);
};
