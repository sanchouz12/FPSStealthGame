// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSProjectile.h"

// Sets default values
ABlackHoleActor::ABlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Radius = 3000;
	Force = -2000;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	AffectedSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("AffectedSphereComp"));

	AffectedSphereComp->InitSphereRadius(Radius);

	RootComponent = MeshComp;
	SphereComp->SetupAttachment(MeshComp);
	AffectedSphereComp->SetupAttachment(MeshComp);

	bIsSucking = false;
}

// Called when the game starts or when spawned
void ABlackHoleActor::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AffectedSphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleActor::OnSuckInside);
	// Projectile generates hit event and destroys, therefore only in this event
	// it's possible to detect wheather the hole was hit or not.
	SphereComp->OnComponentHit.AddDynamic(this, &ABlackHoleActor::OnHit);
}

// Called every frame
void ABlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSucking)
	{
		TArray<UPrimitiveComponent*> OverlappedComps;
		AffectedSphereComp->GetOverlappingComponents(OverlappedComps);

		for (int i = 0; i < OverlappedComps.Num(); i++)
		{
			UPrimitiveComponent* Comp = OverlappedComps[i];

			if (Comp && Comp->IsSimulatingPhysics())
			{
				Comp->AddRadialForce(GetActorLocation(), Radius, Force, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
}

void ABlackHoleActor::OnSuckInside(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		OtherActor->Destroy();
	}
}

void ABlackHoleActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		AFPSProjectile* MyProjectile = Cast<AFPSProjectile>(OtherActor);

		if (MyProjectile)
		{
			bIsSucking = !bIsSucking;
		}
	}
}

