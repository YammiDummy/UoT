// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTakerInterface.h"
#include "IScorable.h"
#include "GameUnit.generated.h"

class ACannon;
class UBoxComponent;
class UArrowComponent;
class UScoreComponent;
class UHealthComponent;
class UStaticMeshComponent;


UCLASS()
class UNIVERSEOFTANKS_API AGameUnit : public APawn, public IDamageTakerInterface, public IIScorable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGameUnit();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UScoreComponent* ScoreComponent;

	UPROPERTY()
	ACannon* Cannon;

protected:

public:

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Fire();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaken(float DamageValue);

	UFUNCTION()
	virtual void TakeDamage(FDamageData DamageData);

	UFUNCTION()
	virtual void GiveScore(FScoreData ScoreData);


};