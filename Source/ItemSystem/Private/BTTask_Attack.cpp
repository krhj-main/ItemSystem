// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "EnemyCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack Player");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	UE_LOG(LogTemp, Warning, TEXT("Attack Task!"));
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController NULL!"));
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	UE_LOG(LogTemp, Error, TEXT("Pawn class : %s"),
		Pawn ? *Pawn->GetClass()->GetName() : TEXT("NULL"));

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Pawn);
	if (!Enemy)
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter Cast failed!"));
		return EBTNodeResult::Failed;
	}

	Enemy->PerformAttack();
	return EBTNodeResult::Succeeded;
	
}
