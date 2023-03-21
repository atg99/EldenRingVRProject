// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPlayershield.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Boss.h"
#include "EnemyBase.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnemySword.h"
#include "VRPlayer.h"
// Sets default values
APCPlayershield::APCPlayershield()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetWorldScale3D(FVector(0.09f, 0.02f, 0.43f));

	boxComp->SetCollisionProfileName(TEXT("ShieldPreset"));
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
void APCPlayershield::BeginPlay()
{
	Super::BeginPlay();
	
	//�ݸ��� �ڽ��� ������ ������ ����� �Լ�
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APCPlayershield::OnOverlap);


	//�ݸ��� �ڽ��� ������ �̺�Ʈ �߻�
	boxComp->SetGenerateOverlapEvents(true);

	// ������ �����Ҷ� ��Ÿ��
	AttackCoolTime = 1.5f;
}

// Called every frame
void APCPlayershield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetShieldSpeed();
	
	if (AttackCoolTime < 1.6f)
	{
		AttackCoolTime += DeltaTime;
	}
	if (ShieldTime < 1.6f)
	{
		ShieldTime += DeltaTime;
	}
}
/*
// Called to bind functionality to input
void APCPlayershield::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
*/

// �ݸ��� �ڽ� ������ �Լ�
void APCPlayershield::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	//if(meshComp->GetComponentVelocity())

	UE_LOG(LogTemp, Warning, TEXT("Defense"));

	// ������ ������ �Ǿ��ٸ�.
	AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor);
	ABoss* boss = Cast<ABoss>(OtherActor);
	AEnemySword* eSword = Cast<AEnemySword>(OtherActor);
	AVRPlayer* player = Cast<AVRPlayer>(OtherActor);
	if (eSword != nullptr && player->Stamina > 10 && player->PoWTime >= 1.5f)
	{
		player->Stamina -= 10;
		player->PoWStart();
		
	}

	// ������ ���
	if (enemy != nullptr && shieldSpeed.Size() > 10)
	{
		enemy->OnDamaged(shieldSpeed.Size());
		// ���� ��ü�� ����
		enemyBase = enemy;
		if (shieldSpeed.Size() > 10)
		{
			WeaponTrace();
		}
		//boxComp->SetGenerateOverlapEvents(false);
	}

	//������ ���
	if (boss != nullptr && AttackCoolTime >= 1.5f)
	{
		boss->CurHP -= WeponDamage;
		AttackCoolTime = 0;
	}
	else
	{
		return;
	}
}

void APCPlayershield::WeaponTrace()
{
	//�� ��ġ ����
	TArray<AActor*> ActorsToIgnore;
	FHitResult hitResult;

	FVector traceStartLoc = wStart->GetComponentLocation();
	FVector endLoc = wEnd->GetComponentLocation();

	const bool isHit = UKismetSystemLibrary::BoxTraceSingle(
		wStart,
		traceStartLoc,
		endLoc,
		FVector(0.1, 20, 0.1),
		GetActorRotation() + FRotator(0, 90, 0),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel10),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true,
		FLinearColor::Gray,
		FLinearColor::Red,
		1
	);

	if (isHit)
	{
		//���� �Լ�
		enemyBase->Desmemberment(hitResult.BoneName, hitResult.Location, hitResult.ImpactNormal);
		//blood niagara
		enemyBase->SpawnNiagaraAttach(hitResult.BoneName, hitResult.Location, (hitResult.Location - enemyBase->GetActorLocation()).Rotation());
	}

}

void APCPlayershield::GetShieldSpeed()
{
	shieldSpeed = meshComp->GetComponentLocation() - prevPos;
	prevPos = meshComp->GetComponentLocation();
}
