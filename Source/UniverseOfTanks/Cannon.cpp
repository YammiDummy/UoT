// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>("CannonMesh");
	CannonMesh -> SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("Spawn point");
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

}

void ACannon::Fire()
{
	if (!bIsReadyToFire)
	{
		GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Red, TEXT("STOP CLICKING!!! (cannon isnt ready)"), true);
		return;
	}

	else if (!CurrentAmmo)
	{
		GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Red, TEXT("STOP CLICKING!!! (ammo is empty)"), true);
		return;
	};

	bIsReadyToFire = false;

	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FIREEEEEE!!! (projectile)"), true);

		if (ProjPool.Num() > 0) {
			Projectile = ProjPool.Pop();
		}

		else 
		{ 
			Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation()); 
		}
		
		if (Projectile)
		{
			Projectile->MoveRange=FireRange;
			Projectile->Start(this);
		}
		GetWorld()->GetTimerManager().SetTimer(SingleFireTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	}

	else if (Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FIREEEEEE!!! (trace)"), true);
		FHitResult HitResult;
		FVector StartPoint = ProjectileSpawnPoint->GetComponentLocation();
		FVector EndPoint = StartPoint + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName("FireTrace"), true, this);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), StartPoint, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			if (HitResult.Actor.IsValid())
			{
				HitResult.Actor->Destroy();
			}

		}
		else {
			DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 0.5f, 0, 5);
		}
		GetWorld()->GetTimerManager().SetTimer(SingleFireTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	}

	else if (Type == ECannonType::FireTriple)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FIREEEEEE!!! (triple)"), true);

		Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->MoveRange = FireRange;
			Projectile->Start(this);
		}
		FRotator RotTemp = ProjectileSpawnPoint->GetComponentRotation();
		RotTemp.Yaw += 10;
		Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), RotTemp);
		if (Projectile)
		{
			Projectile->MoveRange = FireRange;
			Projectile->Start(this);
		}
		RotTemp.Yaw -= 20;
		Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), RotTemp);
		if (Projectile)
		{
			Projectile->MoveRange = FireRange;
			Projectile->Start(this);
		}
		GetWorld()->GetTimerManager().SetTimer(SingleFireTimerHandle, this, &ACannon::Reload, 1.5f / FireRate, false);
	}

	CurrentAmmo--;

}

bool ACannon::IsReadyToFire() const
{
	return bIsReadyToFire;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	ReloadAmmo();
}

void ACannon::EndPlay(EEndPlayReason::Type Reason)
{
	ReloadTimerHandle.Invalidate();

	Super::EndPlay(Reason);

}

void ACannon::Reload()
{
	bIsReadyToFire = true;
	
}

void ACannon::SingleFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FIREEEEEE!!! (auto)")), true;
}

void ACannon::ReloadAmmo()
{

	CurrentAmmo = MaxAmmo;
	Reload();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("RELOADED!!!"));

}

