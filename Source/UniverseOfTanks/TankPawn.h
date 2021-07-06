// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Pawn.h"
#include "DamageTakerInterface.h"
#include "Scorable.h"
#include "TankPawn.generated.h"

class ACannon;
class UBoxComponent;
class UAudioComponent;
class UArrowComponent;
class UScoreComponent;
class UHealthComponent;
class UScoreComponent;
class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class ATankPlayerController;
class UParticleSystemComponent;

UCLASS(Blueprintable)
class UNIVERSEOFTANKS_API ATankPawn : public APawn, public IDamageTakerInterface, public IScorable
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UScoreComponent* ScoreComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* DestroyVisualEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* DestroyAudioEffect;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
	TArray<FVector> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
	float MovementAccurency = 50;

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
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTurretLookAtPoint(FVector InPoint);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void SwapCannon();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetupCannon();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Fire();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaken(float DamageValue);

	UFUNCTION()
	virtual bool TakeDamage(FDamageData DamageData);

	UFUNCTION()
	TArray<FVector> GetPatrollingPoints();

	UFUNCTION()
	FVector GetEyesPosition() const;

	UFUNCTION()
	float GetMovementAccurency();

	UFUNCTION()
	virtual float GetScore() override;

	UFUNCTION()
	virtual void SetScore(float Score) override;

	UPROPERTY()
	ACannon* Cannon;
	
	UPROPERTY()
	ACannon* SecCannon = nullptr;
	
	UPROPERTY()
	float CurrentScore = 0;

private:
	float CurrentMoveForwardAxis = 0.f;
	float CurrentMoveRightAxis = 0.f;
	float CurrentYawAxis = 0.f;
	float TargetYawAxis = 0.f;

	FVector TurretLookAtPoint;

	bool bIsReadyToFire = true;
};