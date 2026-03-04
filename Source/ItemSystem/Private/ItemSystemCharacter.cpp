// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystemCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"  
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InventoryComponent.h"
#include "InteractInterface.h"

// Sets default values
AItemSystemCharacter::AItemSystemCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//springarm 생성 및 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprinArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	//캡슐 상단 눈높이로 위치조정
	//기본 캡슐 절반높이는 88, 눈높이는 70
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));

	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;

	//카메라 생성 및 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	//캐릭터 무브먼트 컴포넌트 설정
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1500.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	//마우스 좌우 회전시 캐릭터 몸체 회전
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//인벤토리 컴포넌트 생성
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));


}

// begin play에서 입력 시스템 설정
// Called when the game starts or when spawned
void AItemSystemCharacter::BeginPlay()
{
	Super::BeginPlay();

	//enhanced input의 mapping context 를 플레이어에게 등록
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	
}

// Called every frame
void AItemSystemCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//입력 바인딩
// Called to bind functionality to input
void AItemSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//enhanced input component로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//wasd
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AItemSystemCharacter::Move);	
		}
		// mouse
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AItemSystemCharacter::Look);
		}

		//jump spacebar
		if(JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}

		//interact e
		if (InteractAction)
		{
			EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AItemSystemCharacter::StartInteract);
		}
	}

}

void AItemSystemCharacter::Move(const FInputActionValue& Value)
{
	//입력값을 2D 벡터로 받아옴
	const FVector2D MovementVector = Value.Get<FVector2D>();

	//컨트롤러의 회전값에서 Yaw만 가져옴
	//캐릭터가 바라보는 방향 기준으로 전후좌우 움직이게 하기 위함
	const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw,0.0f);

	//forward
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//right
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//AddmovementInput CharacterMovementComponent가 실제 이동 처리
	AddMovementInput(ForwardDir, MovementVector.Y); // WS
	AddMovementInput(RightDir, MovementVector.X); // AD
}


//==================================================================
//Look
//==================================================================
void AItemSystemCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	//AddControllerPitchInput, AddControllerYawInput은 ACharacter의 함수로, 컨트롤러의 회전값을 변경하는 함수
	//Yaw 좌우 Pitch 상하
	AddControllerPitchInput(LookVector.Y);
	AddControllerYawInput(LookVector.X);
}
//==================================================================
//StartInteract
//==================================================================
void AItemSystemCharacter::StartInteract()
{
	if (!Camera) return;

	//linetrace Start : camera location
	FVector TraceStart = Camera->GetComponentLocation();
	// end : camera forward vector * interaction range
	FVector TraceEnd = TraceStart + (Camera->GetForwardVector() * InteractionRange);

	//hit result
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); //자기 자신은 충돌에서 제외

	//linetrace start
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,  // Visibility 채널로 검사
		Params
	);

	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();

		//맞은 액터가 interactinterface를 구현한 액터인지 확인
		if (HitActor->Implements<UInteractInterface>())
		{
			//인터페이스의 인터랙트 함수 호출
			IInteractInterface::Execute_Interact(HitActor, this);

			UE_LOG(LogTemp, Warning, TEXT("Interacted with: %s"), *HitActor->GetName());

		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Hit %s but not interactable"), *HitActor->GetName());
		}

		// 여기에 아이템 줍기 로직이 들어갈 예정
		// 다음 단계에서 BP_ItemPickup과 연결합니다
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Nothing in range"));
	}

}
