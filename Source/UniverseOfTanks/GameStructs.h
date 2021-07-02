#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use trace"),
	FireTriple = 2 UMETA(DisplayName = "Use auto")
};

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()
	
	UPROPERTY()
	float DamageValue;

	UPROPERTY();
	AActor* Instigator;

	UPROPERTY()
	AActor* DamageMaker;
};

USTRUCT(BlueprintType)
struct FScoreData
{
	GENERATED_BODY()
	
	UPROPERTY()
	float ScoreValue;

	UPROPERTY();
	AActor* ScoreTaker;

	UPROPERTY();
	AActor* ScoreGiver;
};
