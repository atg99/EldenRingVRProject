// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PlayerStat.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API UUI_PlayerStat : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* decision;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* HP;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* MP;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* SP;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* INT;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* STR;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* curHP;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* curMP;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* curSP;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* curINT;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* curSTR;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* nextHP;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* nextMP;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* nextSP;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* nextINT;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* nextSTR;

	UFUNCTION()
	void ShowStatSet();

private:

	UFUNCTION()
	void HP_Press();

	UFUNCTION()
	void MP_Press();

	UFUNCTION()
	void SP_Press();

	UFUNCTION()
	void INT_Press();

	UFUNCTION()
	void STR_Press();

	UFUNCTION()
	void decision_Press();

	UPROPERTY()
	int32 increaseHP;

	UPROPERTY()
	int32 increaseMP;

	UPROPERTY()
	int32 increaseSP;

	UPROPERTY()
	int32 increaseINT;

	UPROPERTY()
	int32 increaseSTR;

	UPROPERTY()
	class AVRPlayer* player;
};
