// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::PerformAttack()
{
	if (!bCanAttack) return;
	
	bCanAttack = false;

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor != this && Actor->ActorHasTag("Player"))
		{
			if (UStatusComponent* TargetStatus = Actor->FindComponentByClass<UStatusComponent>())
			{
				TargetStatus->ApplyDamage(AttackDamage);
				UE_LOG(LogTemp, Log, TEXT("적이 플레이어를 공격! 데미지: %.1f"), AttackDamage);
			}
		}
	}

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyCharacter::ResetAttack, AttackCooldown, false);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::ResetAttack()
{
	bCanAttack = true;
}

