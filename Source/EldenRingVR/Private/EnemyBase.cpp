// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "EnemySword.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("weapon"));
	weapon->SetupAttachment(GetMesh(), FName("sword"));
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	AEnemySword* sword = Cast<AEnemySword>(weapon->GetChildActor());
	if(sword)
	{
		sword->ownerEnemy = this;
	}
	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

