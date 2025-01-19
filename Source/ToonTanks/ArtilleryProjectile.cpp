// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtilleryProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


void AArtilleryProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.RemoveAll(this);
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AArtilleryProjectile::OnHit);
}

void AArtilleryProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* ProjectileOnwer = GetOwner();
	if (!ProjectileOnwer)
	{
		Destroy();
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor != ProjectileOnwer)
	{
		auto DamageTypeClass = UDamageType::StaticClass();
		TArray<AActor*> IgnoredActors;
		UGameplayStatics::ApplyRadialDamage(OtherActor, Damage, HitComponent->GetComponentLocation(), DamageRadius, DamageTypeClass, IgnoredActors);
	}

	HandleHit();

	Destroy();
}
