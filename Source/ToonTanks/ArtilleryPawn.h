// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ArtilleryPawn.generated.h"

UCLASS()
class TOONTANKS_API AArtilleryPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AArtilleryPawn();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartShooting();
	void SetFireTarget(FVector NewFireTarget);
	bool CheckIsBusy();

	uint32 GetRadius();
	uint32 GetFireRateTime();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AArtilleryProjectile> ProjectileClass;

	TArray<FVector> GetRandomLocationPoints(FVector CenterPoint);
	void LaunchProjectile(FVector HitLocation);
	void SpawnProjectile(FVector OutLaucnVelocity, FVector StartLocatio);
	void RotateToTarget(FVector Location);
	void UpdateTurretAngle(FVector Location);

private:
	int32 Radius = 200;
	int32 PointsNum = 10;
	uint32 LaunchSpeed = 3000;

	FVector FireTarget = FVector::ZeroVector;
	TArray<FVector> Points;
	FTimerHandle FireRateTimerHandle;

	uint32 FireRate = 2;
	bool IsBusy = false;
};
