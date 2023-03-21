// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "VRGameModeBase.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include <DrawDebugHelpers.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include <string>
#include <Components/CapsuleComponent.h>
#include "BossHP.h"
#include "FireCamp.h"
#include "PlayerStatActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Boss.h"
#include "EngineUtils.h"
#include "PlayerWeapon.h"
#include "Potion.h"
#include "PCPlayershield.h"

// Sets default values
AVRPlayer::AVRPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// VR카메라 생성
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bUsePawnControlRotation = true;
	VRCamera->SetRelativeLocation(FVector(0, 0, 70));
	// 손 추가
	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));
	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));

	//스켈레탈 메시 컴포넌트 만들기
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHand);
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHand);

	// 왼손 메시 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (TempMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(TempMesh.Object);
		LeftHandMesh->SetRelativeLocation(FVector(-2.9, -3.5, 4.5));
		LeftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
	}
	// 오른손 메시 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh2(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (TempMesh2.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(TempMesh2.Object);
		RightHandMesh->SetRelativeLocation(FVector(-2.9, 3.5, 4.5));
		RightHandMesh->SetRelativeRotation(FRotator(25, 0, 90));
	}
	//회피
	RollingCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RollingCircle"));
	RollingCircle->SetupAttachment(RootComponent);
	RollingCircle->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FClassFinder<AActor> BossHPUI(TEXT("/Script/Engine.Blueprint'/Game/TW/Blueprint/BP_BossHP.BP_BossHP_C'"));
	if (BossHPUI.Succeeded())
	{
		BossHPFac = BossHPUI.Class;
	}
	ConstructorHelpers::FClassFinder<AActor> GameOverUI(TEXT("/Script/Engine.Blueprint'/Game/TW/Blueprint/BP_GameOverUI.BP_GameOverUI_C'"));
	if (GameOverUI.Succeeded())
	{
		GameOverUIFac = GameOverUI.Class;
	}
	ConstructorHelpers::FClassFinder<AActor> ClearUI(TEXT("/Script/Engine.Blueprint'/Game/TW/Blueprint/BP_ClearUI.BP_ClearUI_C'"));
	if (ClearUI.Succeeded())
	{
		ClearUIFac = ClearUI.Class;
	}

	

	// 집게 손가락 생성
	//오른손
	rightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("rightAim"));
	rightAim->SetupAttachment(RootComponent);
	rightAim->MotionSource = FName("RightAim");
	
	interactionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("interaction"));
	interactionComp->SetupAttachment(rightAim);
	//왼손
	leftAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("leftAim"));
	leftAim->SetupAttachment(RootComponent);
	leftAim->MotionSource = FName("LeftAim");

	
	interactionComp1 = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("interaction1"));
	interactionComp1->SetupAttachment(leftAim);
	


	weponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("weponMeshComp"));
	shieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("shieldMeshComp"));
	// 왼손과 오른손에 장비 착용
	shieldMeshComp->SetupAttachment(GetMesh(), TEXT("hand_l"));
	weponMeshComp->SetupAttachment(GetMesh(), TEXT("hand_r"));

	playerSword = CreateDefaultSubobject<UChildActorComponent>(TEXT("playerSword"));
	playerSword->SetupAttachment(RightHandMesh);

}

