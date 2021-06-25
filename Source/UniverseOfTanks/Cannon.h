// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "Cannon.generated.h"

UCLASS()
class UNIVERSEOFTANKS_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh; 
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	uint8 AutoShotNumber = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type;

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void FireSpecial();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	bool IsReadyToFire() const; 
		
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void ReloadAmmo();


protected:
	// Called when the game starts or when spawned
	void Reload();
	void ReloadSpecial();
	void SetupCannonType(ECannonType CannonType);
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	

private:
	uint8 Ammo = 0;
	bool bIsReadyToFire = false;
	bool bIsReadyToFireSpecial = false;

	FTimerHandle ReloadTimerHandle;
	FTimerHandle ReloadSpecialTimerHandle;
};
