// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ItemSystemCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UInventoryComponent;
class UStatusComponent;


UCLASS()
class ITEMSYSTEM_API AItemSystemCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AItemSystemCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//=======
	// componenets
	//========
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;


	//=========
	// enhanced input
	//=========
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InventoryAction;


	//==================
	//inventory
	//==================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	//==================
	//UI
	//==================
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	bool bIsInventoryOpen = false;

	//==================
	//interact	
	//==================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interacttion")
	float InteractionRange = 300.0f;

	//==================
	//interact ui
	//==================
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentInteractTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	FText CurrentInteractName;


	//==================
	// status health stamina
	//==================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	UStatusComponent* StatusComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	UInputAction* SprintAction;




	//=========
	//input functions
	//=========
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartInteract();
	void ToggleInventory();


	//interact ui
	void CheckForInteractable();

	//sprint
	void StartSprint();
	void StopSprint();



};
