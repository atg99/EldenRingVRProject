// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
#include "Boss.h"
#include "TPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Dagger.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "GameFramework/Pawn.h"
#include "BossAnim.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UBossFSM::UBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<ADagger>BossDagger(TEXT("/Script/Engine.Blueprint'/Game/TW/Blueprint/BP_Dagger.BP_Dagger_C'"));
	if (BossDagger.Succeeded())
	{
		DaggerFac = BossDagger.Class;
	}
}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	Boss = Cast<ABoss>(GetOwner());
	Target = Cast<ATPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (BState)
	{
	case EBossState::Idle:
		IdleState();
		break;

	case EBossState::Move:
		MoveState();
		break;

	case EBossState::Attack:
		AttackState();
		break;


	}
}

void UBossFSM::IdleState()
{

}

void UBossFSM::MoveState()
{

}

void UBossFSM::AttackState()
{

}


void UBossFSM::JumpAttack(float time)
{

	if (Speed > 0.0f && Rate < 0.7f)
	{
		Speed -= Decel;
		Rate += Speed;
		Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, TargetLocation.X, Rate), UKismetMathLibrary::Lerp(BossLocation.Y, TargetLocation.Y, Rate), UKismetMathLibrary::Lerp(BossLocation.Z, BossLocation.Z + 720, 0.5 * Rate)));
	}
	else
	{
		if (!IsLocationReset)
		{
			LocationSet();
			RotationSet();
			IsLocationReset = true;
		}

		if (Rate - 0.7f < 0.05)
		{
			Speed += 0.001 * Excel;
			Rate += Speed;
			Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, TargetLocation.X, Rate - 0.7f), UKismetMathLibrary::Lerp(BossLocation.Y, TargetLocation.Y, Rate - 0.7f), UKismetMathLibrary::Lerp(BossLocation.Z, 0, Rate - 0.7f)));

		}
		else if (Rate - 0.7f < 1)
		{
			Speed += Excel;
			Rate += Speed;
			Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, TargetLocation.X, Rate - 0.7f), UKismetMathLibrary::Lerp(BossLocation.Y, TargetLocation.Y, Rate - 0.7f), UKismetMathLibrary::Lerp(BossLocation.Z, 0, Rate - 0.7f)));
		}
		else
		{
			IsJumpAttack = false;
			IsLocationReset = false;

		}
	}
}


void UBossFSM::TailAttack(float time)
{
	if (TailAttackCount <= 25)
	{
		Boss->AddActorLocalRotation(FRotator(0.0f, -1.0f, 0.0f));
		TailAttackCount++;
	}
	else if (TailAttackCount <= 50)
	{
		Boss->AddActorLocalRotation(FRotator(0.0f, -0.04f, 0.0f));

		TailAttackCount++;
	}
	else if (TailAttackCount <= 110)
	{
		Boss->AddActorLocalRotation(FRotator(0.0f, 12.4333f, 0.0f));
		TailAttackCount++;
	}
	else
	{
		TailAttackCount = 0;
		IsTailAttack = false;
	}


}

void UBossFSM::BackStep(float time)
{
	if (!IsLocationReset || !IsRotationReset)
	{
		LocationSet();
		RotationSet();
		HeadToTargetV = (Target->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal();
		HeadToTargetR = FRotator(0, HeadToTargetV.Rotation().Yaw, 0);
		IsLocationReset = true;
		IsRotationReset = true;
	}
	if (time * 3 < 1)
	{
		Boss->SetActorRotation(UKismetMathLibrary::RLerp(BossRotation, HeadToTargetR, time * 3, true));

	}
	if ((TargetLocation - Boss->GetActorLocation()).Length() < 800 || time * 1.5f < 1)
	{
		Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, (BossLocation - (HeadToTargetV * BackStepDistance)).X, time * 1.5f), UKismetMathLibrary::Lerp(BossLocation.Y, (BossLocation - (HeadToTargetV * BackStepDistance)).Y, time * 1.5f), UKismetMathLibrary::Lerp(BossLocation.Z, BossLocation.Z + 180, time * 1.5f)) - Boss->GetActorUpVector() * (time * 100) + HeadToTargetV * time * 300);
	}
	else
	{
		IsLocationReset = false;
		IsRotationReset = false;
		IsBackStep = false;
		Timer = 0;
	}
}

void UBossFSM::LocationSet()
{
	BossLocation = Boss->GetActorLocation();
	TargetLocation = Target->GetActorLocation();
}

void UBossFSM::RotationSet()
{
	BossRotation = Boss->GetActorRotation();
	TargetRotation = Target->GetActorRotation();
}

void UBossFSM::DaggerAttackThrow1()
{
	Boss->BossAnimInst->DaggerAttack(FName("DaggerStart"));
	Boss->Dagger->SetVisibility(true);
	FTimerHandle DaggerThrowTime;
	GetWorld()->GetTimerManager().SetTimer(DaggerThrowTime, this, &UBossFSM::DaggerAttackThrow2, 2.0f, false);

}
void UBossFSM::DaggerAttackThrow2()
{
	Boss->BossAnimInst->DaggerAttack(FName("Dagger"));

}

void UBossFSM::DaggerAttackThrow3()
{

	Boss->Dagger->SetVisibility(false);
	LocationSet();
	RotationSet();
	FActorSpawnParameters SpawnInfo;
	DaggerAct = GetWorld()->SpawnActor<ADagger>(DaggerFac, Boss->Dagger->GetComponentLocation(), Boss->Dagger->GetComponentRotation(), SpawnInfo);
	HeadToTargetV = (TargetLocation - BossLocation).GetSafeNormal();
	HeadToTargetR = FRotator(0, HeadToTargetV.Rotation().Yaw, 0);
	DaggerAct->DaggerThrow(HeadToTargetV, HeadToTargetR);
}
