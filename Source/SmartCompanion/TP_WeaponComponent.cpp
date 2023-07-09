#include "TP_WeaponComponent.h"
#include "SmartCompanionCharacter.h"
#include "TP_Projectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

UTP_WeaponComponent::UTP_WeaponComponent()
{
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::Fire()
{
	if (!Character || !Character->GetController()) return;

	GenerateProjectile();
	PlayFireSound();
	PlayFireAnimation();
}

void UTP_WeaponComponent::GenerateProjectile()
{
	if (!ProjectileClass) return;

	UWorld* const world = GetWorld();
	if (!world) return;

	SetSpawnRotation();
	SetSpawnLocation();
	SetActorSpawnParametrs();
	Spawn(world);
}

void UTP_WeaponComponent::SetSpawnRotation()
{
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
}

void UTP_WeaponComponent::SetSpawnLocation()
{
	SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
}

void UTP_WeaponComponent::SetActorSpawnParametrs()
{
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
}

void UTP_WeaponComponent::Spawn(UWorld* world)
{
	world->SpawnActor<ATP_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
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

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!Character) return;
	UnregisterUseItem();
}

void UTP_WeaponComponent::AttachWeapon(ASmartCompanionCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (!Character) return;
		
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	GetOwner()->AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));

	Character->SetBattleStateFlag(true);
	RegisterUseItem();
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTP_WeaponComponent::RegisterUseItem()
{
	Character->OnUseItem.AddDynamic(this, &UTP_WeaponComponent::Fire);
}

void UTP_WeaponComponent::UnregisterUseItem()
{
	Character->OnUseItem.RemoveDynamic(this, &UTP_WeaponComponent::Fire);
}