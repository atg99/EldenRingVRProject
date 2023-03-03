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

	UPROPERTY()
		class ABoss* Boss;

	UPROPERTY()
		class ATPlayer* Target;

	UFUNCTION(BlueprintCallable)
		void LocationSet();
	UFUNCTION(BlueprintCallable)
		void JumpAttack(float time);
	UFUNCTION(BlueprintCallable)
		void TailAttack(float time);


	float Speed = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Decel = 0.00008f;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Excel = 0.0005f;
	float Rate = 0.1f;
	float Distance = 1100;

	FVector BossLocation;
	FVector TargetLocation;
	FRotator BossRotation;

	bool IsJumpAttack;
	bool IsLocationReset;
	
	int32 TailAttackCount = 0;
};
