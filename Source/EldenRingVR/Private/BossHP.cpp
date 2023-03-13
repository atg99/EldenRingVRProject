// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHP.h"
#include "Boss.h"
#include "EngineUtils.h"


void UBossHP::NativeConstruct()
{
	Super::NativeConstruct();

	for (TActorIterator<ABoss> it(GetWorld()); it; ++it)
	{
		Boss = *it;
	}

	
}



void UBossHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);



	BossHP->SetPercent(Boss->CurHP / Boss->MaxHP);



}
