// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Pawn.h"
#include "GameUnit.h"
#include "TankPawn.generated.h"


class ATankPlayerController;
class USpringArmComponent;
class UCameraComponent;

UCLASS(Blueprintable)
class UNIVERSEOFTANKS_API ATankPawn : public AGameUnit
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	UArrowComponent* CannonSetupPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class AProjectile* test;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float YawSpeed = 0.5f; 
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float YawSensitivity = 0.5f;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TurretRotationSensitivity = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<class ACannon> CannonClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight (float InAxisValue);
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Yaw(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void SwapCannon();

	UFUNCTION()
	void GetScore();

	//UFUNCTION(BlueprintCallable, Category = "Action")
	//void ReloadAmmo();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetupCannon();

	UPROPERTY()
	ATankPlayerController* TankController = nullptr;
	
	UPROPERTY()
	ACannon* SecCannon = nullptr;

private:
	float CurrentMoveForwardAxis = 0.f;
	float CurrentMoveRightAxis = 0.f;
	float CurrentYawAxis = 0.f;
	float TargetYawAxis = 0.f;
	bool bIsReadyToFire = true;
};