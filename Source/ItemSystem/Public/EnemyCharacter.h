// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UStatusComponent;

UCLASS()
class ITEMSYSTEM_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    // 적도 체력이 필요 (재사용!)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
    UStatusComponent* StatusComponent;

    // 공격 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackDamage = 20.0f;

    // 공격 사거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackRange = 200.0f;

    // 공격 쿨다운
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackCooldown = 2.0f;

    // 공격 실행
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void PerformAttack();

    // 순찰 속도 / 추적 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float PatrolSpeed = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ChaseSpeed = 400.0f;

private:
	bool bCanAttack = true;
	FTimerHandle AttackTimerHandle;
    void ResetAttack();
};
