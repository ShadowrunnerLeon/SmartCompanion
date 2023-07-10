// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SmartModules/SpeechRecognitionModule/SpeechRecognitionModule.h"
#include "Commands/CommandHandler/CommandHandler.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SmartCompanionCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class ASmartCompanionCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, Category = "Flags")
	bool BattleStateFlag = false;

	UPROPERTY(VisibleAnywhere, Category = "Flags")
	bool StealthStateFlag = false;

	bool SmartStatusFlag = false;

	public:
		ASmartCompanionCharacter();
		~ASmartCompanionCharacter();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
		float TurnRateGamepad;

		UPROPERTY(BlueprintAssignable, Category = "Interaction")
		FOnUseItem OnUseItem;

		UFUNCTION(BlueprintCallable)
		bool GetBattleStateFlag();

		UFUNCTION(BlueprintCallable)
		void SetBattleStateFlag(bool val);

		UFUNCTION(BlueprintCallable)
		bool GetStealthStateFlag();

		UFUNCTION(BlueprintCallable)
		void StealthActivate();

		UFUNCTION(BlueprintCallable)
		void StealthDeactivate();

		void SmartActivate();
		void SmartDeactivate();

		void ActivateFirstPersonView();
		void DeactivateFirstPersonView();

		/** Fires a projectile. */
		void OnPrimaryAction();

		void RotateOnAngleYaw(float angle);

	protected:
		void MoveForward(float Value);
		void MoveRight(float Value);
		void TurnAtRate(float Rate);
		void LookUpAtRate(float Rate);

		void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
		void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		void Tick(float DeltaTime);

	public:
		FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
		FORCEINLINE class UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }

	private:
		void SetUseControllerSettings();
		void SetCharacterMovement();
		void SetCameraBoom();
		void SetFollowCamera();
		void SetFirstPersonCamera();

		void BindAction(class UInputComponent* PlayerInputComponent);
		void BindAxis(class UInputComponent* PlayerInputComponent);
		void BindTouch(class UInputComponent* PlayerInputComponent);

		const FRotator GetYawRotationForMoving();
		void MoveInCalculatedDirection(FRotator rotation, EAxis::Type axis, float value);
};

