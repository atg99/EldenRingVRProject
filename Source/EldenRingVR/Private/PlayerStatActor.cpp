// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatActor.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

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
	
}

// Called every frame
void APlayerStatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

