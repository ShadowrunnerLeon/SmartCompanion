#include "SmartCompanionCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "TP_Projectile.h"
#include "GameFramework/InputSettings.h"
#include "Animation/AnimInstance.h"
#include "SmartCompanionGameMode.h"

ASmartCompanionCharacter::ASmartCompanionCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	TurnRateGamepad = 50.f;

	SetFlags();
	SetCharacterMovement();
	SetCameraBoom();
	SetFollowCamera();
	SetFirstPersonCamera();
}

ASmartCompanionCharacter::~ASmartCompanionCharacter()
{
}

void ASmartCompanionCharacter::SetFlags()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

void ASmartCompanionCharacter::SetCharacterMovement()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void ASmartCompanionCharacter::SetCameraBoom()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
}

void ASmartCompanionCharacter::SetFollowCamera()
{
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;
}

void ASmartCompanionCharacter::SetFirstPersonCamera()
{
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh(), "HeadSocket");
	FirstPersonCamera->bUsePawnControlRotation = true;
}

void ASmartCompanionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	BindAction(PlayerInputComponent);
	BindAxis(PlayerInputComponent);
	BindTouch(PlayerInputComponent);
}

void ASmartCompanionCharacter::BindAction(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Stealth", IE_Pressed, this, &ASmartCompanionCharacter::StealthActivate);
	PlayerInputComponent->BindAction("Stealth", IE_Released, this, &ASmartCompanionCharacter::StealthDeactivate);
	PlayerInputComponent->BindAction("Smart", IE_Pressed, this, &ASmartCompanionCharacter::SmartActivate);
	PlayerInputComponent->BindAction("Smart", IE_Released, this, &ASmartCompanionCharacter::SmartDeactivate);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ASmartCompanionCharacter::OnPrimaryAction);
	PlayerInputComponent->BindAction("FirstPersonView", IE_Pressed, this, &ASmartCompanionCharacter::ActivateFirstPersonView);
	PlayerInputComponent->BindAction("FirstPersonView", IE_Released, this, &ASmartCompanionCharacter::DeactivateFirstPersonView);
}

void ASmartCompanionCharacter::BindAxis(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ASmartCompanionCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ASmartCompanionCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ASmartCompanionCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ASmartCompanionCharacter::LookUpAtRate);
}

void ASmartCompanionCharacter::BindTouch(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASmartCompanionCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASmartCompanionCharacter::TouchStopped);
}

void ASmartCompanionCharacter::OnPrimaryAction()
{
	OnUseItem.Broadcast();
}

void ASmartCompanionCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ASmartCompanionCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ASmartCompanionCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASmartCompanionCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASmartCompanionCharacter::MoveForward(float Value)
{
	if (!Controller || !Value) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ASmartCompanionCharacter::MoveRight(float Value)
{
	if (!Controller || !Value) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ASmartCompanionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!((GetWorld()->GetAuthGameMode<ASmartCompanionGameMode>())->GetTaskQueue().empty()))
	{
		((GetWorld()->GetAuthGameMode<ASmartCompanionGameMode>())->GetTaskQueue().front())();
		(GetWorld()->GetAuthGameMode<ASmartCompanionGameMode>())->GetTaskQueue().pop();
	}
}

void ASmartCompanionCharacter::ActivateFirstPersonView()
{
	FirstPersonCamera->SetActive(true);
	FollowCamera->SetActive(false);
}

void ASmartCompanionCharacter::DeactivateFirstPersonView()
{
	FirstPersonCamera->SetActive(false);
	FollowCamera->SetActive(true);
}

bool ASmartCompanionCharacter::GetBattleStateFlag()
{
	return BattleStateFlag;
}

void ASmartCompanionCharacter::SetBattleStateFlag(bool val)
{
	BattleStateFlag = val;
}

bool ASmartCompanionCharacter::GetStealthStateFlag()
{
	return StealthStateFlag;
}

void ASmartCompanionCharacter::StealthActivate()
{
	StealthStateFlag = true;
}

void ASmartCompanionCharacter::StealthDeactivate()
{
	StealthStateFlag = false;
}

void ASmartCompanionCharacter::SmartActivate()
{
	(GetWorld()->GetAuthGameMode<ASmartCompanionGameMode>())->ActivateCommandHandler();
}

void ASmartCompanionCharacter::SmartDeactivate()
{
	(GetWorld()->GetAuthGameMode<ASmartCompanionGameMode>())->DeactivateCommandHandler();
}

void ASmartCompanionCharacter::RotateOnAngleYaw(float angle)
{
	auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->SetControlRotation(FRotator(0, angle, 0));
}
