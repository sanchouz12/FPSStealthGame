// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle, Suspicious, Alerted
};

UCLASS()
class FPSGAME_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	FRotator OriginalRotation;

	FTimerHandle TimerHandle_ResetOrinetation;

	EAIState CurrState;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleSeePawn(APawn* Pawn);

	UFUNCTION()
	void HandleHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetOrientation();

	UFUNCTION()
	void ChangeState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
