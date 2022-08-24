// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"
#include "Perception/PawnSensingComponent.h"
//#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"

// Sets default values
AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	CurrState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIGuard::HandleSeePawn);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AAIGuard::HandleHearNoise);

	OriginalRotation = GetActorRotation();
}

void AAIGuard::HandleSeePawn(APawn* Pawn)
{
	if (Pawn)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

		if (GM && !GM->bIsOver)
		{
			FVector Direction = Pawn->GetActorLocation() - GetActorLocation();
			Direction.Normalize();

			FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			NewRotation.Pitch = 0.0f;
			NewRotation.Roll = 0.0f;

			SetActorRotation(NewRotation);
			
			GM->CompleteMission(Pawn, false);
			ChangeState(EAIState::Alerted);
		}
	}
}

void AAIGuard::HandleHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	if (InstigatorPawn)
	{
		if (CurrState != EAIState::Alerted)
		{
			FVector Direction = Location - GetActorLocation();
			Direction.Normalize();

			FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			NewRotation.Pitch = 0.0f;
			NewRotation.Roll = 0.0f;

			SetActorRotation(NewRotation);

			GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrinetation);
			GetWorldTimerManager().SetTimer(TimerHandle_ResetOrinetation, this, &AAIGuard::ResetOrientation, 3.0f);

			ChangeState(EAIState::Suspicious);
		}
	}
}

void AAIGuard::ResetOrientation()
{
	if (CurrState != EAIState::Alerted)
	{
		SetActorRotation(OriginalRotation);

		ChangeState(EAIState::Idle);
	}
}

void AAIGuard::ChangeState(EAIState NewState)
{
	if (CurrState != NewState)
	{
		CurrState = NewState;
		OnStateChanged(CurrState);
	}
}

// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
