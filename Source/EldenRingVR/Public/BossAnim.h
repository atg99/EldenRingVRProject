// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API UBossAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	UBossAnim();

	virtual void NativeBeginPlay() override;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* DaggerAnim;

	void DaggerAttack(FName SectionName);
	
	UFUNCTION()
		void AnimNotify_DaggerSlashEnd();
	UFUNCTION()
		void AnimNotify_ThrowDagger();
};
