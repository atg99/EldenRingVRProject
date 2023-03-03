// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttackPattern.h"
#include "Boss.h"
#include "TPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UBossAttackPattern::UBossAttackPattern()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
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
	
	TailAttack(DeltaTime);
}

void UBossAttackPattern::LocationSet()
{
	BossLocation = Boss->GetActorLocation();
	TargetLocation = Target->GetActorLocation();
}

void UBossAttackPattern::JumpAttack(float time)
{
	
	if (Speed > 0.0f && Rate < 0.7f)
	{
		Speed -= Decel;
		Rate += Speed;
		Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, TargetLocation.X, Rate), UKismetMathLibrary::Lerp(BossLocation.Y, TargetLocation.Y, Rate), UKismetMathLibrary::Lerp(BossLocation.Z, BossLocation.Z + 720, 0.5 * Rate)));
		//UE_LOG(LogTemp, Warning, TEXT("%f"), Rate);
	}
	else
	{
		if (!IsLocationReset)
		{
			LocationSet();
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
		
	}

}


