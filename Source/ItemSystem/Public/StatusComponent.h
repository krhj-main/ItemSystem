// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

//체력 스태미나 변경시 UI에 알리는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnStatusChanged,
	float, CurrentValue,
	float, MaxValue,
	float, Percentage,
	FName, StatusName
);

//체력 0
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEMSYSTEM_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//==========
	//health
	//==========	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Health")
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Status|Health")
	float CurrentHealth;

	//==========
	//damage
	//==========
	UFUNCTION(BlueprintCallable, Category = "Status|Health")
	void ApplyDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Status|Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Status|Health")
	bool IsAlive() const;

	//==========
	//stamina
	//==========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Status|Stamina")
	float CurrentStamina;

	UFUNCTION(BlueprintCallable, Category = "Status|Stamina")
	bool UseStamina(float Amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Stamina")
	float StaminaRegenRate = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Stamina")
	float StaminaRegenDelay = 1.5f;

	UPROPERTY(BlueprintReadOnly, Category = "Status|Stamina")
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Stamina")
	float SprintStaminaCost = 20.0f;

	UFUNCTION(BlueprintCallable, Category = "Status|Stamina")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "Status|Stamina")
	void StopSprint();

	//==========
	//delegates
	//==========

	UPROPERTY(BlueprintAssignable, Category = "Status")
	FOnStatusChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Status")
	FOnStatusChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Status")
	FOnDeath OnDeath;

private:
	float StaminaRegenTimer = 0.0f;

	float DefaultWalkSpeed = 0.0f;		
};
