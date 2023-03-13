// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "BossHP.generated.h"

/**
 * 
 */
UCLASS()
class ELDENRINGVR_API UBossHP : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere, meta = (bindwidget))
		UProgressBar* BossHP;

	class ABoss* Boss;
	
};
