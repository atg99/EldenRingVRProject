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
	

	//총알 충돌 프리셋
	boxComp->SetCollisionProfileName(TEXT("WeaponPreset"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
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
	boxComp->SetGenerateOverlapEvents(true);
	
	//월드타이머 설정한 함수 실행
	//                                                     실행할함수   초설정, 반복참거짓, 
	GetWorld()->GetTimerManager().SetTimer(lifeTimer, this, &APlayerWeapon::AttackCoolTime, 1, false);
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void APlayerWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만일 부딪힌 대상이 에너미라면
	AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor); 
	ABoss* boss = Cast<ABoss>(OtherActor); 
	if (enemy != nullptr) 
	{
		enemy -> OnDamaged(60);
		
		//boxComp->SetGenerateOverlapEvents(false);
	}
	//만일 부딪힌 대상이 보스라면
	if (boss != nullptr)
	{
		boss -> CurHP-=60;

		//boxComp->SetGenerateOverlapEvents(false);
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