// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class ELDENRINGVR_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* weapon;

	//UPROPERTY(EditAnywhere)
	//class UAIPerceptionComponent* aiPercep;

	//UPROPERTY(EditAnywhere)
	//class UBehaviorTree* behaviorTree;
	
	UPROPERTY(EditAnywhere)
	APawn* playerPawn;

	UFUNCTION()
	virtual void ChangeSpeed(float speed);
};
