// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordedEnemyAICon.h"
#include "Perception/AIPerceptionTypes.h"

ASwordedEnemyAICon::ASwordedEnemyAICon()
{
}

void ASwordedEnemyAICon::BeginPlay()
{
	Super::BeginPlay();

	
}

void ASwordedEnemyAICon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASwordedEnemyAICon::OnTargetUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	Super::OnTargetUpdate(Actor, Stimulus);
}