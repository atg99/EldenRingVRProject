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


// Sets default values
APCPlayer::APCPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스프링암, 카메라 컴포넌트
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));

	// 스프링암을 루트에 붙임
	springArmComp->SetupAttachment(RootComponent);
	// 카메라를 스프링암에 붙임
	cameraComp->SetupAttachment(springArmComp);
	// 스프링암 위치,거리
	springArmComp->SetRelativeLocation(FVector(0, 0, 0));
	springArmComp->TargetArmLength = 400;
	// 카메라 위치,각도
	cameraComp->SetRelativeLocation(FVector(41, 0, 87));
	cameraComp->SetRelativeRotation(FRotator(-20, 0, 0));
	//입력값을 회전에 반영가능
	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = true;
	


}

// Called when the game starts or when spawned
void APCPlayer::BeginPlay()
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
}

// Called every frame
void APCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

