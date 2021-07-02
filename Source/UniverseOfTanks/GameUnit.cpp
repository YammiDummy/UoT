// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUnit.h"
#include "ScoreComponent.h"
#include "HealthComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameStructs.h"
#include "Cannon.h"

// Sets default values
AGameUnit::AGameUnit()
{

}

void AGameUnit::Die()
{
	Destroy();
}

void AGameUnit::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void AGameUnit::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void AGameUnit::GiveScore(FScoreData ScoreData)
{
}

void AGameUnit::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}