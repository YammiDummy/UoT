// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSEOFTANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATankPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Input")
	FVector GetMousePos() const;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Yaw(float AxisVaule);
	void FireSpecial();
	void Fire();
	void ReloadAmmo();
private:
	UPROPERTY()
	class ATankPawn* TankPawn;
	
	FVector CachedMousePos;
};
