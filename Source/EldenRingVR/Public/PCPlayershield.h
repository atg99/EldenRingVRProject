	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PCPlayershield.generated.h"

UCLASS()
class ELDENRINGVR_API APCPlayershield : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APCPlayershield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponSettings)
		class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponSettings)
		class UStaticMeshComponent* meshComp;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		FTimerHandle lifeTimer;

	UFUNCTION()
		void WeaponTrace();

	UPROPERTY(EditAnywhere)
		class UBoxComponent* wStart;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* wEnd;

	UPROPERTY(EditAnywhere)
		float WeponDamage = 10;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY()
		class AEnemyBase* enemyBase;

	UPROPERTY()
		FVector prevPos;

	UPROPERTY()
		FVector shieldSpeed;

	UFUNCTION()
		void GetShieldSpeed();

	float AttackCoolTime = 1.5f;
	float ShieldTime = 1.5f;

};
