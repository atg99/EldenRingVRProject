// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "EnemySword.h"
#include "TEnemyAIController.h"
#include "VectorTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInterface.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("weapon"));
	weapon->SetupAttachment(GetMesh(), FName("sword"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	originMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OriginMesh"));
	originMesh->SetupAttachment(RootComponent);
	originMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	poseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("poseableMesh"));
	poseableMesh->SetupAttachment(RootComponent);
	poseableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//behaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("behavior"));
	//AIControllerClass = ATEnemyAIController::StaticClass();;

	//aiPercep = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(TEXT("weapon"), PBO_None);
		
	con = Cast<ATEnemyAIController>(GetController());

	poseableMesh->SetVisibility(false);
	originMesh->SetVisibility(false);
	
	//con->aiBehavior = behaviorTree;
	
	sword = Cast<AEnemySword>(weapon->GetChildActor());
	if(sword)
	{
		sword->ownerEnemy = this;
	}
	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	bDecrease = false;
	bIncrease = false;
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if(bDecrease)
	// {
	// 	//????????? ????????? ?????????
	// 	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(600, eSpeed, DeltaTime*time);
	// 	if(GetCharacterMovement()->MaxWalkSpeed <= eSpeed)
	// 	{
	// 		GetCharacterMovement()->MaxWalkSpeed = eSpeed;
	// 		bDecrease = false;
	// 	}
	// }
	// else if(bIncrease)
	// {
	// 	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(300, eSpeed, DeltaTime*time);
	// 	if(GetCharacterMovement()->MaxWalkSpeed >= eSpeed)
	// 	{
	// 		GetCharacterMovement()->MaxWalkSpeed = eSpeed;
	// 		bIncrease = false;
	// 	}
	// }
	if(bMerge)
	{
		mergeTime += DeltaTime*0.5;
		
		
		GetMesh()->SetWorldLocation(FMath::Lerp(curTransform.GetLocation(), originTransform.GetLocation(), mergeTime), false, nullptr, ETeleportType::TeleportPhysics);
		//GetMesh()->SetWorldRotation(FMath::Lerp(curTransform.GetRotation(), originTransform.GetRotation(), mergeTime), false, nullptr, ETeleportType::TeleportPhysics);
		//UE::Geometry::Lerp()
		if(mergeTime >= 2)
		{
			
			GetMesh()->SetWorldLocation(originTransform.GetLocation(), false, nullptr, ETeleportType::TeleportPhysics);
			GetMesh()->SetWorldRotation(originTransform.GetRotation(), false, nullptr, ETeleportType::TeleportPhysics);
			bMerge = false;
			mergeTime = 0;
			GetMesh()->SetSimulatePhysics(false);
			GetMesh()->bPauseAnims = false;
			//GetMesh()->SetCollisionProfileName(TEXT("EnemyPreset"));
			//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}

	if(bMergeV2)
	{
		EnemyMergeV2();
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::ChangeSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void AEnemyBase::DeathAction()
{
	PlayAnimMontage(enemyAnim, 1, TEXT("Death"));
}

void AEnemyBase::SetRagdoll()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	sword->SetRagdoll();
	//weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	
	GetMesh()->SetSimulatePhysics(true);
}

void AEnemyBase::DamageReact_F()
{
	PlayAnimMontage(enemyAnim, 1, TEXT("HitF"));
}

void AEnemyBase::AttackPatten1()
{
	PlayAnimMontage(enemyAnim, 1, TEXT("Attack1"));
}

void AEnemyBase::AttackPatten2()
{
	PlayAnimMontage(enemyAnim, 1.5, TEXT("Attack2"));
}

void AEnemyBase::BattleStart()
{
	PlayAnimMontage(enemyAnim, 1, TEXT("BattleStart"));
}

void AEnemyBase::PlayEnemyAnim(FName session, float rate)
{
	PlayAnimMontage(enemyAnim, rate, session);
}

void AEnemyBase::DecreaseSpeed(float len, float speed)
{
	// len ??? deltaTime??? ?????????
	time = len;
	eSpeed = speed;
	bDecrease = true;
}

void AEnemyBase::IncreaseSpeed(float len, float speed)
{
	// len ??? deltaTime??? ?????????
	time = len;
	eSpeed = speed;
	bIncrease = true;
}

void AEnemyBase::OnDamaged(float damage)
{
	enemyHP -= damage;
	
	if(enemyHP <= 0)
	{
		EnemyDie();
		return;
	}
	con->SetbHitValue();
	con->StopMoveTo();
	DamageReact_F();
}

void AEnemyBase::EnemyDie()
{
	con->SetbDieValue();
}

void AEnemyBase::SetSwordDoOnce()
{
	sword->bDoOnce = false;
}

void AEnemyBase::Dash(float force)
{
	LaunchCharacter(GetActorForwardVector()*force, true, false);
}

void AEnemyBase::Desmemberment(FName hitBone)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *hitBone.ToString()));
	
	if(GetMesh()->IsSimulatingPhysics(hitBone)&&(hitBone!="head"&&hitBone!="spine_03"&&hitBone!="spine_02"&&hitBone!="neck_01"))
	{
		GetMesh()->BreakConstraint(FVector(0),FVector(0), hitBone);
	}
	else if(hitBone == "spine_01" || hitBone == "thigh_l" ||  hitBone == "thigh_r" || hitBone == "calf_l" ||
		hitBone == "calf_r" || hitBone == "foot_l" || hitBone == "foot_r" /*|| hitBone == "head" || hitBone == "neck_01"||hitBone=="spine_03"*/||hitBone == "spine_02")
	{
		//SetRagdoll();
		Crawl();
		GetMesh()->SetAllBodiesBelowSimulatePhysics(hitBone, true);
		// FTimerHandle TimerHandle_Ragdoll;
		// GetWorldTimerManager().SetTimer(TimerHandle_Ragdoll, FTimerDelegate::CreateLambda([this]()->void
		// {
		// 	GetMesh()->SetSimulatePhysics(false);	
		// }), 0.2f, false);
	}
	else if(!GetMesh()->IsSimulatingPhysics(hitBone))
	{
		GetMesh()->SetAllBodiesBelowSimulatePhysics(hitBone, true);
		//GetMesh()->BreakConstraint(FVector(0),FVector(0), hitBone);
	}
	else if(GetMesh()->IsSimulatingPhysics(hitBone)&&(hitBone == "head" || hitBone == "neck_01"||hitBone=="spine_03"))
	{
		Crawl();
	}
	

	FVector cam = UKismetMathLibrary::GetForwardVector(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->K2_GetActorRotation());
	FVector camForward = cam * FMath::RandRange(1000.f, 3000.f);
	FVector up = GetActorUpVector()*FMath::RandRange(500.f, 1500.f);
	FVector socketLoc = GetMesh()->GetSocketLocation(hitBone);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	GetMesh()->AddImpulseAtLocation(camForward + up, socketLoc, hitBone);

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	FHitResult result;
	
	GetWorld()->LineTraceSingleByChannel(result, socketLoc, socketLoc+FVector(0,0,-1)*1500, ECC_Visibility
	, params);

	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), bloodDecal, FVector(-63, -128, -128), result.Location, FVector(0,0,1).Rotation(), 60);
}

