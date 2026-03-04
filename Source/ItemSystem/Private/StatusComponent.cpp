// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;

	// walkspeed storage
	if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		DefaultWalkSpeed = Owner->GetCharacterMovement()->MaxWalkSpeed;
	}
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (bIsSprinting)
	{
		float Cost = SprintStaminaCost * DeltaTime;

		if (!UseStamina(Cost))
		{
			StopSprint();
		}
	}

	if (!bIsSprinting && CurrentStamina < MaxStamina)
	{
		if (StaminaRegenTimer > 0.0f)
		{
			StaminaRegenTimer -= DeltaTime;
		}
		else
		{
			CurrentStamina = FMath::Min(CurrentStamina + StaminaRegenRate * DeltaTime, MaxStamina);

			float Percentage = CurrentStamina / MaxStamina;
			OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina, Percentage, "Stamina");
		}
	}
}

//==================
//health
//==================
void UStatusComponent::ApplyDamage(float DamageAmount)
{
	if (!IsAlive()) return;

	CurrentHealth = FMath::Max(CurrentHealth - DamageAmount, 0.0f);

	float Percentage = CurrentHealth / MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, Percentage, "Health");

	UE_LOG(LogTemp, Warning, TEXT("데미지: %.1f | 남은 체력 : %.1f/%.1f"),
		DamageAmount, CurrentHealth, MaxHealth);

	if (!IsAlive())
	{
		OnDeath.Broadcast();
		UE_LOG(LogTemp, Error, TEXT("사망!"));
	}
}


void UStatusComponent::Heal(float HealAmount)
{
	if(!IsAlive()) return;

	CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);

	float Percentage = CurrentHealth / MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, Percentage, "Health");

	UE_LOG(LogTemp, Log, TEXT("회복: %.1f | 체력: %.1f/%.1f"),
		HealAmount, CurrentHealth, MaxHealth);
}


bool UStatusComponent::IsAlive() const
{
	return CurrentHealth > 0.0f;
}

//==================
//stamina
//==================
bool UStatusComponent::UseStamina(float Amount)
{
	if (CurrentStamina < Amount)
	{
		return false;
	}

	CurrentStamina = FMath::Max(CurrentStamina - Amount, 0.0f);

	StaminaRegenTimer = StaminaRegenDelay;

	float Percentage = CurrentStamina / MaxStamina;
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina, Percentage, "Stamina");

	return true;
}


void UStatusComponent::StartSprint()
{
	if (CurrentStamina <= 0.0f)
	{
		return;
	}
	bIsSprinting = true;

	if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * 1.8f;
	}
}
void UStatusComponent::StopSprint()
{
	bIsSprinting = false;
	if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	}
}
