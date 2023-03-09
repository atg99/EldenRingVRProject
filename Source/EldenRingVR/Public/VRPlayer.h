// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VRPlayer.generated.h"

UCLASS()
class ELDENRINGVR_API AVRPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

protected:
	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
		class UCameraComponent* VRCamera;
	// VR컨트롤러
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class UMotionControllerComponent* LeftHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class UMotionControllerComponent* RightHand;
	// 사용할 손 모델
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class USkeletalMeshComponent* LeftHandMesh;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class USkeletalMeshComponent* RightHandMesh;
	// 생명력
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int hp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int maxHp = 100;
};
