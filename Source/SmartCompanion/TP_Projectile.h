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

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	ATP_Projectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	const float LIFETIME_IN_SECONDS = 3.0f;
	const float SPEED = 3000.f;
	const float SPHERE_RADIUS = 5.0f;

private:
	void SetCollisionComp();
	void SetProjectileMovement();
	void AddProjectileImpulse(UPrimitiveComponent* OtherComp);
};
