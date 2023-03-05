// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyMoveBack.h"

#include "AIController.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_EnemyMoveBack::UBTT_EnemyMoveBack()
{
	NodeName = TEXT("Move Back");
}

EBTNodeResult::Type UBTT_EnemyMoveBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	//
	FVector enemyLoc =  enemy->GetActorLocation();
	// //적을 마주보는 뒤쪽으로 이동한다
	// EPathFollowingRequestResult::Type result = OwnerComp.GetAIOwner()->MoveToLocation(enemyLoc - enemy->GetActorForwardVector()*300, 10);
	// //이동속도를 줄인다
	// enemy->ChangeSpeed(150);
	// //지정한 위치까지 도달하면 반환한다
	// goup:
	//
	// if(result == EPathFollowingRequestResult::AlreadyAtGoal)
	// {
	// 	return EBTNodeResult::Succeeded;
	// }
	// else
	// {
	// 	goto goup;
	// }
	enemy->ChangeSpeed(150);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("BackLocation"),enemyLoc - enemy->GetActorForwardVector()*300);
	return EBTNodeResult::Succeeded;
}
