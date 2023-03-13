// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerStat.h"

#include "VRPlayer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UUI_PlayerStat::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<AVRPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	UE_LOG(LogTemp, Warning, TEXT("Set"));
	HP->OnPressed.AddDynamic(this, &UUI_PlayerStat::HP_Press);
	MP->OnPressed.AddDynamic(this, &UUI_PlayerStat::MP_Press);
	SP->OnPressed.AddDynamic(this, &UUI_PlayerStat::SP_Press);
	INT->OnPressed.AddDynamic(this, &UUI_PlayerStat::INT_Press);
	STR->OnPressed.AddDynamic(this, &UUI_PlayerStat::STR_Press);
	decision->OnPressed.AddDynamic(this, &UUI_PlayerStat::decision_Press);
}

void UUI_PlayerStat::ShowStatSet()
{
	curHP->SetText(FText::AsNumber(player->maxHP));
	curMP->SetText(FText::AsNumber(player->maxMP));
	curSP->SetText(FText::AsNumber(player->maxStamina));
	curINT->SetText(FText::AsNumber(player->INT));
	curSTR->SetText(FText::AsNumber(player->STR));

	nextHP->SetText(FText::AsNumber(player->maxHP));
	nextMP->SetText(FText::AsNumber(player->maxMP));
	nextSP->SetText(FText::AsNumber(player->maxStamina));
	nextINT->SetText(FText::AsNumber(player->INT));
	nextSTR->SetText(FText::AsNumber(player->STR));

	this->SetVisibility(ESlateVisibility::Visible);
}

void UUI_PlayerStat::HP_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("HP"));
	increaseHP++;
	nextHP->SetText(FText::AsNumber(player->maxHP++));
}

void UUI_PlayerStat::MP_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("MP"));
	increaseMP++;
	nextMP->SetText(FText::AsNumber(player->maxMP++));
}

void UUI_PlayerStat::SP_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("SP"));
	increaseSP++;
	nextSP->SetText(FText::AsNumber(player->maxStamina++));
}

void UUI_PlayerStat::INT_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("INT"));
	increaseINT++;
	nextINT->SetText(FText::AsNumber(player->INT++));
}

void UUI_PlayerStat::STR_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("STR"));
	increaseSTR++;
	nextSTR->SetText(FText::AsNumber(player->STR++));
}

void UUI_PlayerStat::decision_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("decision"));
	player->maxHP += increaseHP;
	player->maxMP += increaseMP;
	player->maxStamina += increaseSP;
	player->INT += increaseINT;
	player->STR += increaseSTR;

	increaseHP = 0;
	increaseMP = 0;
	increaseSP = 0;
	increaseINT = 0;
	increaseSTR = 0;

	this->SetVisibility(ESlateVisibility::Hidden);
}
