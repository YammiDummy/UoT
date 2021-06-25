// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

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

void ACannon::FireSpecial()
{
	
	if (!bIsReadyToFireSpecial)
	{
		GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Red, TEXT("STOP CLICKING!!! (special isn't ready)"), true);
		return;
	}

	bIsReadyToFireSpecial = false;

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FIREEEEEE!!! (special)"), true);

	GetWorld()->GetTimerManager().SetTimer(ReloadSpecialTimerHandle, this, &ACannon::ReloadSpecial, 5.f / FireRate, false);
}

void ACannon::Fire()
{
	if (!bIsReadyToFire)
	{
		GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Red, TEXT("STOP CLICKING!!! (cannon isnt ready)"), true);
		return;
	}

	else if (!Ammo)
	{
		GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Red, TEXT("STOP CLICKING!!! (ammo is empty) \n Press R to Reload"), true);
		return;
	};

	bIsReadyToFire = false;

	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FIREEEEEE!!! (projectile)"), true);
	}

	else if (Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FIREEEEEE!!! (trace)"), true);
	}

	else if (Type == ECannonType::FireAuto)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FIREEEEEE!!! (auto)")), true;   //Не успел реализовать
	}

	Ammo--;

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 0.5f / FireRate, false);

}

bool ACannon::IsReadyToFire() const
{
	return bIsReadyToFire;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	SetupCannonType(ECannonType::FireProjectile);

	Reload();
	ReloadSpecial();
	
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

void ACannon::ReloadSpecial()
{
	bIsReadyToFireSpecial = true;
}

void ACannon::ReloadAmmo()
{
	if (Type == ECannonType::FireProjectile)
	{
		Ammo = 6;
	}

	else if (Type == ECannonType::FireTrace)
	{
		Ammo = 30;
	}
	
	else if (Type == ECannonType::FireAuto)
	{
		Ammo = 10;
	}

	Reload();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("RELOADED!!!"));
}


void ACannon::SetupCannonType(ECannonType PickedCannonType)
{
	if (PickedCannonType == ECannonType::FireProjectile)
	{
		Type = ECannonType::FireProjectile;
	}

	else if (PickedCannonType == ECannonType::FireTrace)
	{
		Type = ECannonType::FireTrace;
	}

	else if (PickedCannonType == ECannonType::FireAuto)
	{
		Type = ECannonType::FireAuto;
	}

	ReloadAmmo();

}

