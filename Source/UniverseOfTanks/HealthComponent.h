#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStructs.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSEOFTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

    DECLARE_EVENT(UHealthComponent, FOnDie)
    DECLARE_EVENT_OneParam(UHealthComponent, FOnHealthChanged, float)

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health values")
    float MaxHealth = 4;

    UPROPERTY()
    float CurrentHealth;

public:
    FOnDie OnDie;
    FOnHealthChanged OnDamaged;

    bool TakeDamage(FDamageData DamageData);

    float GetHealth() const;

    float GetHealthState() const;

    void AddHealth(float AddiditionalHealthValue);
};