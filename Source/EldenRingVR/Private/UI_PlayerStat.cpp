// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerStat.h"

#include "VRPlayer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UUI_PlayerStat::NativeConstruct()
{
	Super::NativeConstruct();

	AVRPlayer* player = Cast<AVRPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	HP->OnPressed.AddDynamic(this, &UUI_PlayerStat::HP_Press);
	MP->OnPressed.AddDynamic(this, &UUI_PlayerStat::MP_Press);
	SP->OnPressed.AddDynamic(this, &UUI_PlayerStat::SP_Press);
	INT->OnPressed.AddDynamic(this, &UUI_PlayerStat::INT_Press);
	STR->OnPressed.AddDynamic(this, &UUI_PlayerStat::STR_Press);
	decision->OnPressed.AddDynamic(this, &UUI_PlayerStat::decision_Press);
}

void UUI_PlayerStat::ShowStatSet()
{
	
}

void UUI_PlayerStat::HP_Press()
{
	
}

void UUI_PlayerStat::MP_Press()
{
}

void UUI_PlayerStat::SP_Press()
{
}

void UUI_PlayerStat::INT_Press()
{
}

void UUI_PlayerStat::STR_Press()
{
}

void UUI_PlayerStat::decision_Press()
{
}
