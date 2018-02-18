// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletController.h"
#include "Enemy.h"
#include "SpaceShooterGameMode.h"

#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
ABulletController::ABulletController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(RootBox);
	RootBox->bGenerateOverlapEvents = true;
}

// Called when the game starts or when spawned
void ABulletController::BeginPlay()
{
	Super::BeginPlay();
	// Crear estos vinculos en BeginPlay en lugar del constructor para estabilidad
	RootBox->OnComponentBeginOverlap.AddDynamic(this, &ABulletController::OnOverlap);
	
}

// Called every frame
void ABulletController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.X += Speed*DeltaTime;
	SetActorLocation(NewLocation);

	if (NewLocation.X > 600.f)
		this->Destroy();

}

void ABulletController::OnOverlap(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AEnemy::StaticClass())) {
		OtherActor->Destroy();
		this->Destroy();

		((ASpaceShooterGameMode*)GetWorld()->GetAuthGameMode())->IncrementScore();
	}
}

