// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveLoadManager.generated.h"

/**
 * 
 */
class UItemSystemSaveGame;

UCLASS()
class ITEMSYSTEM_API USaveLoadManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", meta = (WorldContext = "WorldContextObject"))
	static bool SaveGame(const UObject* WorldContextObject, FString SlotName = "Slot1");

	UFUNCTION(BlueprintCallable, Category = "SaveLoad", meta = (WorldContext = "WorldContextObject"))
	static bool LoadGame(const UObject* WorldContextObject, FString SlotName = "Slot1");

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	static bool DoesSaveExist(FString SlotName = "SLot1");

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	static bool DeleteSave(FString SlotName = "Slot1");


};
