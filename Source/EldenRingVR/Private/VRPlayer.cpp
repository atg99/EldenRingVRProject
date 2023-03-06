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
}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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