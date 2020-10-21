// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPadActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
ALaunchPadActor::ALaunchPadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	RootComponent = OverlapComp;
	MeshComp->SetupAttachment(OverlapComp);

	LaunchX = 600.0f;
	LaunchY = 0.0f;
	LaunchZ = 2000.0f;
}

// Called when the game starts or when spawned
void ALaunchPadActor::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPadActor::HandleOverlap);
}

void ALaunchPadActor::PlayEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StepFX, GetActorLocation());
}

void ALaunchPadActor::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter)
	{
		PlayEffect();
		OtherCharacter->LaunchCharacter(FVector(LaunchX, LaunchY, LaunchZ), true, true);
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		PlayEffect();
		OtherComp->AddImpulse(FVector(LaunchX, LaunchY, LaunchZ), NAME_None, true);
	}
}

// Called every frame
void ALaunchPadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
