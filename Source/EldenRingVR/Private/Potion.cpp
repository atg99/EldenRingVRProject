// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "VRPlayer.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"



// Sets default values
APotion::APotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APotion::BeginPlay()
{
	Super::BeginPlay();

	//콜리전 박스에 오버랩 됬을때 사용할 함수
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APotion::OnOverlap);


	//콜리전 박스에 오버랩 이벤트 발생
	boxComp->SetGenerateOverlapEvents(true);
}

// Called every frame
void APotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}
// 콜리젼 박스 오버랩 함수
void APotion::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어에게 오버랩 되었다면.
	AVRPlayer* player = Cast<AVRPlayer>(OtherActor);
	// 플레이어 라면
	if (player != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HP UP"));

		player->HP += HealUp;
	}
	//사용후 자신 제거
	Destroy();
}

