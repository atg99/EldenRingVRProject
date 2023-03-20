// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"
#include "Boss.h"
#include "BossFSM.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"


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
	Boss->BossFSM->IsBackStep = true;

}

void UBossAnim::AnimNotify_ThrowDagger()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	Boss->BossFSM->DaggerAttackThrow3();
}

void UBossAnim::AnimNotify_InwardSlashStart()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	Boss->BossMaceAct->SetVisibility(true);
	Boss->CanHit = true;
	
}
void UBossAnim::AnimNotify_InwardSlashNow()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	Boss->BossFSM->BossAttackCameraShake(Boss->BossFSM->BossInwardSlashAttackCamShake, Boss->GetActorLocation());
}
void UBossAnim::AnimNotify_InwardSlashEnd()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	//Boss->BossMaceAct->SetVisibility(false);
	
	Boss->BossFSM->IsInwardSlash = false;
	Boss->CanHit = false;
	
}
void UBossAnim::AnimNotify_SlashGroundStart()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	Boss->BossMaceAct->SetVisibility(true);
	Boss->CanHit = true;
	
}
void UBossAnim::AnimNotify_SlashGroundNow()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	FRotator GroundNotchRot = FRotator(Boss->BossFSM->HeadToTargetR.Pitch, Boss->BossFSM->HeadToTargetR.Yaw + 90, Boss->BossFSM->HeadToTargetR.Roll);
	Boss->BossFSM->BossAttackCameraShake(Boss->BossFSM->BossSlashAttackCamShake, Boss->GetActorLocation());
	GetWorld()->GetTimerManager().ClearTimer(GroundNotchResetTimer);
	Boss->GroundNotch->SetActorLocationAndRotation(Boss->Mace->GetComponentLocation() + FVector(0, 0, 30), GroundNotchRot);
}
void UBossAnim::AnimNotify_SlashGroundEnd()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	//Boss->BossMaceAct->SetVisibility(false);
	Boss->BossFSM->IsSlashGround = false;
	Boss->CanHit = false;
	GetWorld()->GetTimerManager().SetTimer(GroundNotchResetTimer, Boss, &ABoss::GroundNotchDistReset, 5.0f);
}