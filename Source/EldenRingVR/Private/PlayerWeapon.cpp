// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Boss.h"
#include "EnemyBase.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetWorldScale3D(FVector(0.09f, 0.02f, 0.43f));

	boxComp->SetCollisionProfileName(TEXT("WeaponPreset"));
	boxComp->SetGenerateOverlapEvents(true);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetSimulatePhysics(true);

	meshComp->SetRelativeLocation(FVector(0, 0, 0));
	meshComp->SetRelativeScale3D(FVector(1));
	
	wStart = CreateDefaultSubobject<UBoxComponent>(TEXT("start"));
	wStart->SetupAttachment(meshComp);
	wEnd = CreateDefaultSubobject<UBoxComponent>(TEXT("wEnd"));
	wEnd->SetupAttachment(meshComp);
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	//�浹(������)�� �߻��ϸ� ������ �Լ��� �����Ѵ�.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnOverlap);


	//�������̺�Ʈ�� true�� �����Ѵ�
	boxComp->SetGenerateOverlapEvents(true);
	
	//����Ÿ�̸� ������ �Լ� ����
	//                                                     �������Լ�   �ʼ���, �ݺ�������, 
	GetWorld()->GetTimerManager().SetTimer(lifeTimer, this, &APlayerWeapon::AttackCoolTime, 1, false);

	//�������̺�Ʈ�� true�� �����Ѵ�
	AttackCoolTime = 1.5f;

}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	GetSwordSpeed();
	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), swordSpeed.Size()));

	if (AttackCoolTime < 1.6f)
	{
		AttackCoolTime += DeltaTime;
	}

}

void APlayerWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//���� �ε��� ����� ���ʹ̶��
	//if(meshComp->GetComponentVelocity())

	UE_LOG(LogTemp, Warning, TEXT("PWeponAttack"));

	//���� �ε��� ����� ���ʹ̶��
	AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor); 
	ABoss* boss = Cast<ABoss>(OtherActor); 
	if (enemy != nullptr) 
	{
		enemy -> OnDamaged(swordSpeed.Size());
		enemyBase = enemy;
		if(swordSpeed.Size() > 10)
		{
			WeaponTrace();
		}
		//boxComp->SetGenerateOverlapEvents(false);
	}

	//���� �ε��� ����� �������
	if (boss != nullptr)

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


void APlayerWeapon::AttackCoolTime()
{
	boxComp->SetGenerateOverlapEvents(true);
}

void APlayerWeapon::WeaponTrace()
{
	//피 위치 생성
	TArray<AActor*> ActorsToIgnore;
	FHitResult hitResult;
	
	FVector traceStartLoc = wStart->GetComponentLocation();
	FVector endLoc = wEnd->GetComponentLocation();

	const bool isHit = UKismetSystemLibrary::BoxTraceSingle(
		wStart,
		traceStartLoc,
		endLoc,
		FVector(10, 10, 5),
		GetActorRotation()+FRotator(0,90,0),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel10),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true,
		FLinearColor::Gray,
		FLinearColor::Red,
		10
	);

	if(isHit)
	{
		enemyBase->Desmemberment(hitResult.BoneName, hitResult.ImpactPoint, hitResult.ImpactNormal);
	}
	
}

void APlayerWeapon::GetSwordSpeed()
{
	swordSpeed = meshComp->GetComponentLocation() - prevPos;

	prevPos = meshComp->GetComponentLocation();
}
