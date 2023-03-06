// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttackPattern.h"
#include "Boss.h"
#include "TPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "BossAIController.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackBoardData.h"
#include "BehaviorTree/BlackBoardComponent.h"


// Sets default values for this component's properties
UBossAttackPattern::UBossAttackPattern()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	
	
}

void UBossAttackPattern::InitializeComponent()
{
	Super::InitializeComponent();

	Boss = Cast<ABoss>(GetOwner());

	

}


// Called when the game starts
void UBossAttackPattern::BeginPlay()
{
	Super::BeginPlay();

	Target = Cast<ATPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	BossLocation = Boss->GetActorLocation();
	TargetLocation = Target->GetActorLocation();

	BossRotation = Target->GetActorRotation();
	



}


// Called every frame
void UBossAttackPattern::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (IsJumpAttack)
	{
		JumpAttack(DeltaTime);
	}
	
	if (IsTailAttack)
	{
		TailAttackCount++;
		TailAttack(DeltaTime);
	}
	
	if (IsBackStep)
	{
		Timer += DeltaTime;
		BackStep(Timer);
	}
}

void UBossAttackPattern::LocationSet()
{
	BossLocation = Boss->GetActorLocation();
	TargetLocation = Target->GetActorLocation();
}

void UBossAttackPattern::RotationSet()
{
	BossRotation = Boss->GetActorRotation();
	TargetRotation = Target->GetActorRotation();
}

void UBossAttackPattern::JumpAttack(float time)
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


void UBossAttackPattern::TailAttack(float time)
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

void UBossAttackPattern::BackStep(float time)
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
		Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, (BossLocation - (HeadToTargetV * BackStepDistance)).X, time * 1.5f), UKismetMathLibrary::Lerp(BossLocation.Y, (BossLocation - (HeadToTargetV * BackStepDistance)).Y, time * 1.5f), UKismetMathLibrary::Lerp(BossLocation.Z, BossLocation.Z + 180, time * 1.5f)) - Boss->GetActorUpVector()* (time * 100) + HeadToTargetV * time * 300);
	}
	else
	{
		IsLocationReset = false;
		IsRotationReset = false;
		IsBackStep = false;
		Timer = 0;
	}
}

