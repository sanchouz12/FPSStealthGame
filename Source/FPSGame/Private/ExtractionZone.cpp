// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExtractionZone::AExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));

	OverlapComp->SetSimulatePhysics(false);
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RootComponent = OverlapComp;
	DecalComp->SetupAttachment(OverlapComp);
}

void AExtractionZone::BeginPlay()
{
	Super::BeginPlay();

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AExtractionZone::HandleOverlap);
}

void AExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Log, TEXT("Overlapped"));

	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);

	if (MyCharacter == nullptr)
	{
		return;
	}

	if (MyCharacter->bHasObjective)
	{
		// Instead of doing all logic here, we notify gamemode,
		// that the mission has been completed.
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

		if (GM && !GM->bIsOver)
		{
			GM->CompleteMission(MyCharacter, true);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSoud);
	}
}

