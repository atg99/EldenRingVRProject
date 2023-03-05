// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "EnemySword.h"
#include "TEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("weapon"));
	weapon->SetupAttachment(GetMesh(), FName("sword"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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
	//con->aiBehavior = behaviorTree;
	
	AEnemySword* sword = Cast<AEnemySword>(weapon->GetChildActor());
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
	// 	//속도를 천천히 줄인다
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
	AEnemySword* sword = Cast<AEnemySword>(weapon->GetChildActor());
	sword->boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	
	sword->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	sword->sword->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	
	sword->sword->SetSimulatePhysics(true);
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
	PlayAnimMontage(enemyAnim, 1, TEXT("Attack2"));
}

void AEnemyBase::BattleStart()
{
	PlayAnimMontage(enemyAnim, 1, TEXT("BattleStart"));
}

void AEnemyBase::DecreaseSpeed(float len, float speed)
{
	// len 에 deltaTime을 곱한다
	time = len;
	eSpeed = speed;
	bDecrease = true;
}

void AEnemyBase::IncreaseSpeed(float len, float speed)
{
	// len 에 deltaTime을 곱한다
	time = len;
	eSpeed = speed;
	bIncrease = true;
}

void AEnemyBase::OnDamaged(float damage)
{
	enemyHP -= damage;
	con->SetbHitValue();
	if(enemyHP <= 0)
	{
		EnemyDie();
	}
}

void AEnemyBase::EnemyDie()
{
	con->SetbDieValue();
}

