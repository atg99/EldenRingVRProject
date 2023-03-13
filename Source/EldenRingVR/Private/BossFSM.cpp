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
#include "Components/CapsuleComponent.h"

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

	case EBossState::Wait:
		WaitState();
		break;

	case EBossState::Move:
		MoveState();
		break;

	case EBossState::MoveClose:
		MoveCloseState();
		break;

	case EBossState::Attack:
		AttackState();
		break;

	case EBossState::TurnToT:
		TurnToTState();
		break;

	case EBossState::JumpAttack:
		JumpAttackState(DeltaTime);
		break;

	case EBossState::TailAttack:
		TailAttackState(DeltaTime);
		break;
		
	case EBossState::DaggerAttackThrow:
		DaggerAttackThrowState();
		break;

	case EBossState::InwardSlash:
		InwardSlashState();
		break;

	case EBossState::SlashGround:
		SlashGroundState();
		break;
	}

	if (IsBackStep)
	{
		Timer +=  DeltaTime;
		BackStep(Timer);
	}
}

void UBossFSM::IdleState()
{

	LocationSet();
	RotationSet();
	
	
  
	if (Target->GetDistanceTo(Boss) > 1000)
	{
		BState = EBossState::Move;
	}
	
	else if (Target->GetDistanceTo(Boss) < 1000 && 800 < Target->GetDistanceTo(Boss))
	{
		
		IsJumpAttack = true;
		BState = EBossState::JumpAttack;
	}
  
	else
	{
		if (FVector::DotProduct(Boss->GetActorForwardVector(), HeadToTargetV) < 0)
		{
			int32 RandNum = FMath::RandRange(1, 4);
			if (RandNum == 1)
			{
				if (Target->GetDistanceTo(Boss) <= 200)
				{
					IsTailAttack = true;
					BState = EBossState::TailAttack;
				}
				else
				{	
					IsTailAttack = true;
					ReservState = EBossState::TailAttack;
					BState = EBossState::MoveClose;
					
				}
			
			}
			else
			{
				BState = EBossState::TurnToT;
			}
		}
		else
		{
			int32 RandNum = FMath::RandRange(1, 3);
			if (RandNum == 1)
			{
				if (Target->GetDistanceTo(Boss) <= 200)
				{
					IsDaggerThrow = true;
					BState = EBossState::DaggerAttackThrow;
				}
				else
				{
					IsDaggerThrow = true;
					ReservState = EBossState::DaggerAttackThrow;
					BState = EBossState::MoveClose;
				}

			}
			else if (RandNum == 2)
			{
				if (Target->GetDistanceTo(Boss) <= 200)
				{
					IsInwardSlash = true;
					BState = EBossState::InwardSlash;
				}
				else
				{
					IsInwardSlash = true;
					ReservState = EBossState::InwardSlash;
					BState = EBossState::MoveClose;
				}

			}
			else if (RandNum == 3)
			{
				if (Target->GetDistanceTo(Boss) <= 200)
				{
					IsSlashGround = true;
					BState = EBossState::SlashGround;
				}
				else
				{
					IsSlashGround = true;
					ReservState = EBossState::SlashGround;
					BState = EBossState::MoveClose;
				}

			}

		}

	}
  
	Boss->Mace->SetRelativeRotation(FRotator(90, 0, 0));
}

void UBossFSM::WaitState()
{
	if (!IsTimerSet)
	{
		IsTimerSet = true;
		GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &UBossFSM::IdleSet, WaitTime);

		
	}
	Boss->Mace->SetRelativeRotation(FRotator(-90, 0, 0));
	
}

void UBossFSM::IdleSet()
{
	BState = EBossState::Idle;
	IsTimerSet = false;
	GetWorld()->GetTimerManager().ClearTimer(WaitTimer);
}



void UBossFSM::MoveState()
{
	if (Target->GetDistanceTo(Boss) <= 1000)
	{
		BState = EBossState::Idle;
	}

	else
	{
		LocationSet();
		RotationSet();

		Boss->AddMovementInput(HeadToTargetV);
		Boss->SetActorRotation(HeadToTargetR);
	}
}

