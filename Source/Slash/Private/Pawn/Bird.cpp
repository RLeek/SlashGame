// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "InputActionValue.h" 
#include "EnhancedInputComponent.h" 
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h" 

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	capsule->SetCapsuleHalfHeight(20.f);
	capsule->SetCapsuleRadius(20.f);
	SetRootComponent(capsule);

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	mesh->SetupAttachment(capsule);

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERABOON"));
	cameraBoom->TargetArmLength = 300.0f;
	cameraBoom->SetupAttachment(capsule);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	camera->SetupAttachment(cameraBoom);
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();

	// Cast the controller to a player controller
	if (APlayerController* playerController = Cast<APlayerController>(GetController())) 
	{
		if (UEnhancedInputLocalPlayerSubsystem* system = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			system->AddMappingContext(birdContext, 0);
		}
	}

	// Get the Enanced local player substem
	// Add 

}

void ABird::Move(const FInputActionValue& Value) 
{
	const float input = Value.Get<float>();
	if (Controller != nullptr) {
		FVector forward = GetActorForwardVector();
		AddMovementInput(forward, input);
	}
}

void ABird::Turn(const FInputActionValue& Value)
{
	const FVector2D input = Value.Get<FVector2D>();
	if (Controller != nullptr) {
		AddControllerYawInput(input.X);
		AddControllerPitchInput(input.Y);
	}
}


// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ABird::Turn);
	}
}

