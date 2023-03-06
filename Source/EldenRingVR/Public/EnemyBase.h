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

	UPROPERTY()
	bool bAttack;

	UFUNCTION()
	virtual void ChangeSpeed(float speed);

	UFUNCTION()
	void DeathAction();

	UFUNCTION()
	void SetRagdoll();
	
	UFUNCTION()
	void DamageReact_F();
	
	UFUNCTION()
	void AttackPatten1();

	UFUNCTION()
	void AttackPatten2();

	UFUNCTION()
	void BattleStart();

	//이 값이 변하면 이속 변경
	UPROPERTY()
	bool bDecrease;

	UPROPERTY()
	bool bIncrease;
	
	UFUNCTION()
	void DecreaseSpeed(float len, float speed);
	
	UFUNCTION()
	void IncreaseSpeed(float len, float speed);

	UFUNCTION(BlueprintCallable)
	void OnDamaged(float damage);
	
	UFUNCTION()
	void EnemyDie();

	UFUNCTION()
	void SetSwordDoOnce();

	UFUNCTION()
	void Dash(float force);
private:

	UPROPERTY()
	class ATEnemyAIController* con;
	
	UPROPERTY()
	class AEnemySword* sword;
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* enemyAnim;

	UPROPERTY()
	float time = 1;

	UPROPERTY()
	float eSpeed;

	UPROPERTY()
	float enemyHP = 100;
};