// Called when the game starts or when spawned
void AVRPlayer::BeginPlay()
{
	Super::BeginPlay();
	//Enhanced Input 사용처리
	auto PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		auto localPlayer = PC->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_VRInput, 0);
			subSystem->AddMappingContext(IMC_Hand, 0);

		}
	}
	ResetRolling();
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);

	HP = 100;
	MP = maxMP;
	Stamina = maxStamina;


	if (IsBossLev = UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("T_Lev"))
	{

		//for (TActorIterator<ABoss> it(GetWorld()); it; ++it)
		//{
		//	Boss = *it;
		//}

		
		if (BossHPFac)
		{
			BossHP = GetWorld()->SpawnActor(BossHPFac);
			BossHP->SetActorEnableCollision(false);
		}

	}
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("ATG_EldenMap"))
	{
		FFileHelper::LoadFileToString(PlayerStartLocation, *startLoc);
		bool bLco;
		UKismetStringLibrary::Conv_StringToVector(PlayerStartLocation, PlayerStartVec, bLco);
		if (bLco)
		{
			SetActorLocation(PlayerStartVec);
		}
	}

	UploadMoneyFile();

}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 회피 확인 처리
	if (bRolling)
	{
		DrawRollingStraight();
	}

	if (IsBossLev)
	{
		BossHP->SetActorScale3D(FVector(0.02f));
		FRotator HeadToCamRot = (VRCamera->GetComponentLocation() - BossHP->GetActorLocation()).Rotation();
		BossHP->SetActorLocation(VRCamera->GetComponentLocation() + VRCamera->GetForwardVector() * 20 - VRCamera->GetUpVector() * 10);
		BossHP->SetActorRotation(FRotator(0, HeadToCamRot.Yaw, HeadToCamRot.Roll));
		if (EndingUI)
		{
			EndingUI->SetActorLocation(VRCamera->GetComponentLocation() + VRCamera->GetForwardVector() * 20);
			EndingUI->SetActorRotation((VRCamera->GetComponentLocation() - EndingUI->GetActorLocation()).Rotation());
		}
		if (!IsEndingUISpawn)
		{
			if (HP <= 0)
			{
				EndingUI = GetWorld()->SpawnActor(GameOverUIFac);
				EndingUI->SetActorScale3D(FVector(0.021f));
				IsEndingUISpawn = true;
			}
			else if (IsBossDie)
			{
				EndingUI = GetWorld()->SpawnActor(ClearUIFac);
				EndingUI->SetActorScale3D(FVector(0.021f));
				IsEndingUISpawn = true;
			}
		}

	}

	if (Stamina < 100)
	{
		Stamina += DeltaTime;
	}
	if (PoWTime <1.6f)
	{
		PoWTime += DeltaTime;
	}
	else if (PoWTime >= 1.5f)
	{
		PoWEnd();
	}
}

// Called to bind functionality to input
void AVRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	auto InputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputSystem)
	{
		InputSystem->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AVRPlayer::Move);
		InputSystem->BindAction(IA_Mouse, ETriggerEvent::Triggered, this, &AVRPlayer::Turn);
		InputSystem->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AVRPlayer::onActionJump);

		InputSystem->BindAction(IA_Rolling, ETriggerEvent::Started, this, &AVRPlayer::RollingStart);
		InputSystem->BindAction(IA_Rolling, ETriggerEvent::Completed, this, &AVRPlayer::RollingEnd);
		InputSystem->BindAction(IA_Interaction, ETriggerEvent::Started, this, &AVRPlayer::Interact);
		InputSystem->BindAction(IA_Interaction, ETriggerEvent::Completed, this, &AVRPlayer::InteractEnd);

		InputSystem->BindAction(IA_rGrab, ETriggerEvent::Started, this, &AVRPlayer::rTryGrab);
		InputSystem->BindAction(IA_rGrab, ETriggerEvent::Completed, this, &AVRPlayer::rUnTryGrab);
		InputSystem->BindAction(IA_lGrab, ETriggerEvent::Started, this, &AVRPlayer::lTryGrab);
		InputSystem->BindAction(IA_lGrab, ETriggerEvent::Completed, this, &AVRPlayer::lUnTryGrab);
	}
}

void AVRPlayer::Move(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	FString msg = FString::Printf(TEXT("X : %.3f, Y : %.3f"), Axis.X, Axis.Y);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, msg);
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
}

void AVRPlayer::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	if(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		AddControllerPitchInput(Axis.Y);
	}
}

// 회피 기능 활성화처리
void AVRPlayer::RollingStart(const FInputActionValue& Values)
{
	if (Stamina > 10)
	{
	// 누르고 있는 중에는 사용자가 어디를 가리키는지 주시하고 싶다.
	bRolling = true;
	}
	else
	{
		return;
	}
}

