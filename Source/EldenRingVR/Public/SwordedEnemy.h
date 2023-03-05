// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "SwordedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API ASwordedEnemy : public AEnemyBase
{
	GENERATED_BODY()
public:
	ASwordedEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	
	virtual void ChangeSpeed(float speed) override;
};
