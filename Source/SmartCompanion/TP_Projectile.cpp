#include "TP_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ATP_Projectile::ATP_Projectile()
{
	SetCollisionComp();
	RootComponent = CollisionComp;
	SetProjectileMovement();
	InitialLifeSpan = 3.0f;
}

void ATP_Projectile::SetCollisionComp()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATP_Projectile::OnHit);
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
}

void ATP_Projectile::SetProjectileMovement()
{
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void ATP_Projectile::AddProjectileImpulse(UPrimitiveComponent* OtherComp)
{
	OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
}

void ATP_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || (OtherActor == this) || !OtherComp || !(OtherComp->IsSimulatingPhysics())) return;
	
	AddProjectileImpulse(OtherComp);
	Destroy();
}

void ATP_Projectile::BeginPlay()
{
	Super::BeginPlay();
}

void ATP_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}