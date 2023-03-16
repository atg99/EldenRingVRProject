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

	//콜리전 박스에 오버랩 됬을때 사용할 함수
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnOverlap);


	//콜리전 박스에 오버랩 이벤트 발생
	boxComp->SetGenerateOverlapEvents(true);
	
	// 보스를 공격할때 쿨타임
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
// 콜리젼 박스 오버랩 함수
void APlayerWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
	//if(meshComp->GetComponentVelocity())

	UE_LOG(LogTemp, Warning, TEXT("PWeponAttack"));

	// 적에게 오버랩 되었다면.
	AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor); 
	ABoss* boss = Cast<ABoss>(OtherActor); 
	// 졸병일 경우
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

	//보스일 경우
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
