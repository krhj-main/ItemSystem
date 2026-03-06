// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;

	// 이동 방향으로 회전 (순찰 시)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);

	// AI는 컨트롤러 회전을 사용 (SetFocus로 플레이어를 바라봄)
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::PerformAttack()
{
    UE_LOG(LogTemp, Error, TEXT("PerformAttack 호출됨, bCanAttack: %s"),
        bCanAttack ? TEXT("TRUE") : TEXT("FALSE"));

    if (!bCanAttack) return;

    bCanAttack = false;

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    UE_LOG(LogTemp, Error, TEXT("PlayerCharacter: %s"),
        PlayerCharacter ? TEXT("찾음") : TEXT("NULL"));

    if (PlayerCharacter)
    {
        float Distance = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());
        UE_LOG(LogTemp, Error, TEXT("플레이어와 거리: %.1f / 공격범위: %.1f"), Distance, AttackRange);

        if (Distance <= AttackRange)
        {
            if (UStatusComponent* TargetStatus =
                PlayerCharacter->FindComponentByClass<UStatusComponent>())
            {
                TargetStatus->ApplyDamage(AttackDamage);
                UE_LOG(LogTemp, Warning, TEXT("공격 성공! 데미지: %.1f"), AttackDamage);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("StatusComponent를 못 찾음!"));
            }
        }
    }

    GetWorldTimerManager().SetTimer(
        AttackTimerHandle, this, &AEnemyCharacter::ResetAttack, AttackCooldown, false);
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

