#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FItemData.h"
#include "InventoryComponent.generated.h"


// 인벤토리 변경 시 UI에 알려주는 델리게이트(이벤트)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ITEMSYSTEM_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    // ========================
    // 인벤토리 데이터
    // ========================

    // 보유 아이템 배열
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FItemData> Items;

    // 최대 슬롯 수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxSlots = 20;

    // ========================
    // 기능 함수들
    // ========================

    // 아이템 추가 (성공 시 true)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(FItemData NewItem);

    // 아이템 제거
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FName ItemID);

    // 아이템 보유 여부 확인
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool HasItem(FName ItemID) const;

    // 아이템 검색 (찾으면 OutItem에 데이터 복사)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool FindItem(FName ItemID, FItemData& OutItem) const;

    // ========================
    // 이벤트
    // ========================

    // 인벤토리가 변할 때마다 발동 (나중에 UI 갱신에 사용)
    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryUpdated OnInventoryUpdated;
};