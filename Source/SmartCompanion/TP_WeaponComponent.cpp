#include "TP_WeaponComponent.h"
#include "SmartCompanionCharacter.h"
#include "TP_Projectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;

	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::GenerateProjectile()
{
	// Try and fire a projectile
	if (!ProjectileClass) return;

	UWorld* const World = GetWorld();
	if (!World) return;

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// Spawn the projectile at the muzzle
	World->SpawnActor<ATP_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
}

void UTP_WeaponComponent::PlayFireSound()
{
	if (!FireSound) return;
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
}

void UTP_WeaponComponent::PlayFireAnimation()
{
	if (!FireAnimation) return;
		
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	AnimInstance->Montage_Play(FireAnimation, 1.f);
}

void UTP_WeaponComponent::Fire()
{
	if (!Character || !Character->GetController()) return;

	GenerateProjectile();
	PlayFireSound();
	PlayFireAnimation();
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!Character) return;

	// Unregister from the OnUseItem Event
	Character->OnUseItem.RemoveDynamic(this, &UTP_WeaponComponent::Fire);
}

void UTP_WeaponComponent::AttachWeapon(ASmartCompanionCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (!Character) return;
		
	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	GetOwner()->AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));

	Character->SetBattleStateFlag(true);

	// Register so that Fire is called every time the character tries to use the item being held
	Character->OnUseItem.AddDynamic(this, &UTP_WeaponComponent::Fire);
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}