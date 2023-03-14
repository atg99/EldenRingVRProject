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

// Sets default values
AVRPlayer::AVRPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// VRī�޶� ����
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bUsePawnControlRotation = true;
	// �� �߰�
	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));
	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));

	//���̷�Ż �޽� ������Ʈ �����
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHand);
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHand);

	// �޼� �޽� �Ҵ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (TempMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(TempMesh.Object);
		LeftHandMesh->SetRelativeLocation(FVector(-2.9, -3.5, 4.5));
		LeftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
	}
	// ������ �޽� �Ҵ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh2(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (TempMesh2.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(TempMesh2.Object);
		RightHandMesh->SetRelativeLocation(FVector(-2.9, 3.5, 4.5));
		RightHandMesh->SetRelativeRotation(FRotator(25, 0, 90));
	}
	//ȸ��
	RollingCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RollingCircle"));
	RollingCircle->SetupAttachment(RootComponent);
	RollingCircle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	weponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("weponMeshComp"));
	shieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("shieldMeshComp"));
	//����, ���и� �տ� ����
	shieldMeshComp->SetupAttachment(GetMesh(), TEXT("hand_l"));
	weponMeshComp->SetupAttachment(GetMesh(), TEXT("hand_r"));
	
}

// Called when the game starts or when spawned
void AVRPlayer::BeginPlay()
{
	Super::BeginPlay();
	//Enhanced Input ���ó��
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

	HP = maxHP;
	MP = maxMP;
	Stamina = maxStamina;

}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// ȸ�� Ȯ�� ó��
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

// ȸ�� ��� Ȱ��ȭó��
void AVRPlayer::RollingStart(const FInputActionValue& Values)
{
	// ������ �ִ� �߿��� ����ڰ� ��� ����Ű���� �ֽ��ϰ� �ʹ�.
	bRolling = true;
}

void AVRPlayer::RollingEnd(const FInputActionValue& Values)
{
	// ȸ�� ��� ����
	// ���� �ڷ���Ʈ�� �Ұ����ϴٸ�
	if (ResetRolling() == false)
	{
		// ���� ó���� ���� �ʴ´�.
		return;
	}

	// ȸ�� ���� ȸ��ó��
	if (IsRolling)
	{
		DoRolling();
		return;
	}
	// �׷��� ������� ȸ��
	// ȸ�� ��ġ�� �̵��ϰ� �ʹ�.
	SetActorLocation(RollingLocation + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
}

bool AVRPlayer::ResetRolling()
{
	// ȸ�ǰ� Ȱ��ȭ �Ǿ� ���� ���� ȸ�� �����ϴ�.
	bool bCanRolling = RollingCircle->GetVisibleFlag();
	// ��Ŭ �Ⱥ��̰� ó��
	RollingCircle->SetVisibility(false);
	bRolling = false;
	

	return bCanRolling;
}

void AVRPlayer::DrawRollingStraight()
{
	
	// ������ �׸��� �ʹ�.
	// �ʿ����� : ������, ������
	FVector StartPos = VRCamera->GetComponentLocation();
	FVector EndPos = StartPos + VRCamera->GetForwardVector() * 500;

	// �� �� ���̿� �浹ü�� �ִ��� üũ����	
	CheckHitRolling(StartPos, EndPos);
	
	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, -1, 0, 1);
}

bool AVRPlayer::CheckHitRolling(FVector LastPos, FVector& CurPos)
{
	FHitResult HitInfo;
	bool bHit = HitTest(LastPos, CurPos, HitInfo);
	// ���� �ε��� ����� �ٴ��̶��
	if (bHit && HitInfo.GetActor()->GetName().Contains(TEXT("Floor")))
	{
		// ������ ����(EndPos) ���� ������ �����ϰ� �ʹ�.
		CurPos = HitInfo.Location;
		// ��Ŭ Ȱ��ȭ
		RollingCircle->SetVisibility(true);
		// �ڷ���Ʈ��Ŭ�� ��ġ
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
	// �ڱ��ڽ��� �����ض�
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, LastPos, CurPos, ECC_Visibility, Params);

	return bHit;
}
void AVRPlayer::DoRolling()
{
	// ��������� Ȱ��ȭ �Ǿ� ���� ��
	if (IsRolling == false)
	{
		return;
	}
	// ����ó�� �ϰ� �ʹ�.
	// -> �����ð����� ������ �̵��ϴ°ž�
	// ����ð� �ʱ�ȭ
	CurTime = 0;
	// �浹ü ��Ȱ��ȭ 
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 3. �ð��� �귯���Ѵ�.
	// 2. �����ð�����
	// [ĸó]()->{ body }
	GetWorld()->GetTimerManager().SetTimer(RollingHandle, FTimerDelegate::CreateLambda(
		[this]()->void
		{
			// body
			// �����ð��ȿ� �������� �����ϰ� �ʹ�.
			// 1. �ð��� �귯���Ѵ�.
			CurTime += GetWorld()->DeltaTimeSeconds;
			// ����
			FVector CurPos = GetActorLocation();
			// ����
			FVector EndPos = RollingLocation + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			// 2. �̵��ؾ��Ѵ�.
			CurPos = FMath::Lerp<FVector>(CurPos, EndPos, CurTime /RollingTime);
			// 3. �������� ����
			SetActorLocation(CurPos);
			// �ð��� �� �귶�ٸ� 
			if (CurTime >= RollingTime)
			{
				// -> �� ��ġ�� �Ҵ��ϰ�
				SetActorLocation(EndPos);
				// -> Ÿ�̸� �������ֱ�
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