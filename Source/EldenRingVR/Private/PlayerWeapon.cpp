// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Boss.h"
#include "EnemyBase.h"
#include "TimerManager.h"
// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetWorldScale3D(FVector(0.09f, 0.02f, 0.43f));
	//Į�� �浹 ������
	boxComp->SetCollisionProfileName(TEXT("WeaponPreset"));
	boxComp->SetGenerateOverlapEvents(true);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetSimulatePhysics(true);
	//������� ��ġ
	meshComp->SetRelativeLocation(FVector(0, 0, 0));
	meshComp->SetRelativeScale3D(FVector(1));
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	//�浹(������)�� �߻��ϸ� ������ �Լ��� �����Ѵ�.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnOverlap);

	//�������̺�Ʈ�� true�� �����Ѵ�
	AttackCoolTime = 1.5f;
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AttackCoolTime < 1.6f)
	{
		AttackCoolTime += DeltaTime;
	}
}
void APlayerWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("PWeponAttack"));

	//���� �ε��� ����� ���ʹ̶��
	AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor); 
	ABoss* boss = Cast<ABoss>(OtherActor); 
	if (enemy != nullptr) 
	{
		enemy -> OnDamaged(60);
		
		//boxComp->SetGenerateOverlapEvents(false);
	}
	//���� �ε��� ����� �������
	if (boss != nullptr && AttackCoolTime >= 1.5f)
	{
		boss -> CurHP-=60;
		AttackCoolTime = 0;
	}
	else
	{
		return;
	}
}
