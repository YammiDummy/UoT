#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHealth = MaxHealth;
}

bool UHealthComponent::TakeDamage(FDamageData DamageData)
{
	float TakenDamageValue = DamageData.DamageValue;
	CurrentHealth -= TakenDamageValue;

	if (CurrentHealth <= 0)
	{
		if (OnDie.IsBound()) 
		{
			OnDie.Broadcast();
		}
		return true;
	}
	else
	{
		if (OnDamaged.IsBound())
		{
			OnDamaged.Broadcast(TakenDamageValue);
		}
		return false;
	}
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::AddHealth(float AddiditionalHealthValue)
{
	CurrentHealth += AddiditionalHealthValue;
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
}
