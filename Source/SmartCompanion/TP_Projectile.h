#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SMARTCOMPANION_API ATP_Projectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
	public:	
		ATP_Projectile();

		UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

		USphereComponent* GetCollisionComp() const { return CollisionComp; }
		UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	protected:
		virtual void BeginPlay() override;

	public:	
		virtual void Tick(float DeltaTime) override;

	private:
		void SetCollisionComp();
		void SetProjectileMovement();
		void AddProjectileImpulse(UPrimitiveComponent* OtherComp);
};
