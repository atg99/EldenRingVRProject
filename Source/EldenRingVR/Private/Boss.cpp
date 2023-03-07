// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "BossAttackPattern.h"
#include "BossAIController.h"
#include "BossAnim.h"
#include "Dagger.h"


// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossPattern = CreateDefaultSubobject<UBossAttackPattern>(TEXT("BossPattern"));

	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	FName RightHandSocket(TEXT("RightHandSocket"));
	//Dagger = CreateDefaultSubobject<ADagger>(TEXT("Dagger"));
	//ConstructorHelpers::FClassFinder<ADagger>BossDagger(TEXT("/Script/Engine.Blueprint'/Game/TW/Blueprint/BP_Dagger.BP_Dagger_C'"));
	//if (BossDagger.Succeeded())
	//{
	//	DaggerFac = BossDagger.Class;
	//}
	
	//Dagger->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, RightHandSocket);
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

	ConstructorHelpers::FClassFinder<UBossAnim> BossAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/TW/Animation/ABP_Boss.ABP_Boss_C'"));
	if (BossAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BossAnim.Class);
	}
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	
	BossAI = Cast<ABossAIController>(GetController());
	BossAI->TargetSet();
	BossAnimInst = Cast<UBossAnim>(GetMesh()->GetAnimInstance());

	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	//Dagger = GetWorld()->SpawnActor<ADagger>(DaggerFac, FVector(-3.855936f, 89.917361f, 50.000000f), GetActorRotation(), SpawnParams);
	//Dagger->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightHandSocket"));

	BossPattern->DaggerAttackThrow1();


}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