void AVRPlayer::RollingEnd(const FInputActionValue& Values)
{
	// 회피 기능 리셋
	// 만약 텔레포트가 불가능하다면
	if (ResetRolling() == false)
	{
		// 다음 처리를 하지 않는다.
		return;
	}

	// 회피 사용시 회피처리
	if (IsRolling)
	{
		DoRolling();
		Stamina -= 10;
		return;
	}
	// 그렇지 않을경우 회피
	// 회피 위치로 이동하고 싶다.
	SetActorLocation(RollingLocation + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
}

bool AVRPlayer::ResetRolling()
{
	// 회피가 활성화 되어 있을 때만 회피 가능하다.
	bool bCanRolling = RollingCircle->GetVisibleFlag();
	// 써클 안보이게 처리
	RollingCircle->SetVisibility(false);
	bRolling = false;
	

	return bCanRolling;
}

void AVRPlayer::DrawRollingStraight()
{
	
	// 직선을 그리고 싶다.
	// 필요정보 : 시작점, 종료점
	FVector StartPos = leftAim->GetComponentLocation();
	FVector EndPos = StartPos + leftAim->GetForwardVector() * 500;

	// 두 점 사이에 충돌체가 있는지 체크하자	
	CheckHitRolling(StartPos, EndPos);
	
	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, -1, 0, 1);
}

bool AVRPlayer::CheckHitRolling(FVector LastPos, FVector& CurPos)
{
	FHitResult HitInfo;
	bool bHit = HitTest(LastPos, CurPos, HitInfo);
	if (bHit)
	{
		// 마지막 점을(EndPos) 최종 점으로 수정하고 싶다.
		CurPos = HitInfo.Location;
		// 써클 활성화
		RollingCircle->SetVisibility(true);
		// 텔레포트써클을 위치
		RollingCircle->SetWorldLocation(CurPos);
		RollingLocation = CurPos;
	}
	else
	{
		RollingCircle->SetVisibility(false);
	}
	return bHit;
}

