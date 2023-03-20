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

	warningMessage->SetVisibility(ESlateVisibility::Hidden);

	
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

	//플레이어 레벨을 설정
	playerLevel = player->playerLevel;
	playerLevelText->SetText(FText::AsNumber(playerLevel));

	curRunes = player->PlayerMoney;
	curMoney->SetText(FText::AsNumber(curRunes));

	curRunes = player->PlayerMoney;
	curMoney->SetText(FText::AsNumber(curRunes));

	//필요룬을 계산하고 표시한다
	CalculateNeedRune();

	//버튼을 유효화 한다
	HP->SetIsEnabled(true);
	MP->SetIsEnabled(true);
	SP->SetIsEnabled(true);
	INT->SetIsEnabled(true);
	STR->SetIsEnabled(true);
	decision->SetIsEnabled(true);
	
	PlayAnimation(showAnim);
	//this->SetVisibility(ESlateVisibility::Visible);
}

void UUI_PlayerStat::HP_Press()
{
	if(curRunes < needRunes)
	{
		warningMessage->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(warningAnim);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("HP"));
	increaseHP++;
	nextHP->SetText(FText::AsNumber(player->maxHP++));

	playerLevel++;
	playerLevelText->SetText(FText::AsNumber(playerLevel));

	//룬을 사용한다
	curRunes -= needRunes;
	curMoney->SetText(FText::AsNumber(curRunes));
	
	CalculateNeedRune();
}

void UUI_PlayerStat::MP_Press()
{
	if(curRunes < needRunes)
	{
		PlayAnimation(warningAnim);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("MP"));
	increaseMP++;
	nextMP->SetText(FText::AsNumber(player->maxMP++));

	playerLevel++;
	playerLevelText->SetText(FText::AsNumber(playerLevel));

	//룬을 사용한다
	curRunes -= needRunes;
	curMoney->SetText(FText::AsNumber(curRunes));
	
	CalculateNeedRune();
}

void UUI_PlayerStat::SP_Press()
{
	if(curRunes < needRunes)
	{
		PlayAnimation(warningAnim);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("SP"));
	increaseSP++;
	nextSP->SetText(FText::AsNumber(player->maxStamina++));

	playerLevel++;
	playerLevelText->SetText(FText::AsNumber(playerLevel));

	//룬을 사용한다
	curRunes -= needRunes;
	curMoney->SetText(FText::AsNumber(curRunes));
	
	CalculateNeedRune();
}

void UUI_PlayerStat::INT_Press()
{
	if(curRunes < needRunes)
	{
		PlayAnimation(warningAnim);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("INT"));
	increaseINT++;
	nextINT->SetText(FText::AsNumber(player->INT++));

	playerLevel++;
	playerLevelText->SetText(FText::AsNumber(playerLevel));

	//룬을 사용한다
	curRunes -= needRunes;
	curMoney->SetText(FText::AsNumber(curRunes));
	
	CalculateNeedRune();
}

void UUI_PlayerStat::STR_Press()
{
	if(curRunes < needRunes)
	{
		PlayAnimation(warningAnim);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("STR"));
	increaseSTR++;
	nextSTR->SetText(FText::AsNumber(player->STR++));

	playerLevel++;
	playerLevelText->SetText(FText::AsNumber(playerLevel));

	//룬을 사용한다
	curRunes -= needRunes;
	curMoney->SetText(FText::AsNumber(curRunes));
	
	CalculateNeedRune();
}

void UUI_PlayerStat::decision_Press()
{
	UE_LOG(LogTemp, Warning, TEXT("decision"));
	player->maxHP += increaseHP;
	player->maxMP += increaseMP;
	player->maxStamina += increaseSP;
	player->INT += increaseINT;
	player->STR += increaseSTR;

	//플레이어 레벨을 플레이어에 	playerLevel++;
	player->playerLevel = playerLevel;
	
	increaseHP = 0;
	increaseMP = 0;
	increaseSP = 0;
	increaseINT = 0;
	increaseSTR = 0;

	PlayAnimation(hideAnim);
	//this->SetVisibility(ESlateVisibility::Hidden);

	//버튼을 무효화 한다
	HP->SetIsEnabled(false);
	MP->SetIsEnabled(false);
	SP->SetIsEnabled(false);
	INT->SetIsEnabled(false);
	STR->SetIsEnabled(false);
	decision->SetIsEnabled(false);
}

void UUI_PlayerStat::CalculateNeedRune()
{
	needRunes = 600 + 18 * playerLevel;
	needMoney->SetText(FText::AsNumber(needRunes));
}