void UBossFSM::MoveCloseState()
{
	if (Target->GetDistanceTo(Boss) <= 200)
	{
		BState = ReservState;
	}

	else
	{
		LocationSet();
		RotationSet();

		Boss->AddMovementInput(HeadToTargetV);
		Boss->SetActorRotation(HeadToTargetR);
	}
}


void UBossFSM::AttackState()
{
	
}


void UBossFSM::JumpAttackState(float time)
{
	if (IsJumpAttack)
	{

		if (Speed > 0.0f && Rate < 0.7f)
		{
			JumpAnimNum = 1;
			Speed -= Decel;
			Rate += Speed;
			Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, TargetLocation.X, Rate), UKismetMathLibrary::Lerp   (BossLocation.Y, TargetLocation.Y, Rate), UKismetMathLibrary::Lerp(BossLocation.Z, BossLocation.Z + 720, 0.5 * Rate)));
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
				JumpAnimNum = 2;
				Speed += 0.001 * Excel;
				Rate += Speed;
				Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, TargetLocation.X, Rate - 0.7f),		UKismetMathLibrary::Lerp(BossLocation.Y, TargetLocation.Y, Rate - 0.7f), UKismetMathLibrary::Lerp(BossLocation.Z, TargetLocation.Z, Rate - 0.7f)));

			}
			else if (Rate - 0.7f < 1)
			{
				Speed += Excel;
				Rate += Speed;
				Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, TargetLocation.X, Rate - 0.7f),		UKismetMathLibrary::Lerp(BossLocation.Y, TargetLocation.Y, Rate - 0.7f), UKismetMathLibrary::Lerp(BossLocation.Z, TargetLocation.Z, Rate - 0.7f)));
			}
			else
			{
				JumpAnimNum = 3;
				IsJumpAttack = false;
				IsLocationReset = false;
				Speed = 0.01f;
				Rate = 0.1f;
				BState = EBossState::Wait;
			}
		}
	}
	
}


void UBossFSM::TailAttackState(float time)
{
	if (IsTailAttack)
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
			BState = EBossState::Wait;
		}
	
	}
	

}

void UBossFSM::BackStep(float time)
{
	if (!IsLocationReset || !IsRotationReset)
	{
		LocationSet();
		RotationSet();
		
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
	HeadToTargetV = (TargetLocation - BossLocation).GetSafeNormal();

}

void UBossFSM::RotationSet()
{
	BossRotation = Boss->GetActorRotation();
	TargetRotation = Target->GetActorRotation();
	HeadToTargetR = FRotator(0, HeadToTargetV.Rotation().Yaw, 0);
}

void UBossFSM::TurnToTState()
{
	if (TurnToTCount == 0)
	{
		LocationSet();
		RotationSet();
		TurnToTCount++;
	}
	
	else if (TurnToTCount * 0.007 < 1)
	{
		Boss->SetActorRotation(UKismetMathLibrary::RLerp(BossRotation, HeadToTargetR, 0.007 * TurnToTCount, true));
		TurnToTCount++;
	}

	else
	{
		TurnToTCount = 0;
		BState = EBossState::Idle;
	}

}

void UBossFSM::DaggerAttackThrowState()
{
	if (IsDaggerThrow)
	{
		IsDaggerThrow = false;
		Boss->BossAnimInst->DaggerAttack(FName("DaggerStart"));
		Boss->Dagger->SetVisibility(true);
		Boss->DaggerComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		FTimerHandle DaggerThrowTime;
		GetWorld()->GetTimerManager().SetTimer(DaggerThrowTime, this, &UBossFSM::DaggerAttackThrow2, 2.0f, false);
	}


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
	
	
	DaggerAct->DaggerThrow(HeadToTargetV, HeadToTargetR);
	BState = EBossState::Wait;

}

void UBossFSM::InwardSlashState()
{
	if (!IsInwardSlash)
	{
		BState = EBossState::Wait;
	
	}
}

void UBossFSM::SlashGroundState()
{
	if (!IsSlashGround)
	{
		BState = EBossState::Wait;
	}

}

