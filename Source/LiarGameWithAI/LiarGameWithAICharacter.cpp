// Copyright Epic Games, Inc. All Rights Reserved.

#include "LiarGameWithAICharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerState.h"
#include "GamePlayerWidget/GamePlayerName.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ALiarGameWithAICharacter

ALiarGameWithAICharacter::ALiarGameWithAICharacter()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (tempMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// // Create a camera boom (pulls in towards the player if there is a collision)
	// CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// CameraBoom->SetupAttachment(RootComponent);
	// CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	// CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	//
	// // Create a follow camera
	// FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	// FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GamePlayerName = CreateDefaultSubobject<UWidgetComponent>(TEXT("GamePlayerName"));
	GamePlayerName->SetupAttachment(RootComponent);
	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALiarGameWithAICharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ALiarGameWithAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALiarGameWithAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//SetWidgetNameRot();
	// if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("ALobbyManager::Tick"));
	// }
}

void ALiarGameWithAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		// EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALiarGameWithAICharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALiarGameWithAICharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ALiarGameWithAICharacter::SetWidgetNameRot()
{
	// APlayerController* PlayerController = Cast<APlayerController>(GetController());
	// int32 PlayerIndex = UGameplayStatics::GetPlayerControllerID(PlayerController);
	// if (PlayerIndex < 0)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Invalid PlayerIndex: %d"), PlayerIndex);
	// 	return;
	// }
	// // PlayerIndex 번째 플레이어의 카메라 가져오기
	// APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	// if (!CamManager)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("CameraManager is nullptr for PlayerIndex: %d"), 0);
	// 	return;
	// }
	// APlayerController* Character =Cast<APlayerController>(this->GetController()); 
	// int32 Index = Character->NetPlayerIndex;
	
	// AActor* cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	// 카메라의 앞방향(반대), 윗방향을 이용해서 Rotator 를 구하자.

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		ACameraActor* cam = Cast<ACameraActor>(PC->GetViewTarget());
		FRotator rot = UKismetMathLibrary::MakeRotFromXZ(-cam->GetActorForwardVector(), cam->GetActorUpVector());
		// compHP 를 구한  Rotator 값으로 설정.
		GamePlayerName->SetWorldRotation(rot);
	}
	// UE_LOG(LogTemp,Warning,TEXT("PlayerIndex%d"),Index);
}

void ALiarGameWithAICharacter::SetUserId(FString userId, bool mine)
{
	UserId = userId;

	UGamePlayerName* NameUI = Cast<UGamePlayerName>(GamePlayerName->GetWidget());

	if (NameUI)
	{
		NameUI->PlayerName->SetText(FText::FromString(UserId));
		
		if (mine)
		{
			FSlateColor Col(FLinearColor::Yellow);
			NameUI->PlayerName->SetColorAndOpacity(Col);
		}
		else
		{
			FSlateColor Col(FLinearColor::White);
			NameUI->PlayerName->SetColorAndOpacity(Col);
		}
	}
}

void ALiarGameWithAICharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALiarGameWithAICharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALiarGameWithAICharacter::SetSitAnim()
{
	if (Anim)
	{
		Anim->bSitting = true;
	}
}
