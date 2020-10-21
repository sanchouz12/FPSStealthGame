// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPadActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API ALaunchPadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPadActor();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* StepFX;

	UPROPERTY(EditInstanceOnly, Category = "LaunchSettings")
	float LaunchX;

	UPROPERTY(EditInstanceOnly, Category = "LaunchSettings")
	float LaunchY;

	UPROPERTY(EditInstanceOnly, Category = "LaunchSettings")
	float LaunchZ;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffect();

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
