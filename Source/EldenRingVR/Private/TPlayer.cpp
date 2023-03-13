// Fill out your copyright notice in the Description page of Project Settings.


#include "TPlayer.h"

// Sets default values
ATPlayer::ATPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	 // if (BossHPFac)
	 // {
	 // 	BossHP = GetWorld()->SpawnActor(BossHPFac);
	 // }
	 //
	 // BossHP->SetActorEnableCollision(false);
	
}

// Called every frame
void ATPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//BossHP->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 1000 - FVector::UpVector * 100);
	//BossHP->SetActorRotation(FRotator(0, (BossHP->GetActorLocation() - GetActorLocation()).Rotation().Yaw + 180, 0));
	
}

// Called to bind functionality to input
void ATPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

