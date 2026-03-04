#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddItem(FItemData NewItem)
{
    // 1단계: 스택 가능한 아이템이면 기존 슬롯에 합치기 시도
    if (NewItem.bIsStackable)
    {
        for (FItemData& ExistingItem : Items)
        {
            if (ExistingItem.ItemID == NewItem.ItemID)
            {
                int32 SpaceLeft = ExistingItem.MaxStackCount - ExistingItem.CurrentCount;
                if (SpaceLeft > 0)
                {
                    int32 AmountToAdd = FMath::Min(NewItem.CurrentCount, SpaceLeft);
                    ExistingItem.CurrentCount += AmountToAdd;

                    UE_LOG(LogTemp, Log, TEXT("아이템 스택: %s (수량: %d)"),
                        *ExistingItem.ItemID.ToString(), ExistingItem.CurrentCount);

                    OnInventoryUpdated.Broadcast();
                    return true;
                }
            }
        }
    }

    // 2단계: 새 슬롯에 추가
    if (Items.Num() < MaxSlots)
    {
        Items.Add(NewItem);

        UE_LOG(LogTemp, Log, TEXT("아이템 추가: %s (슬롯 %d/%d)"),
            *NewItem.ItemID.ToString(), Items.Num(), MaxSlots);

        OnInventoryUpdated.Broadcast();
        return true;
    }

    // 3단계: 인벤토리 가득 참
    UE_LOG(LogTemp, Warning, TEXT("인벤토리 가득 참! %s 추가 실패"), *NewItem.ItemID.ToString());
    return false;
}

bool UInventoryComponent::RemoveItem(FName ItemID)
{
    for (int32 i = 0; i < Items.Num(); i++)
    {
        if (Items[i].ItemID == ItemID)
        {
            UE_LOG(LogTemp, Log, TEXT("아이템 제거: %s"), *ItemID.ToString());
            Items.RemoveAt(i);
            OnInventoryUpdated.Broadcast();
            return true;
        }
    }
    return false;
}

bool UInventoryComponent::HasItem(FName ItemID) const
{
    for (const FItemData& Item : Items)
    {
        if (Item.ItemID == ItemID)
        {
            return true;
        }
    }
    return false;
}

bool UInventoryComponent::FindItem(FName ItemID, FItemData& OutItem) const
{
    for (const FItemData& Item : Items)
    {
        if (Item.ItemID == ItemID)
        {
            OutItem = Item;
            return true;
        }
    }
    return false;
}
