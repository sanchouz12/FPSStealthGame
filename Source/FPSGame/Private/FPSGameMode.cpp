// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "..\Public\FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	bIsOver = false;
}

void AFPSGameMode::BeginPlay()
{
	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bSuccess)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);
		bIsOver = true;

		APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());

		if (PC)
		{
			if (SpectatingViewPointClass == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("SpectatingViewPointClass is nullptr."));
				return;
			}

			TArray<AActor*> Actors;

			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewPointClass, Actors);

			if (Actors.Num())
			{
				AActor* NewViewTarget = Actors[0];

				PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}

		OnMissionComplete(InstigatorPawn, bSuccess);
	}
}
