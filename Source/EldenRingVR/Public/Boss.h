// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss.generated.h"

UCLASS()
class ELDENRINGVR_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* Dagger;
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* BossDust;
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* BossTailDust;
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* BossMaceAct;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mace;
	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* DaggerComp;
	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* MaceComp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> GroundNotchFac;
	UPROPERTY(EditAnywhere)
		class AActor* GroundNotch;


	UPROPERTY()
		class UBossAnim* BossAnimInst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
		class UBossFSM* BossFSM;

	UFUNCTION()
		void OnDaggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnMaceBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHP = 10000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurHP = MaxHP;

	bool CanHit;

	UFUNCTION()
		void OnDamaged(float damage);

	void GroundNotchDistReset();
	
};
