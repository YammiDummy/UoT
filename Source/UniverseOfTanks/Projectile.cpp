// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Cannon.h"
#include "DamageTakerInterface.h"
#include "IScorable.h"
#include "TankPawn.h"
#include "Tower.h"

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
	AActor* MyOwner = GetOwner();
	AActor* OwnerByOwner = MyOwner != nullptr ? MyOwner->GetOwner() : nullptr;
	if (OtherActor != MyOwner && OtherActor != OwnerByOwner)
	{
		IDamageTakerInterface* DamageTakerActor = Cast<IDamageTakerInterface>(OtherActor);
		IIScorable* ScoreGiverActor = Cast<IIScorable>(OtherActor);
		if (DamageTakerActor)
		{
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.Instigator = MyOwner;
			DamageData.DamageMaker = this;

			if (ScoreGiverActor)
			{
				FScoreData ScoreData;
				ScoreData.ScoreGiver = OtherActor;
				ScoreData.ScoreTaker = OwnerByOwner;
			}

			DamageTakerActor->TakeDamage(DamageData);
		}
	}
	if (!OtherActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FIREEEEEE!!! (triple)"), true);
		if (TargetDestroyed.IsBound())
		{
			TargetDestroyed.Broadcast();
		}
	}
	
	ToPool();
}

void AProjectile::Move()
{
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(NewLocation);
}

void AProjectile::ToPool()
{
	GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Green, TEXT("POOLED"), true);
	GetWorldTimerManager().ClearTimer(MovementTimerHandle);
	SetActorLocation(FVector(0, 0, -30));
	LaunchCannon->ProjPool.Push(this);
	
}

