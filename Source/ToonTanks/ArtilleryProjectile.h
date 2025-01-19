// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ArtilleryProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AArtilleryProjectile : public AProjectile
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	UPROPERTY(EditAnywhere)
	float DamageRadius = 300.f;
};
