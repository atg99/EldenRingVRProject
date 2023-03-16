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
	//칼날 충돌 프리셋
	boxComp->SetCollisionProfileName(TEXT("WeaponPreset"));
	boxComp->SetGenerateOverlapEvents(true);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetSimulatePhysics(true);
	//상대적인 위치
	meshComp->SetRelativeLocation(FVector(0, 0, 0));
	meshComp->SetRelativeScale3D(FVector(1));
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	//충돌(오버랩)이 발생하면 실행할 함수를 연결한다.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnOverlap);

	//오버랩이벤트를 true로 설정한다
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

	//만일 부딪힌 대상이 에너미라면
	AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor); 
	ABoss* boss = Cast<ABoss>(OtherActor); 
	if (enemy != nullptr) 
	{
		enemy -> OnDamaged(60);
		
		//boxComp->SetGenerateOverlapEvents(false);
	}
	//만일 부딪힌 대상이 보스라면
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
