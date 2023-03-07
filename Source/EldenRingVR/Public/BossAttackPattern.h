// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossAttackPattern.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELDENRINGVR_API UBossAttackPattern : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossAttackPattern();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABoss* Boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATPlayer* Target;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADagger> DaggerFac;
	UPROPERTY(EditAnywhere)
	class ADagger* DaggerAct;

	UFUNCTION(BlueprintCallable)
		void LocationSet();
	UFUNCTION(BlueprintCallable)
		void RotationSet();
	UFUNCTION(BlueprintCallable)
		void JumpAttack(float time);
	UFUNCTION(BlueprintCallable)
		void TailAttack(float time);
	UFUNCTION(BlueprintCallable)
		void BackStep(float time);
	UFUNCTION(BlueprintCallable)
		void DaggerAttackThrow1();
	UFUNCTION(BlueprintCallable)
		void DaggerAttackThrow2();	
	UFUNCTION(BlueprintCallable)
		void DaggerAttackThrow3();
	
	

	float Speed = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Decel = 0.00008f;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Excel = 0.0005f;
	float Rate = 0.1f;
	float JumpAttackDistance = 1100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BackStepDistance = 1000;

	FVector BossLocation;
	FVector TargetLocation;
	FVector HeadToTargetV;
	FRotator BossRotation;
	FRotator TargetRotation;
	FRotator HeadToTargetR;
	FVector JumpAttackLoc;

	float Timer = 0;

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
	
	int32 TailAttackCount = 0;






};
