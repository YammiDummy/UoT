// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS(Blueprintable)
class UNIVERSEOFTANKS_API ATankPawn : public APawn
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

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
	void Yaw(float InYawValue);

private:
	float CurrentMoveForwardAxis = 0;
	float CurrentMoveRightAxis = 0;
};
