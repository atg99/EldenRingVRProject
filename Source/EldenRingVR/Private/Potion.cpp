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

	//�ݸ��� �ڽ��� ������ ������ ����� �Լ�
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APotion::OnOverlap);


	//�ݸ��� �ڽ��� ������ �̺�Ʈ �߻�
	boxComp->SetGenerateOverlapEvents(true);
}

// Called every frame
void APotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}
// �ݸ��� �ڽ� ������ �Լ�
void APotion::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �÷��̾�� ������ �Ǿ��ٸ�.
	AVRPlayer* player = Cast<AVRPlayer>(OtherActor);
	// �÷��̾� ���
	if (player != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HP UP"));

		player->HP += HealUp;
	}
	//����� �ڽ� ����
	Destroy();
}

