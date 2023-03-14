// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatActor.h"

#include "UI_PlayerStat.h"
#include "VRPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerStatActor::APlayerStatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("rootScene"));
	SetRootComponent(rootComp);
	
	statWindow = CreateDefaultSubobject<UWidgetComponent>(TEXT("statWindow"));
	statWindow->SetupAttachment(RootComponent);
	statWindow->SetCollisionProfileName(TEXT("3DWidgetPresset"));
}

// Called when the game starts or when spawned
void APlayerStatActor::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AVRPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	player->statWindow = this;

	statUI = Cast<UUI_PlayerStat>(statWindow->GetWidget());
}

// Called every frame
void APlayerStatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(FRotator( 0,(player->GetActorLocation()-GetActorLocation()).Rotation().Yaw, 0));
	//SetActorRotation((player->GetActorLocation()-GetActorLocation()).GetSafeNormal().)
}

void APlayerStatActor::HideStatWindow()
{
}

void APlayerStatActor::SetStatWindow()
{
	if(statUI)
	{
		SetActorLocation(player->VRCamera->GetComponentLocation()+player->VRCamera->GetForwardVector()*650);
		statUI->ShowStatSet();
	}
}
