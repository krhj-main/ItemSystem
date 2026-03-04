#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "FItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_BODY()

    // 아이템 고유 식별자 (예: "Key_Basement", "Ammo_Pistol")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID;

    // UI에 표시할 이름 (예: "지하실 열쇠")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText ItemName;

    // 아이템 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText ItemDescription;

    // UI 아이콘 (필요할 때만 로드하는 소프트 레퍼런스)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSoftObjectPtr<UTexture2D> ItemIcon;

    // 중첩(스택) 가능 여부 (탄약 = true, 열쇠 = false)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsStackable = false;

    // 최대 중첩 수량
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxStackCount = 1;

    // 현재 보유 수량
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 CurrentCount = 1;
};