// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySword.h"

#include "EnemyBase.h"
#include "TPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemySword::AEnemySword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sword"));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Sword.SK_Sword'"));
	if (TempMesh.Succeeded()) {
		sword->SetSkeletalMesh(TempMesh.Object);
	}

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(sword);
	boxComp->SetCollisionProfileName(FName("EnemyWeaponOverlap"));
}

// Called when the game starts or when spawned
void AEnemySword::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemySword::SwordBeginOverlap);
	
}

// Called every frame
void AEnemySword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySword::SwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//플레이어에게 칼이 닿았다
	ATPlayer* player0 = Cast<ATPlayer>(OtherActor);
	if(player0)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("PlayerHit")));
		//데미지 함수

		//플레이어 임펙트
		WeaponTrace();
		
	}
}

void AEnemySword::WeaponTrace()
{
	//피 위치 생성
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> hitArray;
	
	FVector startLoc = sword->GetSocketLocation(TEXT("weapon_top"));
	FVector endLoc = sword->GetSocketLocation(TEXT("weapon_bottom"));

	const bool isHit = UKismetSystemLibrary::BoxTraceMulti(
		sword,
		startLoc,
		endLoc,
		FVector(5, 90, 20),
		sword->GetSocketRotation(TEXT("weapon_top")),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		hitArray,
		true,
		FLinearColor::Gray,
		FLinearColor::Red,
		2
	);
}

