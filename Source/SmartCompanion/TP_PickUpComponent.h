// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "SmartCompanionCharacter.h"
#include "TP_PickUpComponent.generated.h"

// Declaration of the delegate that will be called when someone picks this up
// The character picking this up is the parameter sent with the notification
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, ASmartCompanionCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SMARTCOMPANION_API UTP_PickUpComponent : public USphereComponent
{
	GENERATED_BODY()

	public:
		UPROPERTY(BlueprintAssignable, Category = "Interaction")
		FOnPickUp OnPickUp;

		UTP_PickUpComponent();

	protected:
		virtual void BeginPlay() override;

		UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	private:
		const float SPHERE_RADIUS = 32.f;

	private:
		void RegisterEvent();
		void UnregisterEvent();
		void Notify(ASmartCompanionCharacter* character);
};
