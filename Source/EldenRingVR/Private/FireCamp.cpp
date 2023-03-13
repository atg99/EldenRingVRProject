// Fill out your copyright notice in the Description page of Project Settings.


#include "FireCamp.h"

#include "PCPlayer.h"
#include "VRPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFireCamp::AFireCamp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	fireBox = CreateDefaultSubobject<UBoxComponent>(TEXT("fireBox"));
	fireBox->SetupAttachment(RootComponent);

	interaction = CreateDefaultSubobject<UWidgetComponent>(TEXT("interaction"));
	interaction->SetupAttachment(fireBox);
}

// Called when the game starts or when spawned
void AFireCamp::BeginPlay()
{
	Super::BeginPlay();
	fireBox->OnComponentBeginOverlap.AddDynamic(this, &AFireCamp::FireBeginOverlap);
	fireBox->OnComponentEndOverlap.AddDynamic(this, &AFireCamp::FireEndOverlap);
	interaction->SetVisibility(false);

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AFireCamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(interaction->IsVisible())
	{
		interaction->SetWorldRotation((playerPawn->GetActorLocation()-interaction->GetComponentLocation()).GetSafeNormal().Rotation());
	}

}

void AFireCamp::FireBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		//auto player = Cast<APCPlayer>(OtherActor);
		interaction->SetVisibility(true);

		AVRPlayer* vrPlayer = Cast<AVRPlayer>(OtherActor);
		if(vrPlayer)
		{
			vrPlayer->bStatInteraction = true;
		}
		
	}
}

void AFireCamp::FireEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		
		interaction->SetVisibility(false);
		AVRPlayer* vrPlayer = Cast<AVRPlayer>(OtherActor);
		if(vrPlayer)
		{
			vrPlayer->bStatInteraction = false;
		}
		
	}
}
