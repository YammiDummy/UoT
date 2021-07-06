// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "GameStructs.h"
#include "Projectile.h"
#include "ScoreComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

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
	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankPawn::DamageTaken);

	ScoreComponent = CreateDefaultSubobject<UScoreComponent>(TEXT("Score component"));

	DestroyVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Visual effect"));
	DestroyVisualEffect->SetupAttachment(BodyMesh);
	DestroyVisualEffect->bAutoActivate = false;

	DestroyAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio effect"));
	DestroyAudioEffect->SetupAttachment(BodyMesh);
	DestroyAudioEffect->bAutoActivate = false;
}


// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

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

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TurretLookAtPoint);
	FRotator CurTurretRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurTurretRotation.Pitch;
	TargetRotation.Roll = CurTurretRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurTurretRotation, TargetRotation, TurretRotationSensitivity));
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

void ATankPawn::SetTurretLookAtPoint(FVector InPoint)
{
	TurretLookAtPoint = InPoint;
}

void ATankPawn::SwapCannon()
{
	ACannon* Temp = Cannon;
	Cannon = SecCannon;
	SecCannon = Temp;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("SWAP!!!"), true);
	
}

//void ATankPawn::ReloadAmmo()
//{
//	if (Cannon)
//	{
//		Cannon->ReloadAmmo();
//	}
//}

void ATankPawn::SetupCannon()
{
	if (Cannon&&!SecCannon)
	{
		SwapCannon();
	}

	if (Cannon) {
		Cannon->Destroy();
		Cannon = nullptr;

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("CANNON DESTROYED!!!"), true);
	}

	if (CannonClass)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("SETUP!!!"), true);
	}
}

void ATankPawn::Die()
{
	FVector DestroyLocation = GetActorLocation();
	DestroyVisualEffect->ActivateSystem();
	DestroyAudioEffect->Play();
	PrimaryActorTick.bCanEverTick = false;
	BodyMesh->DestroyComponent();
	TurretMesh->DestroyComponent();
	CannonSetupPoint->DestroyComponent();
	Cannon->Destroy();
}

void ATankPawn::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

bool ATankPawn::TakeDamage(FDamageData DamageData)
{
	return HealthComponent->TakeDamage(DamageData);
}

TArray<FVector> ATankPawn::GetPatrollingPoints()
{
	return PatrollingPoints;
}

FVector ATankPawn::GetEyesPosition() const
{
	return CannonSetupPoint->GetComponentLocation();
}

float ATankPawn::GetMovementAccurency()
{
	return MovementAccurency;
}

float ATankPawn::GetScore()
{
	return ScoreComponent->GetScore();
}

void ATankPawn::SetScore(float Score)
{
	ScoreComponent->SetScore(Score);
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}