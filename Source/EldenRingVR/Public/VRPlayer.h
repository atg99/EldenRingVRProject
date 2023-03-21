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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Interaction;
	
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


	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
		class UCameraComponent* VRCamera;
protected:
	// VR컨트롤러
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class UMotionControllerComponent* LeftHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
		class UMotionControllerComponent* RightHand;

	// ����� �� ��
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "MotionController")
		class USkeletalMeshComponent* LeftHandMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController")
		class USkeletalMeshComponent* RightHandMesh;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* playerSword;
	


public:	

	// 무기 메쉬
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* weponMeshComp;
	// 방패 메쉬
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* shieldMeshComp;
	
public:	
	
	// 생명력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxHP = 200;

	// 마나
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxMP = 100;

	// 스테미나
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxStamina = 100;

	//지력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int INT = 100;

	// 힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int STR = 100;

public:	// 회피 시작하기

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
	UFUNCTION()
	void onActionJump();
	
	

	UFUNCTION()
	void Interact();

	UFUNCTION()
	void InteractEnd();

	UPROPERTY(EditAnywhere)
	class UWidgetInteractionComponent* interactionComp;

	UPROPERTY(EditAnywhere)
	class UWidgetInteractionComponent* interactionComp1;

	//UPROPERTY()
	//TSubclassOf<class APlayerStatActor> statActor;

public:

	UPROPERTY(BlueprintReadOnly)
	class ABoss* Boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BossHPFac;
	
	UPROPERTY()
	AActor* BossHP;

	UPROPERTY()
	TSubclassOf<AActor> GameOverUIFac;
	UPROPERTY()
	TSubclassOf<AActor> ClearUIFac;
	UPROPERTY()
	AActor* EndingUI;
	

	bool IsBossLev;
	bool IsEndingUISpawn;
	bool IsBossDie;


public:
	UPROPERTY()
	class APlayerStatActor* statWindow;
	
	UPROPERTY()
	bool bStatInteraction;
	
	UFUNCTION(BlueprintCallable)
	void OnDamaged(float damage);

	UPROPERTY()
	class AFireCamp* savePoint;
	
	UPROPERTY()
	FString PlayerStartLocation;

	UPROPERTY()
	FVector PlayerStartVec;

	UPROPERTY()
	FString startLoc = FString("../../../Content/SaveFiles/startLoc.txt");

	UPROPERTY(EditAnywhere)
	class UMotionControllerComponent* rightAim;
	UPROPERTY(EditAnywhere)
	class UMotionControllerComponent* leftAim;
	
	//잡기 버튼을 누르면 물체를 잡고싶다.
	// 필요속성: 입력액션,잡을범위
	// 잡을 범위
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
		float GrabRange = 100;

	//오른손
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_rGrab;
	// 잡은 물체 기억
	UPROPERTY()
	class UPrimitiveComponent* rGrabbedObject;

	UPROPERTY()
	class APlayerWeapon* GrabbedWeapon;

	UPROPERTY()
	class APCPlayershield* GrabbedShield;

	// 잡은 녀석이 있는지 여부 기억할 변수
	bool rIsGrabbed = false;
	// 잡기 시도
	void rTryGrab();
	// 놓기
	void rUnTryGrab();
	//잡고 있는중
	void rGrabbing();
	// Release 상태로 되돌려놓기
	void rReleaseUIInput();

	//왼손
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_lGrab;
	// 잡은 물체 기억
	UPROPERTY()
		class UPrimitiveComponent* lGrabbedObject;
	// 잡은 녀석이 있는지 여부 기억할 변수
	bool lIsGrabbed = false;
	// 잡기 시도
	void lTryGrab();
	// 놓기
	void lUnTryGrab();
	//잡고 있는중
	void lGrabbing();
	// Release 상태로 되돌려놓기
	void lReleaseUIInput();

	UFUNCTION()
	void UploadMoneyFile();
	
	UPROPERTY()
	FString PlayerMoneyFile = FString("../../../Content/SaveFiles/PlayerMoneyFile.txt");
	
	UPROPERTY(EditAnywhere, Category=PlayerStatus)
	int32 PlayerMoney = 0;
	
	//돈을 얻는다
	UFUNCTION()
	void GetMoney(int32 income);

	UPROPERTY()
	int32 playerLevel = 1;

	UPROPERTY()
	FString playerLevelFile = FString("../../../Content/SaveFiles/PlayerLevelFile.txt");
	
	void PoWStart();
	void PoWEnd();

	float PoWTime = 1.5f;
};
