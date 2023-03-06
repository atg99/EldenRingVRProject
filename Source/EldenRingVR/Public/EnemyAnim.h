// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBlend = false;
private:

	UPROPERTY()
	class AEnemyBase* enemy;

	UPROPERTY()
	class ATEnemyAIController* aICon;
	
	UFUNCTION()
	void AnimNotify_StopMove();
	
	UFUNCTION()
	void AnimNotify_BlendT();
	
	UFUNCTION()
	void AnimNotify_BlendF();

	UFUNCTION()
	void AnimNotify_ClearbHit();
	
	UFUNCTION()
	void AnimNotify_SetRagdoll();

	UFUNCTION()
	void AnimNotify_AttackT();

	UFUNCTION()
	void AnimNotify_AttackF();

	UFUNCTION()
	void AnimNotify_Dash();
};
