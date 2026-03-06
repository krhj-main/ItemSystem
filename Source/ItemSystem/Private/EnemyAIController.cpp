// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Perception/AIperceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyAIController::AEnemyAIController()
{
	// Perception Component 생성
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerceptionComp);

	//시야설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 2000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComp->ConfigureSense(*SightConfig);

	//청각설정
	HearingConfig = CreateDefaultSubobject <UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 2000.0f;
	HearingConfig->SetMaxAge(3.0f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;

	AIPerceptionComp->ConfigureSense(*HearingConfig);

	AIPerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());

	AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (!Actor) continue;

		if (Actor->ActorHasTag("Player"))
		{
			FActorPerceptionBlueprintInfo Info;
			AIPerceptionComp->GetActorsPerception(Actor, Info);

			bool bCurrentlySensed = false;
			for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
			{
				if (Stimulus.WasSuccessfullySensed())
				{
					bCurrentlySensed = true;
					break;
				}
			}
			if (bCurrentlySensed)
			{
				DetectedPlayer = Actor;

				SetFocus(Actor);

				if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn()))
				{
					Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->ChaseSpeed;
				}

				if (GetBlackboardComponent())
				{
					GetBlackboardComponent()->SetValueAsObject("TargetActor", Actor);
					GetBlackboardComponent()->SetValueAsVector("LastKnownLocation",Actor->GetActorLocation());

					// === 디버그 로그 추가 ===
					UE_LOG(LogTemp, Error, TEXT("BB 키 개수: %d"),
						GetBlackboardComponent()->GetNumKeys());

					GetBlackboardComponent()->SetValueAsObject("TargetPlayer", Actor);

					UObject* CheckValue = GetBlackboardComponent()->GetValueAsObject("TargetPlayer");
					UE_LOG(LogTemp, Error, TEXT("저장 시도한 Actor: %s"), *Actor->GetName());
					UE_LOG(LogTemp, Error, TEXT("BB에서 읽은 값: %s"),
						CheckValue ? *CheckValue->GetName() : TEXT("NULL"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("BlackboardComponent가 NULL!"));
				}

				UE_LOG(LogTemp,Warning,TEXT("플레이어 감지"));
			}
			else
			{
				DetectedPlayer = nullptr;

				ClearFocus(EAIFocusPriority::Gameplay);

				if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn()))
				{
					Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->PatrolSpeed;
				}

				if (GetBlackboardComponent())
				{
					GetBlackboardComponent()->ClearValue("TargetActor");
				}
				UE_LOG(LogTemp, Log, TEXT("플레이어를 놓침"));
			}
		}
	}
}

bool AEnemyAIController::HasDetectedPlayer() const
{
	return DetectedPlayer != nullptr;
}
