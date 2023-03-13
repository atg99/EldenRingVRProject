// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerStatActor.generated.h"

UCLASS()
class ELDENRINGVR_API APlayerStatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerStatActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* rootComp;
	
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* statWindow;

	UFUNCTION()
	void SetStatWindow();

	UFUNCTION()
	void HideStatWindow();
	
private:

	UPROPERTY()
	class UUI_PlayerStat* statUI;
	
	UPROPERTY()
	class AVRPlayer* player;
};




