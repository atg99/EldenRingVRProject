// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PCPlayer.generated.h"

UCLASS()
class ELDENRINGVR_API APCPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APCPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 스프링암
	UPROPERTY(EditAnywhere)
		class USpringArmComponent* springArmComp;
	// 카메라
	UPROPERTY(EditAnywhere)
		class UCameraComponent* cameraComp;
	//이동속도
	UPROPERTY(EditAnywhere)
		float moveSpeed = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputMappingContext* IMC_VRInput;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Move;
	// 이동 처리
	void Move(const FInputActionValue& Values);
	// 마우스 입력
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Mouse;
	// 마우스 회전
	void Turn(const FInputActionValue& Values);

	//점프 입력
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Jump;
	//점프 처리
	void onActionJump();
	// 공격입력
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Attack;
	//공격처리
	void Attack();
	//방어입력
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Defence;
	//방어처리
	void Defence();
	//대시입력
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Desh;
	//대시처리
	void Desh();

	//몽타주
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontageFactory;

	/*
	UFUNCTION(BlueprintCallable)
		void CallByBlueprint();
	UFUNCTION(BlueprintCallable)
		void OnAttack(FName sectionName);
	*/
	

	
};
