// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "TankPawn.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* BoxComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = BoxComponent;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BoxMesh->SetupAttachment(RootComponent);
	
	BoxMesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	BoxMesh->SetCollisionProfileName(FName("OverlapAll"));
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxMesh->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* Tank = Cast<ATankPawn>(OtherActor);
	if (Tank) {
		UE_LOG(LogTemp, Display, TEXT("Picked"));
		Tank->CannonClass = CannonClass;
		Tank->SetupCannon();
		Destroy();
	}

}
