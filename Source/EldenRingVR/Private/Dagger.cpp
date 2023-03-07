// Fill out your copyright notice in the Description page of Project Settings.


#include "Dagger.h"

// Sets default values
ADagger::ADagger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DaggerComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dagger"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> DaggerMesh(TEXT("/Script/Engine.StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Dagger_1.SM_Dagger_1'"));
	if (DaggerMesh.Succeeded())
	{
		DaggerComp->SetStaticMesh(DaggerMesh.Object);
	}
	
}

// Called when the game starts or when spawned
void ADagger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADagger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsDaggerThrow)
	{
		SetActorLocation(GetActorLocation() + ToTargetV * 10);
	}
	

}

void ADagger::DaggerThrow(FVector vec, FRotator rot)
{
	ToTargetV = vec;
	ToTargetR = rot;
	SetActorRotation(rot + FRotator(0, 90, -90));
	IsDaggerThrow = true;
	FTimerHandle DestroyTime;
	GetWorldTimerManager().SetTimer(DestroyTime, this, &ADagger::DaggerDestroy, 5);
}

void ADagger::DaggerDestroy()
{
	Destroy();
}