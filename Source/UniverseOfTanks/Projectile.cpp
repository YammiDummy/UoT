// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Cannon.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	ProjMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjMesh->SetupAttachment(RootComponent);
	ProjMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

}

void AProjectile::Start(ACannon* InCannon)
{
	LaunchCannon = InCannon;
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
	GetWorld()->GetTimerManager().SetTimer(LifeTimerHandle, this, &AProjectile::ToPool, 2.f, false);
	//SetLife(MoveRange / MoveSpeed);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
	OtherActor->Destroy();
	ToPool();
}

void AProjectile::Move()
{
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(NewLocation);
}

void AProjectile::ToPool()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("POOLED"), true);
	GetWorldTimerManager().ClearTimer(MovementTimerHandle);
	SetActorLocation(FVector(0, 0, -30));
	LaunchCannon->ProjPool.Push(this);
	
}

