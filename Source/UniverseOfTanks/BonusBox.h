// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BonusBox.generated.h"

UCLASS()
class UNIVERSEOFTANKS_API ABonusBox : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABonusBox();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components");
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components");
	float AmmoModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components");
	float BonusDuration;

	UPROPERTY()
	class ATankPawn* Tank;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndBonus();

private:
	UPROPERTY()
	FTimerHandle BonusTimer;

};
