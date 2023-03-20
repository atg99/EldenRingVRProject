// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

UCLASS()
class ELDENRINGVR_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

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
	float WeponDamage = 60;

	UFUNCTION(BlueprintPure)
	float GetSwordSpeedfloat();

protected:

	UPROPERTY()
	class AEnemyBase* enemyBase;

	UPROPERTY()
	FVector prevPos;

	UPROPERTY(BlueprintReadOnly)
	FVector swordSpeed;

	UFUNCTION()
	void GetSwordSpeed();

	float AttackCoolTime;

};
