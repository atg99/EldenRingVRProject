// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

#include "EnemyBase.h"
#include "TEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	enemy = Cast<AEnemyBase>(TryGetPawnOwner());

	aICon = Cast<ATEnemyAIController>(enemy->GetController());
}

void UEnemyAnim::AnimNotify_StopMove()
{
	aICon->StopMoveTo();
}

void UEnemyAnim::AnimNotify_BlendT()
{
	bBlend = true;
}

void UEnemyAnim::AnimNotify_BlendF()
{
	bBlend = false;
}

void UEnemyAnim::AnimNotify_ClearbHit()
{
	aICon->GetBlackboardComponent()->SetValueAsBool(FName("bHit"), false);
}

void UEnemyAnim::AnimNotify_SetRagdoll()
{
	enemy->SetRagdoll();
}
