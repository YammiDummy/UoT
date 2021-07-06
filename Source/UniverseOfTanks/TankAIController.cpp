// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	AiTank = Cast<ATankPawn>(GetPawn());

	if (AiTank && AiTank->PatrollingPoints.Num() >= 0)
	{
		CurrentPatrolPointIndex = 0;
		FVector TankLocation = AiTank->GetActorLocation();
		for (const auto& Point : AiTank->PatrollingPoints)
		{
			CachedPatrollingPoints.Add(TankLocation + Point);
		}
	}
}

void ATankAIController::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (!AiTank || CurrentPatrolPointIndex == INDEX_NONE)
	{
		return;
	}

	if (AiTank->PrimaryActorTick.bCanEverTick == false)
	{
		PrimaryActorTick.bCanEverTick = false;
		return;
	}

	AiTank->MoveForward(1);

	FVector CurrentPoint = CachedPatrollingPoints[CurrentPatrolPointIndex];
	FVector PawnLocation = AiTank->GetActorLocation();
	if (FVector::Distance(CurrentPoint, PawnLocation) <= AiTank-> MovementAccurency)
	{
		if (++CurrentPatrolPointIndex >= CachedPatrollingPoints.Num())
			CurrentPatrolPointIndex = 0;
	}

	FVector MoveDirection = CurrentPoint - PawnLocation;
	MoveDirection.Normalize();
	FVector ForwardDirection = AiTank->GetActorForwardVector();
	FVector RightDirection = AiTank->GetActorRightVector();

	DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

	float ForwardAngleCos = FVector::DotProduct(ForwardDirection, MoveDirection);
	float RightAngleCos = FVector::DotProduct(RightDirection, MoveDirection);
	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(ForwardAngleCos));
	float RightAngle = FMath::RadiansToDegrees(FMath::Acos(RightAngleCos));

	float RotationValue = 0;
	if (ForwardAngle > 5)
		RotationValue = 1;
	if (RightAngle > 90)
		RotationValue = -RotationValue;

	UE_LOG(LogTemp, Warning, TEXT("AI Rotation forwardAngle: %f rightAngle: %f rotationValue: %f"), ForwardAngle, RightAngle, RotationValue);
	AiTank->Yaw(RotationValue);

	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerTank)
	{
		FVector PlayerPawnLocation = PlayerTank->GetActorLocation();
		bool bCanSeePlayer = false;

		if (FVector::Distance(PlayerPawnLocation, PawnLocation) < TargetingRange)
		{
			FCollisionQueryParams Params(FName(TEXT("EnemyEyes")), true, this);
			Params.AddIgnoredActor(AiTank);
			Params.bReturnPhysicalMaterial = false;
			FHitResult HitResult;

			if (GetWorld()->LineTraceSingleByChannel(HitResult, AiTank->GetEyesPosition(), PlayerPawnLocation, ECC_Visibility, Params))
			{
				if (HitResult.Actor.Get()) {
					DrawDebugLine(GetWorld(), AiTank->GetEyesPosition(), HitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
					bCanSeePlayer = HitResult.Actor.Get() == PlayerTank;
				}
			}

		}

		if (bCanSeePlayer)
		{

			AiTank->MoveForward(0);

			DrawDebugLine(GetWorld(), PawnLocation, PlayerPawnLocation, FColor::Green, false, 0.1f, 0, 5);
			AiTank->SetTurretLookAtPoint(PlayerPawnLocation);

			FVector DirectionToPlayer = PlayerPawnLocation - PawnLocation;
			DirectionToPlayer.Normalize();
			FVector TurretDirection = AiTank->TurretMesh->GetForwardVector();
			float AngleToPlayer = FMath::Acos(FVector::DotProduct(TurretDirection, DirectionToPlayer));

			if (FMath::RadiansToDegrees(AngleToPlayer) < Accurency)
			{
				AiTank->Fire();
			}
		}
		else
		{
			AiTank->SetTurretLookAtPoint(AiTank->BodyMesh->GetComponentLocation());

		}
	}
}