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

	FTimerHandle GroundNotchResetTimer;

	void DaggerAttack(FName SectionName);
	
	UFUNCTION()
		void AnimNotify_DaggerSlashEnd();
	UFUNCTION()
		void AnimNotify_ThrowDagger();
	UFUNCTION()
		void AnimNotify_InwardSlashStart();	
	UFUNCTION()
		void AnimNotify_InwardSlashEnd();	
	UFUNCTION()
		void AnimNotify_SlashGroundStart();
	UFUNCTION()
		void AnimNotify_SlashGroundNow();
	UFUNCTION()
		void AnimNotify_SlashGroundEnd();

};
