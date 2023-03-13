// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dagger.generated.h"

UCLASS()
class ELDENRINGVR_API ADagger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADagger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* DaggerComp;
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* DaggerCapComp;

	class ABoss* PostActor;

	UFUNCTION()
	void OnDaggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DaggerThrow(FVector vec, FRotator rot);
	void DaggerDestroy();
	

	bool IsDaggerThrow;
	FVector ToTargetV;
	FRotator ToTargetR;

	bool CanAttack;
};
