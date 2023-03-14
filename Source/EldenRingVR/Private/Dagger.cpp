// Fill out your copyright notice in the Description page of Project Settings.


#include "Dagger.h"
#include "Components/CapsuleComponent.h"
#include "Boss.h"
#include "VRPlayer.h"
#include "BossFSM.h"

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
	DaggerCapComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DaggerCap"));
	DaggerCapComp->SetupAttachment(DaggerComp);
	DaggerCapComp->SetRelativeScale3D(FVector(0.7f));
	DaggerCapComp->SetRelativeLocation(FVector(0, 0, 20));
	DaggerCapComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DaggerCapComp->OnComponentBeginOverlap.AddDynamic(this, &ADagger::OnDaggerBeginOverlap);
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
		SetActorLocation(GetActorLocation() + ToTargetV * 30);
	}
	

}

void ADagger::OnDaggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	class AVRPlayer* Player = Cast<AVRPlayer>(SweepResult.GetActor());
	if (Player && CanAttack)
	{
		Player->OnDamaged(3);
		CanAttack = false;
	}
	

	
}

void ADagger::DaggerThrow(FVector vec, FRotator rot)
{
	DaggerCapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ToTargetV = vec;
	ToTargetR = rot;
	SetActorRotation(rot + FRotator(0, 90, -90));
	IsDaggerThrow = true;
	CanAttack = true;
	FTimerHandle DestroyTime;
	GetWorldTimerManager().SetTimer(DestroyTime, this, &ADagger::DaggerDestroy, 5);
}

void ADagger::DaggerDestroy()
{
	CanAttack = false;
	Destroy();
}