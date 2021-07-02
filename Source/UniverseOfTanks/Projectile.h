// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class UNIVERSEOFTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()


		DECLARE_EVENT(AProjectile, FAddScore)

public:
	AProjectile();


	FAddScore TargetDestroyed;

	void Start(class ACannon* InCannon);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ProjMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveRate = 0.005f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MoveRange = 1000.f;

	UPROPERTY();
	ACannon* LaunchCannon;
	FTimerHandle MovementTimerHandle;
	FTimerHandle LifeTimerHandle;

protected:
	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Move();

	UFUNCTION()
	void ToPool();
};
