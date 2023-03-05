// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TEnemyAIController.h"
#include "SwordedEnemyAICon.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API ASwordedEnemyAICon : public ATEnemyAIController
{
	GENERATED_BODY()
public:
	ASwordedEnemyAICon();
protected:

	virtual void BeginPlay() override;
	
	void OnTargetUpdate(AActor* Actor, FAIStimulus Stimulus) override;

public:
	virtual void Tick(float DeltaTime) override;
};
