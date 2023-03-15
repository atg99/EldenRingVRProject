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

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* originMesh;

	UPROPERTY(EditAnywhere)
	class UPoseableMeshComponent* poseableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* pmHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* pmBody;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* pmWaist;

	
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

	UFUNCTION()
	void PlayEnemyAnim(FName session, float rate);

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

	UFUNCTION(BlueprintCallable)
	void Desmemberment(FName hitBone, FVector hitLoc, FVector hitNoraml);

	UFUNCTION()
	void Crawl();

	UFUNCTION(BlueprintCallable)
	void EnemyMerge();

	UPROPERTY()
	bool bMerge;

	UPROPERTY()
	float mergeTime = 0;

	UPROPERTY()
	FTransform originTransform;

	UPROPERTY()
	FTransform curTransform ;

	UFUNCTION(BlueprintCallable)
	void SetPoseableMeshToGetMesh();
	
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

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* bloodDecal;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* mat;
	
	UFUNCTION()
	void EnemyMergeV2Setup();

	UFUNCTION()
	void EnemyMergeV2();

	UFUNCTION()
	void EnemyMergeEnd();
	
	UPROPERTY()
	bool bMergeV2;

	UPROPERTY()
	float mergeV2LerpTime;

	UPROPERTY()
	TArray<float> mergeDis;

	UPROPERTY()
	FTimerHandle timerHandle_Die;

	UFUNCTION()
	double EaseInElastic(double t);

	UFUNCTION()
	double EaseInOutBounce(double t);

	UFUNCTION()
	double EaseInBounce(double t);
};


