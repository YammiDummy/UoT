// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);

	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);

	InputComponent->BindAxis("Yaw", this, &ATankPlayerController::Yaw);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);

	//InputComponent->BindAction("Reload", IE_Pressed, this, &ATankPlayerController::ReloadAmmo);

	InputComponent->BindAction("SwapCannon", IE_Pressed, this, &ATankPlayerController::SwapCannon);

}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MousePosition, MouseDirection;
	DeprojectMousePositionToWorld(MousePosition, MouseDirection);

	if (TankPawn) {
		FVector TankPosition = TankPawn->GetActorLocation();
		CachedMousePos = MousePosition - TankPosition;
		CachedMousePos.Z = 0;
		CachedMousePos.Normalize();
		CachedMousePos = TankPosition + CachedMousePos * 500.f;
		
		//DrawDebugLine(GetWorld(), TankPosition, CachedMousePos, FColor::Green, false, 0.0f, 0, 5.f);
	}
}

FVector ATankPlayerController::GetMousePos() const
{
	return CachedMousePos;
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(AxisValue);
	}
}

void ATankPlayerController::MoveRight(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveRight(AxisValue);
	}
}

void ATankPlayerController::Yaw(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->Yaw(AxisValue);
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}

void ATankPlayerController::SwapCannon()
{
	if (TankPawn)
	{
		TankPawn->SwapCannon();
	}
}

//void ATankPlayerController::ReloadAmmo()
//{
//	if (TankPawn)
//	{
//		TankPawn->ReloadAmmo();
//	}
//}
