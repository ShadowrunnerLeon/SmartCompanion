// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_PickUpComponent.h"

UTP_PickUpComponent::UTP_PickUpComponent()
{
	SphereRadius = SPHERE_RADIUS;
}

void UTP_PickUpComponent::RegisterEvent()
{
	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}

void UTP_PickUpComponent::UnregisterEvent()
{
	OnComponentBeginOverlap.RemoveAll(this);
}

void UTP_PickUpComponent::Notify(ASmartCompanionCharacter* character)
{
	OnPickUp.Broadcast(character);
}

void UTP_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterEvent();
}

void UTP_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ASmartCompanionCharacter* character = Cast<ASmartCompanionCharacter>(OtherActor);
	if (!character) return;
	Notify(character);
	UnregisterEvent();
}