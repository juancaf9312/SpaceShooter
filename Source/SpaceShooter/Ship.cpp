// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "BulletController.h"
#include "Enemy.h"
#include "SpaceShooterGameMode.h"

#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(CollisionBox);
	CollisionBox->bGenerateOverlapEvents = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AShip::OnOverlap);
	
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!CurrentVelocity.IsZero()) {

		FVector NewLocation = GetActorLocation() + Speed*CurrentVelocity*DeltaTime;

		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveX", this, &AShip::Move_XAxis);
	PlayerInputComponent->BindAxis("MoveY", this, &AShip::Move_YAxis);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AShip::OnShoot);
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &AShip::OnRestart).bExecuteWhenPaused = true;

}

void AShip::Move_XAxis(float AxisValue)
{
	CurrentVelocity.X = AxisValue * 100;
}

void AShip::Move_YAxis(float AxisValue)
{
	CurrentVelocity.Y = AxisValue * 100;
}

void AShip::OnShoot()
{
	UWorld* World = GetWorld();
	if (ensure(World)) {
		FVector Location = GetActorLocation();
		World->SpawnActor<ABulletController>(BulletBlueprint,Location,FRotator::ZeroRotator);

	}
}

void AShip::OnRestart()
{
	if (Died) {
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()),false);
		Died = false;
	}
}

void AShip::OnOverlap(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AEnemy::StaticClass())) {
		Died = true;

		this->SetActorHiddenInGame(true);

		((ASpaceShooterGameMode*)GetWorld()->GetAuthGameMode())->OnGameOver();

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

