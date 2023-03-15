// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPlayer.h"
#include "EnhancedInputSubsystems.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "VRGameModeBase.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include <DrawDebugHelpers.h>
#include <Components/CapsuleComponent.h>
#include "PCPlayerAnim.h"
#include "BossHP.h"


// Sets default values
APCPlayer::APCPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��������, ī�޶�, ����, ���� ������Ʈ
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	weponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("weponMeshComp"));
	shieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("shieldMeshComp"));

	// ���������� ��Ʈ�� ����
	springArmComp->SetupAttachment(RootComponent);
	// ī�޶� �������Ͽ� ����
	cameraComp->SetupAttachment(springArmComp);
	// �������� ��ġ,�Ÿ�
	springArmComp->SetRelativeLocation(FVector(0, 0, 0));
	springArmComp->TargetArmLength = 400;
	// ī�޶� ��ġ,����
	cameraComp->SetRelativeLocation(FVector(41, 0, 87));
	cameraComp->SetRelativeRotation(FRotator(-20, 0, 0));
	//�Է°��� ȸ���� �ݿ�����
	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = true;

	
	ConstructorHelpers::FClassFinder<UBossHP> BossHPCL(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TW/UI/UI_Boss.UI_Boss_C'"));
	if (BossHPCL.Succeeded())
	{
		BossHPC = BossHPCL.Class;
	}

	//����, ���и� �տ� ����
	weponMeshComp->SetupAttachment(GetMesh(), TEXT("RightHand"));
	shieldMeshComp->SetupAttachment(GetMesh(), TEXT("LeftHand"));




}

// Called when the game starts or when spawned
void APCPlayer::BeginPlay()
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
		}
	}

	BossHP = CreateWidget<UBossHP>(GetWorld(), BossHPC);
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_T_Lev"))
	{
		BossHP->AddToViewport();
		
	}

	
}

// Called every frame
void APCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//BossHP->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 1000 - FVector::UpVector * 100);
	//BossHP->SetActorRotation(FRotator(0, (BossHP->GetActorLocation() - GetActorLocation()).Rotation().Yaw + 180, 0));
	


}

// Called to bind functionality to input
void APCPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	auto InputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputSystem)
	{
		InputSystem->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APCPlayer::Move);
		InputSystem->BindAction(IA_Mouse, ETriggerEvent::Triggered, this, &APCPlayer::Turn);
		InputSystem->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &APCPlayer::onActionJump);
		InputSystem->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &APCPlayer::Attack);
		InputSystem->BindAction(IA_Defence, ETriggerEvent::Triggered, this, &APCPlayer::Defence);
		InputSystem->BindAction(IA_Desh, ETriggerEvent::Triggered, this, &APCPlayer::Desh);

	}
}

void APCPlayer::Move(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
}

void APCPlayer::Turn(const FInputActionValue& Values)	
{
	FVector2D Axis = Values.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void APCPlayer::onActionJump()
{
	Jump();
}

void APCPlayer::Attack()
{
	UE_LOG(LogTemp, Log, TEXT("attack"));
	auto anim = Cast<UPCPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->OnAttack(TEXT("ATStart"));
	//PlayAnimMontage(AttackMontageFactory);
	
}
void APCPlayer::Defence()
{
	UE_LOG(LogTemp, Log, TEXT("defence"));

	//PlayAnimMontage(AttackMontageFactory);
	auto anim = Cast<UPCPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->OnAttack(TEXT("DFStart"));
}
void APCPlayer::Desh()
{
}

void APCPlayer::OnMyAttack()
{
}

void APCPlayer::OnMyDefence()
{
}

