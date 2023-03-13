// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossFSM.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle,
	Wait,
	Move,
	MoveClose,
	Attack,
	TurnToT,
	JumpAttack,
	TailAttack,
	DaggerAttackThrow,
	InwardSlash,
	SlashGround
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELDENRINGVR_API UBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABoss* Boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AVRPlayer* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EBossState BState = EBossState::Idle;

	EBossState ReservState;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ADagger> DaggerFac;
	UPROPERTY(EditAnywhere)
		class ADagger* DaggerAct;

	void IdleState();
	void WaitState();
	void MoveState();
	void MoveCloseState();
	
	void AttackState();

	UFUNCTION(BlueprintCallable)
		void LocationSet();
	UFUNCTION(BlueprintCallable)
		void RotationSet();
	UFUNCTION(BlueprintCallable)
		void TurnToTState();
	UFUNCTION(BlueprintCallable)
		void JumpAttackState(float time);
	UFUNCTION(BlueprintCallable)
		void TailAttackState(float time);
	UFUNCTION(BlueprintCallable)
		void BackStep(float time);
	UFUNCTION(BlueprintCallable)
		void DaggerAttackThrowState();
	UFUNCTION(BlueprintCallable)
		void DaggerAttackThrow2();
	UFUNCTION(BlueprintCallable)
		void DaggerAttackThrow3();	
	UFUNCTION(BlueprintCallable)
		void InwardSlashState();
	UFUNCTION(BlueprintCallable)
		void SlashGroundState();



	float Speed = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Decel = 0.00008f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Excel = 0.0005f;
	float Rate = 0.1f;
	float JumpAttackDistance = 1100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BackStepDistance = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int JumpAnimNum;

	FVector BossLocation;
	FVector TargetLocation;
	FVector HeadToTargetV;
	FRotator BossRotation;
	FRotator TargetRotation;
	FRotator HeadToTargetR;
	FVector JumpAttackLoc;

	float Timer = 0;
	float WaitTime = 3;
	bool IsTimerSet;
	FTimerHandle WaitTimer;
	void IdleSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsJumpAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsTailAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsBackStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsLocationReset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsRotationReset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDaggerThrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsInwardSlash;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsSlashGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsTurnToT;
	

	int32 TailAttackCount = 0;
	int32 TurnToTCount = 0;




		
};
