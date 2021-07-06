// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreComponent.h"

// Sets default values for this component's properties
UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentScore = 0.f;
}


float UScoreComponent::GetScore( )
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("GET"), true);
	return RewardScore;
}

void UScoreComponent::SetScore(float Score)
{
	CurrentScore += Score;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("%f"), CurrentScore), true);
}