void AEnemyBase::Crawl()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Crawlllllllllll")));
	sword->SetRagdoll();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ChangeSpeed(500);

	con->SetbCrawl();

	//con->DisableBT();
	//con->MoveToPlayer();
	PlayEnemyAnim(TEXT("Crawl"), 5);
}

void AEnemyBase::EnemyMerge()
{
	StopAnimMontage();
	GetMesh()->bPauseAnims = true;
	GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//GetMesh()->SetCollisionProfileName(TEXT("ReverseMeshPreset"));
	originTransform = originMesh->GetComponentTransform();
		
	curTransform = GetMesh()->GetComponentTransform();
	bMerge = true;
}

void AEnemyBase::SetPoseableMeshToGetMesh()
{
	if(!poseableMesh){ return; }
	
	for (auto PoseableBone : poseableMesh->GetAllSocketNames())
	{
		poseableMesh->SetBoneTransformByName(PoseableBone, GetMesh()->GetSocketTransform(PoseableBone), EBoneSpaces::WorldSpace);
	}

	EnemyMergeV2Setup();
}

void AEnemyBase::EnemyMergeV2Setup()
{
	GetMesh()->SetVisibility(false);
	poseableMesh->SetVisibility(true);
	bMergeV2 = true;
}

void AEnemyBase::EnemyMergeV2()
{
	mergeV2LerpTime = GetWorld()->DeltaTimeSeconds;
	int a = 0;
	for (auto Bone : poseableMesh->GetAllSocketNames())
	{
		FTransform From2 = poseableMesh->GetSocketTransform(Bone);
		FTransform To = originMesh->GetSocketTransform(Bone);
		FTransform lerpTransform = UKismetMathLibrary::TLerp(From2, To, mergeV2LerpTime);
		poseableMesh->SetBoneTransformByName(Bone, lerpTransform, EBoneSpaces::WorldSpace);
		a++;
	}

	FTimerHandle timerHandle_merge;
	GetWorld()->GetTimerManager().SetTimer(timerHandle_merge, FTimerDelegate::CreateLambda([this]()->void
	{
		mergeV2LerpTime = 0;
		bMergeV2 = false;
		EnemyMergeEnd();
	}), 3 ,false);
	// if(mergeV2LerpTime >= 1)
	// {
	// 	mergeV2LerpTime = 0;
	// 	bMergeV2 = false;
	// 	EnemyMergeEnd();
	// }
}

void AEnemyBase::EnemyMergeEnd() 
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("MergeV2 Done")));
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetWorldTransform(originMesh->GetComponentTransform());
	GetMesh()->SetVisibility(true);
	poseableMesh->SetVisibility(false);
}

