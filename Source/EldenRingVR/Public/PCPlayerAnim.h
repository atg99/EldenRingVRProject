// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PCPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API UPCPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
		
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontageFactory;
	UFUNCTION(BlueprintCallable)
	void OnAttack(FName sectionName);
	UFUNCTION(BlueprintCallable)
	void DoAttack();
	UFUNCTION(BlueprintCallable)
	void DoDefence();
	
};