bool AVRPlayer::HitTest(FVector LastPos, FVector CurPos, FHitResult& HitInfo)
{
	FCollisionQueryParams Params;
	// 자기자신은 무시해라
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, LastPos, CurPos, ECC_Visibility, Params);

	return bHit;
}
void AVRPlayer::DoRolling()
{
	// 워프기능이 활성화 되어 있을 때
	if (IsRolling == false)
	{
		return;
	}
	// 워프처리 하고 싶다.
	// -> 일정시간동안 빠르게 이동하는거야
	// 경과시간 초기화
	CurTime = 0;
	// 충돌체 비활성화 
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 3. 시간이 흘러야한다.
	// 2. 일정시간동안
	// [캡처]()->{ body }
	GetWorld()->GetTimerManager().SetTimer(RollingHandle, FTimerDelegate::CreateLambda(
		[this]()->void
		{
			// body
			// 일정시간안에 목적지에 도착하고 싶다.
			// 1. 시간이 흘러야한다.
			CurTime += GetWorld()->DeltaTimeSeconds;
			// 현재
			FVector CurPos = GetActorLocation();
			// 도착
			FVector EndPos = RollingLocation + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			// 2. 이동해야한다.
			CurPos = FMath::Lerp<FVector>(CurPos, EndPos, CurTime /RollingTime);
			// 3. 목적지에 도착
			SetActorLocation(CurPos);
			// 시간이 다 흘렀다면 
			if (CurTime >= RollingTime)
			{
				// -> 그 위치로 할당하고
				SetActorLocation(EndPos);
				// -> 타이머 종료해주기
				GetWorld()->GetTimerManager().ClearTimer(RollingHandle);
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
	), 0.02f, true);

}

void AVRPlayer::onActionJump()
{
	Jump();
}



void AVRPlayer::Interact()
{
	if(interactionComp)
	{
		//interactionComp->PressPointerKey(FKey(FName("E")));
		UE_LOG(LogTemp, Warning, TEXT("INteraction"));
		interactionComp->PressPointerKey(EKeys::LeftMouseButton);
	}
	if(bStatInteraction)
	{
		// 	GetWorld()->SpawnActor<APlayerStatActor>(statActor, VRCamera->GetComponentLocation()+VRCamera->GetForwardVector()*600, FRotator::ZeroRotator);statWindow->SetStatWindow();
		statWindow->SetStatWindow();
		//UKismetStringLibrary::Conv_StringToVector()

		// 백터를 스트링으로 변환하고
		PlayerStartLocation = UKismetStringLibrary::Conv_VectorToString(savePoint->GetActorLocation());
		// 스트링 파일에 저장한다
		FFileHelper::SaveStringToFile(PlayerStartLocation, *startLoc);
		
		bStatInteraction = false;
	}
}

void AVRPlayer::InteractEnd()
{
	if(interactionComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("INteractionEnd"));
		interactionComp->ReleasePointerKey(EKeys::LeftMouseButton);
	}
}

void AVRPlayer::OnDamaged(float damage)
{
	HP -= damage;
	if(HP <= 0)
	{
		//체력이 0이하면 레벨을 다시 시작한다
		//UGameplayStatics::OpenLevel(GetWorld(), FName("ATG_EldenMap"));
		if(savePoint)
		{
			
		}
	}
}
// 물체를 잡고 싶다.
// 오른손으로 잡고싶다.
void AVRPlayer::rTryGrab()
{
	
	// 중심점
	FVector Center = RightHand->GetComponentLocation();
	// 충돌체크(구충돌)
	// 충돌할 물체를 기록할 배열
	// 충돌 질의 작성
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredComponent(RightHand);
	TArray<FOverlapResult> HitObjs;
	bool bHit = GetWorld()->OverlapMultiByChannel(HitObjs, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), Param);
	// 충돌하지 않았다면 아무처리 하지 않는다.
	if (bHit == false)
	{
		return;
	}
	// ->가장 가까운 물체 잡도록 하자(검출과정)

	// 가장 가까운 물체 인덱스
	float ClosestDist = 101;
	int Closest = 0;
	for (int i = 0; i < HitObjs.Num(); i++)
	{
		// 1. 물리 기능이 활성화 되어 있는 녀석만 판단
		//-> 만약 부딪힌 컴포넌트가 물리기능이 비활성화 되어있었다면
		if (HitObjs[i].GetComponent()->IsSimulatingPhysics() == false)
		{
			//검출하고 싶지 않다.
			continue;
		}
		//잡았다!
		rIsGrabbed = true;
		// 2. 현재 가장 가까운 녀석과 이번에 검출할 녀석과 더 가까운 녀석이 있다면
		// ->필요속성: 이번에 검출할 녀석과 손과의 거리
		float NextDist = FVector::Dist(HitObjs[i].GetActor()->GetActorLocation(), Center);

		// 3. 만일 이번에 현제꺼 보다 더 가깝다면
		if (NextDist < ClosestDist)
		{
			ClosestDist = NextDist;
			// ->가장 가까운 녀석으로 변경하기
			Closest = i;
		}
	}
	// 만약 잡았다면
	if (rIsGrabbed)
	{
		rGrabbedObject = HitObjs[Closest].GetComponent();
		// -> 물체 물리기능 비활성화
		rGrabbedObject->SetSimulatePhysics(false);
		//만약에 잡은게 무기라면
		APlayerWeapon* weapon;
		weapon = Cast<APlayerWeapon>(HitObjs[Closest].GetActor());
		if(weapon)
		{
			GrabbedWeapon = weapon;
			weapon->boxComp->SetCollisionProfileName(TEXT("PlayerWeaponPresset"));
		}

		APCPlayershield* shield;
		shield = Cast<APCPlayershield>(HitObjs[Closest].GetActor());
		if (shield)
		{
			GrabbedShield = shield;
			shield->boxComp->SetCollisionProfileName(TEXT("PlayerWeaponPresset"));
		}
		
		// -> 손에 붙여주자
		rGrabbedObject->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);
	}
}
//잡은 녀석이 있으면 놓고싶다.
void AVRPlayer::rUnTryGrab()
{
	if (rIsGrabbed == false)
	{
		return;
	}

	// 1. 잡지 않은 상태로 전환
	rIsGrabbed = false;
	// 2. 손에서 떼어내기
	rGrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 3. 물리기능 활성화
	rGrabbedObject->SetSimulatePhysics(true);
	// 4. 충돌기능 활성화
	rGrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if(GrabbedShield)
	{
		GrabbedShield->boxComp->SetCollisionProfileName(TEXT("BlockAll"));
		GrabbedShield = nullptr;
	}
	if (GrabbedWeapon)
	{
		GrabbedWeapon->boxComp->SetCollisionProfileName(TEXT("BlockAll"));
		GrabbedWeapon = nullptr;
	}
}

