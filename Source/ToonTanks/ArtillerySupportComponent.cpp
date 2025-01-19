// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtillerySupportComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ArtilleryPawn.h"
#include "Tank.h"

// Sets default values for this component's properties
UArtillerySupportComponent::UArtillerySupportComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UArtillerySupportComponent::BeginPlay()
{
	Super::BeginPlay();

	ArtilleryPawn = Cast<AArtilleryPawn>(UGameplayStatics::GetActorOfClass(this, AArtilleryPawn::StaticClass()));
	Tank = Cast<ATank>(GetOwner());

	if (!Tank) return;
	TankPlayerController = Cast<APlayerController>(Tank->GetController());

	if (!TankPlayerController) return;
	TankPlayerController->InputComponent->BindAction(TEXT("Artillery"), IE_Released, this, &UArtillerySupportComponent::ToggleSearchLocationMode);
}

// Called every frame
void UArtillerySupportComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TankPlayerController && SearchLocationMode)
	{
		FHitResult HitResult;
		bool Hit = TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

		if (Hit)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, ArtilleryPawn->GetRadius(), 40, FColor::Red);
		}
	}
}

void UArtillerySupportComponent::ToggleSearchLocationMode()
{
	if (ArtilleryPawn->CheckIsBusy() || !Tank || !Tank->HasStrikes()) return;

	//UE_LOG(LogTemp, Warning, TEXT("ToggleSearchLocationMode"));
	SearchLocationMode = !SearchLocationMode;

	TankPlayerController->InputComponent->RemoveActionBinding(TEXT("Fire"), EInputEvent::IE_Pressed);
	if (SearchLocationMode)
	{
		TankPlayerController->InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UArtillerySupportComponent::ActivateSelectionMode);
	}
	else
	{
		TankPlayerController->InputComponent->BindAction(TEXT("Fire"), IE_Pressed, Tank, &ATank::Fire);
	}
}

void UArtillerySupportComponent::ActivateSelectionMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("ActivateSelectionMode"));
	ToggleSearchLocationMode();
	if (!TankPlayerController || !Tank) return;

	FHitResult HitResult;
	bool Hit = TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

	if (Hit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, ArtilleryPawn->GetRadius(), 40, FColor::Red, false, ArtilleryPawn->GetFireRateTime());
		ArtilleryPawn->SetFireTarget(HitResult.ImpactPoint);
	}

	Tank->DecreaseStrikeAmount();
}
