// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "BossAnim.h"
#include "Dagger.h"
#include "BossFSM.h"
#include "Components/CapsuleComponent.h"
#include "VRPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FName RightHandSocket(TEXT("RightHandSocket"));
	FName LeftHandSocket(TEXT("LeftHandSocket"));
	Dagger = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Dagger"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> DaggerMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Dagger_1.SK_Dagger_1'"));
	if (DaggerMesh.Succeeded())
	{
		Dagger->SetSkeletalMesh(DaggerMesh.Object);
	}
	Dagger->SetupAttachment(GetMesh(), RightHandSocket);
	Dagger->SetRelativeRotation(FRotator(0, -90, -90));
	Dagger->SetRelativeScale3D(FVector(2.0f));
	Dagger->SetVisibility(false);

	Mace = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mace"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MaceMesh(TEXT("/Script/Engine.StaticMesh'/Game/TW/Blueprint/SM_Mace1.SM_Mace1'"));
	if (MaceMesh.Succeeded())
	{
		Mace->SetStaticMesh(MaceMesh.Object);
	}
	Mace->SetupAttachment(GetMesh(), LeftHandSocket);
	Mace->SetRelativeRotation(FRotator(-90, 0, 0));
	Mace->SetRelativeScale3D(FVector(1.5f));

	ConstructorHelpers::FClassFinder<UBossAnim> BossAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/TW/Animation/ABP_Boss.ABP_Boss_C'"));
	if (BossAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BossAnim.Class);
	}

	BossFSM = CreateDefaultSubobject<UBossFSM>(TEXT("BossFSM"));
	DaggerComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DaggerComp"));
	DaggerComp->SetupAttachment(Dagger);
	DaggerComp->SetRelativeScale3D(FVector(0.7f));
	DaggerComp->SetRelativeLocation(FVector(0, 0, 20));
	DaggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DaggerComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnDaggerBeginOverlap);
	MaceComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MaceComp"));
	MaceComp->SetupAttachment(Mace);
	MaceComp->SetRelativeScale3D(FVector(0.7, 0.7, 1.2));
	MaceComp->SetRelativeLocation(FVector(0, 0, 33.3333f));
	MaceComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnMaceBeginOverlap);
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	BossAnimInst = Cast<UBossAnim>(GetMesh()->GetAnimInstance());


}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurHP <= 0)
	{
		UGameplayStatics::SetGamePaused(this , true);
	}

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::OnDaggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CanHit)
	{
		BossFSM->Target->HP--;
		UE_LOG(LogTemp, Warning, TEXT("Dagger"));
		CanHit = false;
	}
	

}

void ABoss::OnMaceBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CanHit)
	{
		BossFSM->Target->HP--;
		UE_LOG(LogTemp, Warning, TEXT("Mace"));
		CanHit = false;
	}

}
