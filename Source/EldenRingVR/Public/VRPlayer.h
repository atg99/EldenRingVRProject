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

	//�̵��ӵ�
	UPROPERTY(EditAnywhere)
		float moveSpeed = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputMappingContext* IMC_VRInput;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Move;
	// �̵� ó��
	void Move(const FInputActionValue& Values);
public:
	// ���콺 �Է�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Mouse;
	// ���콺 ȸ��
	void Turn(const FInputActionValue& Values);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputMappingContext* IMC_Hand;

protected:
	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
		class UCameraComponent* VRCamera;
	// VR��Ʈ�ѷ�
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class UMotionControllerComponent* LeftHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class UMotionControllerComponent* RightHand;
	// ����� �� ��
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "MotionController")
		class USkeletalMeshComponent* LeftHandMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController")
		class USkeletalMeshComponent* RightHandMesh;
	
	// ����
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* weponMeshComp;
	// ����
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* shieldMeshComp;
	
public:	
	
	// �����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int maxHP = 100;
	// ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int maxMP = 100;
	// ���׹̳�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int maxStamina = 100;

	// ȸ�� �����ϱ�
	UPROPERTY(VisibleAnywhere, Category = "Rolling")
		class UStaticMeshComponent* RollingCircle;
	// ȸ�� Ȱ��ȭ ����
	bool bRolling = false;
	// ȸ�� ��ư ��������
	void RollingStart(const FInputActionValue& Values);
	// ȸ�� ��ư ������
	void RollingEnd(const FInputActionValue& Values);
	// ȸ�� ��� ����
	bool ResetRolling();
	// ���� ȸ�� ó���ϱ�
	void DrawRollingStraight();
	// ȸ�Ǽ��� �浹üũ �Լ�
	bool CheckHitRolling(FVector LastPos, FVector& CurPos);
	// �浹 ó�� �Լ�
	bool HitTest(FVector LastPos, FVector CurPos, FHitResult& HitInfo);
	// ȸ�� �Է¾׼�
		UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Rolling;
	// �ڷ���Ʈ�� ��ġ
	FVector RollingLocation;
	
private:
	// ȸ�� ��뿩��
	UPROPERTY(EditAnywhere, Category = "Rolling", meta = (AllowPrivateAccess = true))
		bool IsRolling = true;

	// Ÿ�̸�
	UPROPERTY()
		FTimerHandle RollingHandle;
	// ����ð�
	UPROPERTY()
		float CurTime = 0;
	// ������ �� �ɸ� �ð�
	UPROPERTY(EditAnywhere, Category = "Rolling", meta = (AllowPrivateAccess = true))
		float RollingTime = 0.4f;

	// ȸ�� ������ �Լ�
	void DoRolling();
	//���� �Է�
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Jump;
	//���� ó��
	void onActionJump();

	void DoAttack();
	void DoDefence();
};
