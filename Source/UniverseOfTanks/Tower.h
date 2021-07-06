// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Scorable.h"
#include "CoreMinimal.h"
#include "DamageTakerInterface.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

class ACannon;
class UBoxComponent;
class UArrowComponent;
class UScoreComponent;
class UHealthComponent;
class UScoreComponent;
class UStaticMeshComponent;

UCLASS()
class UNIVERSEOFTANKS_API ATower : public AActor, public IDamageTakerInterface, public IScorable
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UScoreComponent* ScoreComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.005f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;

	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TowerMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

public:

	ATower();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Fire();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaken(float DamageValue);

	UFUNCTION()
	virtual bool TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
	virtual float GetScore() override;

	UFUNCTION()
	virtual void SetScore(float Score) override;

protected:

	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();

	bool Test();
private:

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY()
	ACannon* Cannon;
};
