// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"



class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	

public:

	//===================
	// Perception
	//===================
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComp;

	UAISenseConfig_Sight* SightConfig;

	UAISenseConfig_Hearing* HearingConfig;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	//===================
	// Behavior Tree
	//===================
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	//==================
	// 상태관리
	//==================

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	AActor* DetectedPlayer;

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool HasDetectedPlayer() const;
};
