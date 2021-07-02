// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "UniverseOfTanks.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameStructs.h"


// Sets default values
	ATower::ATower()
	{
		BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower body"));
		RootComponent = BodyMesh;

		TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower tower"));
		TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

		CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
		CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

		HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
		HitCollider->SetupAttachment(TurretMesh);

		UStaticMesh* TowerMeshTemp = LoadObject<UStaticMesh>(this, *TowerMeshPath);
		if (TowerMeshTemp)
			TurretMesh->SetStaticMesh(TowerMeshTemp);

		UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
		if (BodyMeshTemp)
			BodyMesh->SetStaticMesh(BodyMeshTemp);

		HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
		HealthComponent->OnDie.AddUObject(this, &ATower::Die);
		HealthComponent->OnDamaged.AddUObject(this, &ATower::DamageTaken);
	}

	// Called when the game starts or when spawned
	void ATower::BeginPlay()
	{
		Super::BeginPlay();

		FActorSpawnParameters Params;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

		FTimerHandle TargetingTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATower::Targeting, TargetingRate, true, TargetingRate);
	}

	void ATower::Destroyed()
	{
		if (Cannon)
			Cannon->Destroy();
	}

	void ATower::Targeting()
	{
		if (IsPlayerInRange())
		{
			RotateToPlayer();
		}

		if (CanFire() && Cannon && Cannon->IsReadyToFire())
		{
			Fire();
		}
	}

	void ATower::RotateToPlayer()
	{
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
		FRotator CurrentRotation = TurretMesh->GetComponentRotation();
		TargetRotation.Pitch = CurrentRotation.Pitch;
		TargetRotation.Roll = CurrentRotation.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TargetingSpeed));


	}

	bool ATower::IsPlayerInRange()
	{
		return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
	}

	bool ATower::CanFire()
	{
		FVector TargetingDir = TurretMesh->GetForwardVector();
		FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
		DirToPlayer.Normalize();
		float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir, DirToPlayer)));
		return AimAngle <= Accurency;
	}