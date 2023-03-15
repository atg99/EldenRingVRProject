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

	UPROPERTY(EditAnywhere)
	class UAIPerceptionComponent* aiPercep;

	UPROPERTY(EditAnywhere)
	APawn* playerPawn;

	UPROPERTY()
	bool bPlayerDetacted = false;

	UPROPERTY()
	class UBehaviorTree* BT;

	UPROPERTY()
	class UBlackboardData* BB;

private:

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere) 
	class UBehaviorTree* aiBehavior;

	UPROPERTY()
	bool bHit = false;

	UPROPERTY()
	bool bNoClear;
	
	UFUNCTION()
	virtual void OnTargetUpdate(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void MoveToPlayer();

	UFUNCTION()
	void StopMoveTo();

	UFUNCTION()
	void ResetPlayerValue();

	UFUNCTION()
	void SetbHitValue();
	
	UFUNCTION()
	void ClearbHitValue();

	UFUNCTION()
	void SetbDieValue();

	UFUNCTION()
	void ClearbDieValue();

	UFUNCTION()
	void NoClearTimer();

	UFUNCTION()
	void SetbCrawl();

	UFUNCTION()
	void DisableBT();

	
	
};
