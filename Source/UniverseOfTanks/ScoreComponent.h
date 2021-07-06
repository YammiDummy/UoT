// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStructs.h"
#include "ScoreComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSEOFTANKS_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	float CurrentScore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reward")
	float RewardScore = 50.f;

public:

public:

	UScoreComponent();

	float GetScore();

	void SetScore(float Score);

};
