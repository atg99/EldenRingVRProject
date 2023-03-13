// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPlayerAnim.h"
#include "PCPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPCPlayerAnim::OnAttack(FName sectionName)
{
	//Montage_Play(AttackMontageFactory);
	APCPlayer* owner = Cast<APCPlayer>(TryGetPawnOwner());
	owner->PlayAnimMontage(AttackMontageFactory, 1, sectionName);
}

void UPCPlayerAnim::DoAttack()
{
	APCPlayer* owner = Cast<APCPlayer>(TryGetPawnOwner());
	owner->OnMyAttack();
}

void UPCPlayerAnim::DoDefence()
{
	APCPlayer* owner = Cast<APCPlayer>(TryGetPawnOwner());
	owner->OnMyDefence();
}
	

