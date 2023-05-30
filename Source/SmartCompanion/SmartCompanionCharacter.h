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

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** First Person camera */
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

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
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

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void Tick(float DeltaTime);

private:
	//SpeechRecognitionModule speechRecognitionModule;
	TSharedPtr<CommandHandler> commandHandler = MakeShared<CommandHandler>(GetWorld());

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns FirstPersonCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }
};

