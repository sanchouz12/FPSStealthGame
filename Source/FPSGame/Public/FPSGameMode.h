// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spectating")
	TSubclassOf<AActor> SpectatingViewPointClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	bool bIsOver;

	AFPSGameMode();

	void CompleteMission(APawn* InstigatorPawn, bool bSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionComplete(APawn* InstigatorPawn, bool bSuccess);
};
