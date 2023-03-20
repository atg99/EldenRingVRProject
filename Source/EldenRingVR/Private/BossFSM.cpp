// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
#include "Boss.h"
#include "VRPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Dagger.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "GameFramework/Pawn.h"
#include "BossAnim.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"

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

	ConstructorHelpers::FClassFinder<UCameraShakeBase>BossAttackCam(TEXT("/Script/Engine.Blueprint'/Game/TW/UI/BP_BossAttackCameraShake.BP_BossAttackCameraShake_C'"));
	if (BossAttackCam.Succeeded())
	{
		BossAttackCamShake = BossAttackCam.Class;
	}

	ConstructorHelpers::FClassFinder<UCameraShakeBase>BossSlashAttackCam(TEXT("/Script/Engine.Blueprint'/Game/TW/UI/BP_BossAttackCameraShake1.BP_BossAttackCameraShake1_C'"));
	if (BossSlashAttackCam.Succeeded())
	{
		BossSlashAttackCamShake = BossSlashAttackCam.Class;
	}
	ConstructorHelpers::FClassFinder<UCameraShakeBase>BossInwardSlashAttackCam(TEXT("/Script/Engine.Blueprint'/Game/TW/UI/BP_BossAttackCameraShake2.BP_BossAttackCameraShake2_C'"));
	if (BossInwardSlashAttackCam.Succeeded())
	{
		BossInwardSlashAttackCamShake = BossInwardSlashAttackCam.Class;
	}
	ConstructorHelpers::FClassFinder<UCameraShakeBase>BossTailAttackCam(TEXT("/Script/Engine.Blueprint'/Game/TW/UI/BP_BossAttackCameraShake3.BP_BossAttackCameraShake3_C'"));
	if (BossTailAttackCam.Succeeded())
	{
		BossTailAttackCamShake = BossTailAttackCam.Class;
	}

	

}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	Boss = Cast<ABoss>(GetOwner());
	Target = Cast<AVRPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	
}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsLevSeq)
	{
		return;
	}
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

	case EBossState::MoveLeft:
		MoveLeftState(DeltaTime);
		break;

	case EBossState::MoveRight:
		MoveRightState(DeltaTime);
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
	
	
  
	if (Target->GetDistanceTo(Boss) > 1500)
	{
		BState = EBossState::Move;
	}
	
	else if (Target->GetDistanceTo(Boss) < 1500 && 1000 < Target->GetDistanceTo(Boss))
	{
		if (IsFirst)
		{
			IsFirst = false;
			IsJumpAttack = true;
			BState = EBossState::JumpAttack;
			return;
		}
		
		int32 RandNum = FMath::RandRange(1, 3);
		if (RandNum == 1)
		{
			IsJumpAttack = true;
			BState = EBossState::JumpAttack;
		}
		else
		{
			ReservState = EBossState::Idle;
			BState = EBossState::MoveClose;
			
		}
	
	}
  
	else
	{
		if (FVector::DotProduct(Boss->GetActorForwardVector(), HeadToTargetV) < 0)
		{
			int32 RandNum = FMath::RandRange(1, 5);
			if (RandNum == 1)
			{
				if (Target->GetDistanceTo(Boss) <= 300)
				{
					IsTailAttack = true;
					Boss->CanHit = true;
					BState = EBossState::TailAttack;
				}
				else
				{	
					IsTailAttack = true;
					Boss->CanHit = true;
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
			int32 RandNum = FMath::RandRange(1, 5);
			if (TempNum == RandNum)
			{
				RandNum = (TempNum + 1) % 5;
			
			}
			if (RandNum == 1)
			{
				if (Target->GetDistanceTo(Boss) <= 300)
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
				if (Target->GetDistanceTo(Boss) <= 300)
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
				if (Target->GetDistanceTo(Boss) <= 300)
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
			else if (RandNum == 4)
			{
				if (Target->GetDistanceTo(Boss) <= 300)
				{
					IsTailAttack = true;
					Boss->CanHit = true;
					BState = EBossState::TailAttack;
				}
				else
				{
					IsTailAttack = true;
					Boss->CanHit = true;
					ReservState = EBossState::TailAttack;
					BState = EBossState::MoveClose;

				}
			}
			else
			{
				if (FVector::DotProduct(Boss->GetActorRightVector(), HeadToTargetV) > 0)
				{
					BState = EBossState::MoveLeft;
				}
				else
				{
					BState = EBossState::MoveRight;
				}


			}
			TempNum = RandNum;

		}
	}
  
	//Boss->Mace->SetRelativeRotation(FRotator(90, 0, 0));
}

void UBossFSM::WaitState()
{
	if (!IsTimerSet)
	{
		IsTimerSet = true;
		GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &UBossFSM::IdleSet, WaitTime);

		
	}
	
	//Boss->Mace->SetRelativeRotation(FRotator(-90, 0, 0));
	
}

void UBossFSM::IdleSet()
{
	if (Boss->BossMaceAct->IsVisible())
	{
		Boss->BossMaceAct->SetVisibility(false);
	}
	BState = EBossState::Idle;
	IsTimerSet = false;
	GetWorld()->GetTimerManager().ClearTimer(WaitTimer);
}



void UBossFSM::MoveState()
{
	if (Target->GetDistanceTo(Boss) <= 1500)
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
	if (Target->GetDistanceTo(Boss) <= 300)
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

void UBossFSM::MoveLeftState(float time)
{
	if (TempRate <= 1)
	{
		LocationSet();
		RotationSet();
		TempRate += time * 0.34f;
		Boss->AddMovementInput(-Boss->GetActorRightVector() * 0.4f);
		Boss->SetActorRotation(HeadToTargetR);
	}
	else
	{
		TempRate = 0;
		BState = EBossState::Idle;
	}

}
void UBossFSM::MoveRightState(float time)
{

	if (TempRate <= 1)
	{
		LocationSet();
		RotationSet();
		TempRate += time * 0.34f;
		Boss->AddMovementInput(Boss->GetActorRightVector() * 0.4f);
		Boss->SetActorRotation(HeadToTargetR);
	}
	else
	{
		TempRate = 0;
		BState = EBossState::Idle;
	}

}


void UBossFSM::AttackState()
{
	
}


void UBossFSM::JumpAttackState(float time)
{
	if (IsJumpAttack)
	{

		if (Speed > 0.0f && Rate < 0.5f)
		{
			JumpAnimNum = 1;
			Speed -= Decel;
			Rate += Speed;
			Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, TargetLocation.X, Rate), UKismetMathLibrary::Lerp   (BossLocation.Y, TargetLocation.Y, Rate), UKismetMathLibrary::Lerp(BossLocation.Z, BossLocation.Z + 3000, 0.5 * Rate)));
		}
		else
		{


			if (!IsLocationReset)
			{
				TempRate = Rate;
				LocationSet();
				RotationSet();
				IsLocationReset = true;
			}

			FVector HitLoc = TargetLocation - HeadToTargetV * 300;
			
			if (Rate - TempRate < 0.3)
			{
				JumpAnimNum = 2;
				Speed += 0.0005 * Excel;
				Rate += Speed;
				
				Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, HitLoc.X, Rate - 0.7f),		UKismetMathLibrary::Lerp(BossLocation.Y, HitLoc.Y, Rate - 0.7f), UKismetMathLibrary::Lerp(BossLocation.Z, TargetLocation.Z, Rate - 0.7f)));

			}
			else if (Rate - TempRate < 1)
			{
				Speed += Excel * 25;
				Rate += Speed;
				Boss->SetActorLocation(FVector(UKismetMathLibrary::Lerp(BossLocation.X, HitLoc.X, Rate - 0.7f),		UKismetMathLibrary::Lerp(BossLocation.Y, HitLoc.Y, Rate - 0.7f), UKismetMathLibrary::Lerp(BossLocation.Z, TargetLocation.Z, Rate - 0.7f)));
			}
			else
			{
				if (Target->GetDistanceTo(Boss) < 600)
				{
					Target->OnDamaged(10);
				}
			
				
				JumpAnimNum = 3;
				IsJumpAttack = false;
				IsLocationReset = false;
				FCollisionQueryParams Param;
				FHitResult HitInfo;
				Param.AddIgnoredActor(Boss);
				bool IsHit = GetWorld()->LineTraceSingleByChannel(HitInfo, Boss->GetActorLocation(), Boss->GetActorLocation() - Boss->GetActorUpVector() * 1000, ECollisionChannel::ECC_Visibility, Param);
				if (IsHit)
				{
					Boss->SpawnGroundAttackNotch(HitInfo.ImpactPoint, HeadToTargetR);
					BossAttackCameraShake(BossAttackCamShake, HitInfo.ImpactPoint);
				}
				
				Speed = 0.01f;
				Rate = 0.1f;
				TempRate = 0;
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
			if (!Boss->BossTailDust->IsVisible())
			{
				Boss->BossTailDust->SetVisibility(true);
				Boss->BossDust->SetVisibility(false);
				
			}
			Boss->AddActorLocalRotation(FRotator(0.0f, -1.0f, 0.0f));
			TailAttackCount++;
		}
		else if (TailAttackCount <= 50)
		{
			Boss->AddActorLocalRotation(FRotator(0.0f, -0.04f, 0.0f));
			BossAttackCameraShake(BossTailAttackCamShake, Boss->GetActorLocation());
			TailAttackCount++;
		}
		else if (TailAttackCount <= 110)
		{
			Boss->AddActorLocalRotation(FRotator(0.0f, 12.4333f, 0.0f));
			TailAttackCount++;
			if (Boss->CanHit && Boss->GetDistanceTo(Target) <= 300)
			{
				Boss->CanHit = false;
				Target->OnDamaged(5);
			}
		}
		else
		{
			Boss->BossTailDust->SetVisibility(false);
			Boss->BossDust->SetVisibility(true);

			TailAttackCount = 0;
			IsTailAttack = false;
			Boss->CanHit = false;
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
		Boss->AddMovementInput(Boss->GetActorForwardVector() * 0.7);
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
		GetWorld()->GetTimerManager().SetTimer(DaggerThrowTime, this, &UBossFSM::DaggerAttackThrow2, 0.5f, false);
	}


}
void UBossFSM::DaggerAttackThrow2()
{
	Boss->BossAnimInst->DaggerAttack(FName("Dagger"));
	Boss->CanHit = true;
	

}

void UBossFSM::DaggerAttackThrow3()
{
	Boss->CanHit = false;
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


void UBossFSM::BossAttackCameraShake(TSubclassOf<UCameraShakeBase> CamShake, FVector Loc)
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CamShake, Loc, 0, 1000, 1, false);

}