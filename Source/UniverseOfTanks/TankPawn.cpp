// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TuretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}


// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	TankController = Cast<ATankPlayerController>(GetController());

	SetupCannon();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();																	   //Movement
	FVector NewLocation = CurrentLocation + GetActorForwardVector() * MoveSpeed * CurrentMoveForwardAxis * DeltaTime   //Forward
										  + GetActorRightVector() * MoveSpeed * CurrentMoveRightAxis * DeltaTime;	   //Right
	
	SetActorLocation(NewLocation);

	CurrentYawAxis = FMath::Lerp(CurrentYawAxis, TargetYawAxis, YawSensitivity);									   //Rotation
	FRotator CurrentRotation = GetActorRotation();
	float YawValue = CurrentRotation.Yaw + YawSpeed * CurrentYawAxis * DeltaTime;
	SetActorRotation(FRotator(0, YawValue, 0));

	if (TankController)																								   //TurretMovement
	{
		FVector TurretLookAtPoint = TankController->GetMousePos();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TurretLookAtPoint);
		FRotator CurTurretRotation = TurretMesh->GetComponentRotation();
		TargetRotation.Pitch = CurTurretRotation.Pitch;
		TargetRotation.Roll = CurTurretRotation.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(CurTurretRotation, TargetRotation, TurretRotationSensitivity));
	}
}

void ATankPawn::MoveForward(float InAxisValue)
{
	CurrentMoveForwardAxis = InAxisValue;
}

void ATankPawn::MoveRight(float InAxisValue)
{
	CurrentMoveRightAxis = InAxisValue;
}

void ATankPawn::Yaw(float InAxisValue)
{
	TargetYawAxis = InAxisValue;
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::ReloadAmmo()
{
	if (Cannon)
	{
		Cannon->ReloadAmmo();
	}
}

void ATankPawn::SetupCannon()
{
	if (Cannon) {
		Cannon->Destroy();
		Cannon = nullptr;
	}

	if (CannonClass)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

