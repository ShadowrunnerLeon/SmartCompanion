#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_WeaponComponent.generated.h"

class ASmartCompanionCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SMARTCOMPANION_API UTP_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

	public:	
		UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ATP_Projectile> ProjectileClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

		UTP_WeaponComponent();

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AttachWeapon(ASmartCompanionCharacter* TargetCharacter);

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Fire();

	protected:
		virtual void BeginPlay() override;
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	public:	
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	private:
		ASmartCompanionCharacter* Character;

		FRotator SpawnRotation;
		FVector SpawnLocation;
		FActorSpawnParameters ActorSpawnParams;

	private:
		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void GenerateProjectile();

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetSpawnRotation();

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetSpawnLocation();

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetActorSpawnParametrs();

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Spawn(UWorld* world);

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void PlayFireSound();

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void PlayFireAnimation();

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void RegisterUseItem();

		UFUNCTION(BlueprintCallable, Category = "Weapon")
		void UnregisterUseItem();
};
