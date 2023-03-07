// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"
#include "Boss.h"
#include "BossAttackPattern.h"

UBossAnim::UBossAnim()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> DaggerMont(TEXT("/Script/Engine.AnimMontage'/Game/TW/Animation/DaggerAttack.DaggerAttack'"));
	if (DaggerMont.Succeeded())
	{
		DaggerAnim = DaggerMont.Object;
	}
}

void UBossAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();


}

void UBossAnim::DaggerAttack(FName SectionName)
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	Boss->PlayAnimMontage(DaggerAnim, 1, SectionName);
}

void UBossAnim::AnimNotify_DaggerSlashEnd()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	Boss->BossPattern->IsBackStep = true;
}

void UBossAnim::AnimNotify_ThrowDagger()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	Boss->BossPattern->DaggerAttackThrow3();
}