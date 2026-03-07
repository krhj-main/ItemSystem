// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveLoadManager.h"
#include "ItemSystemSaveGame.h"
#include "ItemSystemCharacter.h"
#include "InventoryComponent.h"
#include "StatusComponent.h"
#include "Kismet/GameplayStatics.h"

bool USaveLoadManager::SaveGame(const UObject* WorldContextObject, FString SlotName)
{
	UItemSystemSaveGame* SaveGameInstance = Cast<UItemSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(UItemSystemSaveGame::StaticClass()));

	if (!SaveGameInstance) return false;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(WorldContextObject->GetWorld(), 0);

	AItemSystemCharacter* Player = Cast<AItemSystemCharacter>(PlayerCharacter);
	if (!Player) return false;

	SaveGameInstance->PlayerLocation = Player->GetActorLocation();
	SaveGameInstance->PlayerRotation = Player->GetControlRotation();

	if (Player->StatusComponent)
	{
		SaveGameInstance->CurrentHealth = Player->StatusComponent->CurrentHealth;
		SaveGameInstance->CurrentStamina = Player->StatusComponent->CurrentStamina;
	}

	if (Player->InventoryComponent)
	{
		SaveGameInstance->InventoryItems = Player->InventoryComponent->Items;
	}

	SaveGameInstance->SaveDateTime = FDateTime::Now();
	SaveGameInstance->SaveSlotName = SlotName;

	bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("저장 성공 슬롯: %s"), *SlotName);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("저장 실패"));
	}
	return bSuccess;


}

bool USaveLoadManager::LoadGame(const UObject* WorldContextObject, FString SlotName)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UE_LOG(LogTemp,Warning, TEXT("세이브 파일 없음: %s"),*SlotName);
		return false;
	}

	UItemSystemSaveGame* LoadedGame = Cast<UItemSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (!LoadedGame) return false;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(WorldContextObject->GetWorld(),0);

	AItemSystemCharacter* Player = Cast<AItemSystemCharacter>(PlayerCharacter);
	if (!Player) return false;

	Player->SetActorLocation(LoadedGame->PlayerLocation);

	if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
	{
		PC->SetControlRotation(LoadedGame->PlayerRotation);
	}

	if (Player->StatusComponent)
	{
		Player->StatusComponent->CurrentHealth = LoadedGame->CurrentHealth;
		Player->StatusComponent->CurrentStamina = LoadedGame->CurrentStamina;

		float HealthPct = LoadedGame->CurrentHealth / Player->StatusComponent->MaxHealth;
		Player->StatusComponent->OnHealthChanged.Broadcast(LoadedGame->CurrentHealth, Player->StatusComponent->MaxHealth, HealthPct, "Health");

		float StaminaPct = LoadedGame->CurrentStamina / Player->StatusComponent->MaxStamina;
		Player->StatusComponent->OnStaminaChanged.Broadcast(
			LoadedGame->CurrentStamina,
			Player->StatusComponent->MaxStamina,
			StaminaPct, "Stamina");
	}

	if (Player->InventoryComponent)
	{
		Player->InventoryComponent->Items = LoadedGame->InventoryItems;
		Player->InventoryComponent->OnInventoryUpdated.Broadcast();
	}

	UE_LOG(LogTemp, Warning, TEXT("게임 로드 성공! 슬롯: %s (저장 시간: %s)"),
		*SlotName, *LoadedGame->SaveDateTime.ToString());

	return true;
}

bool USaveLoadManager::DoesSaveExist(FString SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

bool USaveLoadManager::DeleteSave(FString SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, 0);
		UE_LOG(LogTemp, Log, TEXT("세이브 삭제 : %s"),*SlotName);
		return true;
	}
	return false;
}