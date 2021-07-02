// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreComponent.h"

// Sets default values for this component's properties
UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentScore = 0;
}


void UScoreComponent::GiveScore(FScoreData ScoreData)
{

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("SCORE"), true);

}