// 왼손으로 잡고싶다.
void AVRPlayer::lTryGrab()
{

	// 중심점
	FVector Center = LeftHand->GetComponentLocation();
	// 충돌체크(구충돌)
	// 충돌할 물체를 기록할 배열
	// 충돌 질의 작성
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredComponent(LeftHand);
	TArray<FOverlapResult> HitObjs;
	bool bHit = GetWorld()->OverlapMultiByChannel(HitObjs, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), Param);
	// 충돌하지 않았다면 아무처리 하지 않는다.
	if (bHit == false)
	{
		return;
	}
	// ->가장 가까운 물체 잡도록 하자(검출과정)

	// 가장 가까운 물체 인덱스
	float ClosestDist = 101;
	int Closest = 0;
	for (int i = 0; i < HitObjs.Num(); i++)
	{
		// 1. 물리 기능이 활성화 되어 있는 녀석만 판단
		//-> 만약 부딪힌 컴포넌트가 물리기능이 비활성화 되어있었다면
		if (HitObjs[i].GetComponent()->IsSimulatingPhysics() == false)
		{
			//검출하고 싶지 않다.
			continue;
		}
		//잡았다!
		lIsGrabbed = true;
		// 2. 현재 가장 가까운 녀석과 이번에 검출할 녀석과 더 가까운 녀석이 있다면
		// ->필요속성: 이번에 검출할 녀석과 손과의 거리
		float NextDist = FVector::Dist(HitObjs[i].GetActor()->GetActorLocation(), Center);

		// 3. 만일 이번에 현제꺼 보다 더 가깝다면
		if (NextDist < ClosestDist)
		{
			// ->가장 가까운 녀석으로 변경하기
			Closest = i;
		}
	}
	// 만약 잡았다면
	if (lIsGrabbed)
	{
		lGrabbedObject = HitObjs[Closest].GetComponent();
		// -> 물체 물리기능 비활성화
		lGrabbedObject->SetSimulatePhysics(false);
		//rGrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		APlayerWeapon* weapon;
		weapon = Cast<APlayerWeapon>(HitObjs[Closest].GetActor());
		if(weapon)
		{
			GrabbedWeapon = weapon;
			weapon->boxComp->SetCollisionProfileName(TEXT("PlayerWeaponPresset"));
		}

		APCPlayershield* shield;
		shield = Cast<APCPlayershield>(HitObjs[Closest].GetActor());
		if (shield)
		{
			GrabbedShield = shield;
			shield->boxComp->SetCollisionProfileName(TEXT("PlayerWeaponPresset"));
		}
		// -> 손에 붙여주자
		lGrabbedObject->AttachToComponent(LeftHand, FAttachmentTransformRules::KeepWorldTransform);
	}
}
//잡은 녀석이 있으면 놓고싶다.
void AVRPlayer::lUnTryGrab()
{
	if (lIsGrabbed == false)
	{
		return;
	}

	// 1. 잡지 않은 상태로 전환
	lIsGrabbed = false;
	// 2. 손에서 떼어내기
	lGrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 3. 물리기능 활성화
	lGrabbedObject->SetSimulatePhysics(true);
	// 4. 충돌기능 활성화
	lGrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


	if(GrabbedWeapon)
	{
		GrabbedWeapon->boxComp->SetCollisionProfileName(TEXT("BlockAll"));
		GrabbedWeapon = nullptr;
	}
	if (GrabbedShield)
	{
		GrabbedShield->boxComp->SetCollisionProfileName(TEXT("BlockAll"));
		GrabbedShield = nullptr;
	}
}

// 무적시간 (Power over wellming)
void AVRPlayer::PoWStart()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PoWTime = 0;
}

void AVRPlayer::PoWEnd()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AVRPlayer::UploadMoneyFile()
{
	FString money;

	//파일을 로드한다
	FFileHelper::LoadFileToString(money, *PlayerMoneyFile);
	
	//문자열을 정수로
	PlayerMoney = FCString::Atoi(*money);
}

void AVRPlayer::GetMoney(int32 income)
{
	//돈을 추가한다
	PlayerMoney += income;
	FFileHelper::SaveStringToFile(FString::FromInt(PlayerMoney), *PlayerMoneyFile);
}

