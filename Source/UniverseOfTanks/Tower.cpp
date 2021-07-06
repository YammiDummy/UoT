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
#include "ScoreComponent.h"
#include "DrawDebugHelpers.h"


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

	ScoreComponent = CreateDefaultSubobject<UScoreComponent>(TEXT("Score component"));
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
		if (CanFire() && Cannon && Cannon->IsReadyToFire())
		{
			Fire();
		}
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
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir, DirectionToPlayer)));
	if (AimAngle <= Accurency)
	{
		bool bCanSeePlayer = false;

		FCollisionQueryParams Params(FName(TEXT("EnemyEyes")), true, this);
		Params.AddIgnoredActor(this);
		Params.bReturnPhysicalMaterial = false;
		FHitResult HitResult;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CannonSetupPoint->GetComponentLocation(), PlayerPawn->GetActorLocation(), ECC_Visibility, Params))
		{
			if (HitResult.Actor.Get()) {
				DrawDebugLine(GetWorld(), CannonSetupPoint->GetComponentLocation(), HitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
				return (HitResult.Actor.Get() == PlayerPawn);
			}
		}
	}
	return false;
}
//bool ATower::Test()
//{
//	if (PlayerPawn)
//	{
//		FVector PlayerPawnLocation = PlayerPawn->GetActorLocation();
//		bool bCanSeePlayer = false;
//
//		if (FVector::Distance(PlayerPawnLocation, GetActorLocation()) < TargetingRange)
//		{
//			FCollisionQueryParams Params(FName(TEXT("EnemyEyes")), true, this);
//			Params.AddIgnoredActor(this);
//			Params.bReturnPhysicalMaterial = false;
//			FHitResult HitResult;
//
//			if (GetWorld()->LineTraceSingleByChannel(HitResult, CannonSetupPoint->GetComponentLocation(), PlayerPawnLocation, ECC_Visibility, Params))
//			{
//				if (HitResult.Actor.Get()) {
//					DrawDebugLine(GetWorld(), CannonSetupPoint->GetComponentLocation(), HitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
//					bCanSeePlayer = HitResult.Actor.Get() == PlayerPawn;
//				}
//			}
//
//		}
//
//		if (bCanSeePlayer)
//		{
//			DrawDebugLine(GetWorld(), GetActorLocation(), PlayerPawnLocation, FColor::Green, false, 0.1f, 0, 5);
//			AiTank->SetTurretLookAtPoint(PlayerPawnLocation);
//
//			FVector DirectionToPlayer = PlayerPawnLocation - GetActorLocation();
//			DirectionToPlayer.Normalize();
//			FVector TurretDirection = AiTank->TurretMesh->GetForwardVector();
//			float AngleToPlayer = FMath::Acos(FVector::DotProduct(TurretDirection, DirectionToPlayer));
//
//			if (FMath::RadiansToDegrees(AngleToPlayer) < Accurency)
//			{
//				AiTank->Fire();
//			}
//		}
//		else
//		{
//
//			TurretMesh->SetWorldRotation(FMath::Lerp(TurretMesh->GetComponentRotation(), TurretMesh->GetComponentRotation(), TargetingSpeed));;
//
//		}
//	}
//}

void ATower::Die()
{
	Destroy();
}

void ATower::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

bool ATower::TakeDamage(FDamageData DamageData)
{
	return HealthComponent->TakeDamage(DamageData);
}

float ATower::GetScore()
{
	return ScoreComponent->GetScore();
}

void ATower::SetScore(float Score)
{
	ScoreComponent->SetScore(Score);
}

void ATower::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}