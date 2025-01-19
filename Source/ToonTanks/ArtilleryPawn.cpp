// Fill out your copyright notice in the Description page of Project Settings.

#include "ArtilleryProjectile.h"
#include "ArtilleryPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AArtilleryPawn::AArtilleryPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base mesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// Called every frame
void AArtilleryPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FireTarget != FVector::ZeroVector && !IsBusy)
	{
		Points = GetRandomLocationPoints(FireTarget);
		RotateToTarget(FireTarget);
		StartShooting();
	}
}

void AArtilleryPawn::RotateToTarget(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation(LookAtRotation);
}

void AArtilleryPawn::UpdateTurretAngle(FVector LookAtTarget)
{
	FRotator Rotation = TurretMesh->GetComponentRotation();
	FRotator LookAtRotation = FRotator(LookAtTarget.Rotation().Pitch, Rotation.Yaw, Rotation.Roll);
	TurretMesh->SetWorldRotation(LookAtRotation);
}

void AArtilleryPawn::StartShooting()
{
	FVector Point = Points.Pop();
	LaunchProjectile(Point);
	//DrawDebugSphere(GetWorld(), Point, 25, 10, FColor::Blue, false, 15.f);

	if (Points.Num() > 0)
	{
		GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &AArtilleryPawn::StartShooting, FireRate, false);
	}
	else
	{
		SetFireTarget(FVector::ZeroVector);
	}

	IsBusy = Points.Num() > 0;
}

TArray<FVector> AArtilleryPawn::GetRandomLocationPoints(FVector CenterPoint)
{
	TArray<FVector> Result;

	for (int i = 0; i < PointsNum; i++) {
		FVector2D RandPoint = FMath::RandPointInCircle(Radius);
		FVector Location(RandPoint.X + CenterPoint.X, RandPoint.Y + CenterPoint.Y, CenterPoint.Z);
		Result.Add(Location);
	}

	return Result;
}

void AArtilleryPawn::LaunchProjectile(FVector HitLocation)
{
	if (!ProjectileClass) return;

	FVector OutLaucnVelocity;
	FVector StartLocation = ProjectileSpawnPoint->GetComponentLocation();

	auto HaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaucnVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		true,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (HaveAimSolution) {
		SpawnProjectile(OutLaucnVelocity, StartLocation);
		UpdateTurretAngle(OutLaucnVelocity);
	}
}

void AArtilleryPawn::SpawnProjectile(FVector OutLaucnVelocity, FVector StartLocation)
{
	FVector AimDirection = OutLaucnVelocity.GetSafeNormal();

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		StartLocation,
		AimDirection.Rotation()
	);

	Projectile->SetOwner(this);
	Projectile->Launch(LaunchSpeed);
}

uint32 AArtilleryPawn::GetRadius()
{
	return Radius;
}

uint32 AArtilleryPawn::GetFireRateTime()
{
	return FireRate * PointsNum;
}

bool AArtilleryPawn::CheckIsBusy()
{
	return IsBusy;
}

void AArtilleryPawn::SetFireTarget(FVector NewFireTarget)
{
	FireTarget = NewFireTarget;
}
