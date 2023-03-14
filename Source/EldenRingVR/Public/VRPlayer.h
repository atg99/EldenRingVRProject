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
public:
	// 마우스 입력
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Mouse;
	// 마우스 회전
	void Turn(const FInputActionValue& Values);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputMappingContext* IMC_Hand;

protected:
	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
		class UCameraComponent* VRCamera;
	// VR컨트롤러
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class UMotionControllerComponent* LeftHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class UMotionControllerComponent* RightHand;
	// 사용할 손 모델
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "MotionController")
		class USkeletalMeshComponent* LeftHandMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController")
		class USkeletalMeshComponent* RightHandMesh;
	
	// 무기
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* weponMeshComp;
	// 방패
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* shieldMeshComp;
	
public:	
	
	// 생명력
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int maxHP = 100;
	// 마나
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int maxMP = 100;
	// 스테미나
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int maxStamina = 100;

	// 회피 시작하기
	UPROPERTY(VisibleAnywhere, Category = "Rolling")
		class UStaticMeshComponent* RollingCircle;
	// 회피 활성화 여부
	bool bRolling = false;
	// 회피 버튼 눌렀을때
	void RollingStart(const FInputActionValue& Values);
	// 회피 버튼 땟을때
	void RollingEnd(const FInputActionValue& Values);
	// 회피 기능 리셋
	bool ResetRolling();
	// 직선 회피 처리하기
	void DrawRollingStraight();
	// 회피선과 충돌체크 함수
	bool CheckHitRolling(FVector LastPos, FVector& CurPos);
	// 충돌 처리 함수
	bool HitTest(FVector LastPos, FVector CurPos, FHitResult& HitInfo);
	// 회피 입력액션
		UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Rolling;
	// 텔레포트할 위치
	FVector RollingLocation;
	
private:
	// 회피 사용여부
	UPROPERTY(EditAnywhere, Category = "Rolling", meta = (AllowPrivateAccess = true))
		bool IsRolling = true;

	// 타이머
	UPROPERTY()
		FTimerHandle RollingHandle;
	// 경과시간
	UPROPERTY()
		float CurTime = 0;
	// 워프할 때 걸릴 시간
	UPROPERTY(EditAnywhere, Category = "Rolling", meta = (AllowPrivateAccess = true))
		float RollingTime = 0.4f;

	// 회피 수행할 함수
	void DoRolling();
	//점프 입력
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Jump;
	//점프 처리
	void onActionJump();

	void DoAttack();
	void DoDefence();
};
