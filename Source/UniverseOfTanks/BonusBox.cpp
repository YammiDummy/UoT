// Fill out your copyright notice in the Description page of Project Settings.


#include "BonusBox.h"
#include "TankPawn.h"
#include "Cannon.h"

// Sets default values
ABonusBox::ABonusBox()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* BoxComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = BoxComponent;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BoxMesh->SetupAttachment(RootComponent);

	BoxMesh->OnComponentBeginOverlap.AddDynamic(this, &ABonusBox::OnMeshOverlapBegin);
	BoxMesh->SetCollisionProfileName(FName("OverlapAll"));
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxMesh->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ABonusBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABonusBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Tank = Cast<ATankPawn>(OtherActor);
	if (Tank) {

		if (Tank->Cannon) {
			Tank->Cannon->MaxAmmo *= AmmoModifier;
			Tank->Cannon->ReloadAmmo();
		}

		if (Tank->SecCannon)
		{
			Tank->SecCannon->MaxAmmo *= AmmoModifier;
			Tank->SecCannon->ReloadAmmo();
		}	
		
		SetActorLocation(FVector(0, 0, -20));
		BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorld()->GetTimerManager().SetTimer(BonusTimer, this, &ABonusBox::EndBonus, BonusDuration, false);
	}
}


void ABonusBox::EndBonus()
{
	if (Tank->Cannon->CurrentAmmo > Tank->Cannon->MaxAmmo)
	{
		Tank->Cannon->CurrentAmmo = Tank->Cannon->MaxAmmo;
		Tank->Cannon->MaxAmmo /= AmmoModifier;
	}
	Destroy();
}
