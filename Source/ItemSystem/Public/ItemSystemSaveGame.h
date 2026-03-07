// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FItemData.h"
#include "ItemSystemSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UItemSystemSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UItemSystemSaveGame();

	//======================
	//player transform
	//======================

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FVector PlayerLocation;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FRotator PlayerRotation;

	//======================
	// heath and stamina
	//======================
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float CurrentStamina;

	//======================
	// inventory
	//======================
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TArray<FItemData> InventoryItems;

	//======================
	//meta
	//======================
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FDateTime SaveDateTime;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FString SaveSlotName;

	
};
