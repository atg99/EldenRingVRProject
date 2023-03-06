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
	// ��������
	UPROPERTY(EditAnywhere)
		class USpringArmComponent* springArmComp;
	// ī�޶�
	UPROPERTY(EditAnywhere)
		class UCameraComponent* cameraComp;
	//�̵��ӵ�
	UPROPERTY(EditAnywhere)
		float moveSpeed = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputMappingContext* IMC_VRInput;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Move;
	// �̵� ó��
	void Move(const FInputActionValue& Values);
	// ���콺 �Է�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Mouse;
	// ���콺 ȸ��
	void Turn(const FInputActionValue& Values);
		

};