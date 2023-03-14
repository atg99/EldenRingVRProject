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
#include <Components/CapsuleComponent.h>
#include "BossHP.h"
#include "PlayerStatActor.h"
#include "Components/WidgetInteractionComponent.h"

// Sets default values
AVRPlayer::AVRPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// VR카메라 생성
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bUsePawnControlRotation = true;
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

	ConstructorHelpers::FClassFinder<UBossHP> BossHPCL(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TW/UI/UI_Boss.UI_Boss_C'"));
	if (BossHPCL.Succeeded())
	{
		BossHPC = BossHPCL.Class;
	}

	interactionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("interaction"));
	interactionComp->SetupAttachment(VRCamera);
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
		}
	}
	ResetRolling();
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);

	HP = maxHP;
	MP = maxMP;
	Stamina = maxStamina;

	BossHP = CreateWidget<UBossHP>(GetWorld(), BossHPC);
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_T_Lev"))
	{
		BossHP->AddToViewport();

	}
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
	}
}

void AVRPlayer::Move(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
}

void AVRPlayer::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

// 회피 기능 활성화처리
void AVRPlayer::RollingStart(const FInputActionValue& Values)
{
	// 누르고 있는 중에는 사용자가 어디를 가리키는지 주시하고 싶다.
	bRolling = true;
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
	FVector StartPos = VRCamera->GetComponentLocation();
	FVector EndPos = StartPos + VRCamera->GetForwardVector() * 500;

	// 두 점 사이에 충돌체가 있는지 체크하자	
	CheckHitRolling(StartPos, EndPos);
	
	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, -1, 0, 1);
}

bool AVRPlayer::CheckHitRolling(FVector LastPos, FVector& CurPos)
{
	FHitResult HitInfo;
	bool bHit = HitTest(LastPos, CurPos, HitInfo);
	// 만약 부딪힌 대상이 바닥이라면
	if (bHit && HitInfo.GetActor()->GetName().Contains(TEXT("Floor")))
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

void AVRPlayer::DoAttack()
{
}
void AVRPlayer::DoDefence()
{
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
}
