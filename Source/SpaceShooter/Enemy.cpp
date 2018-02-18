// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(RootBox);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.X += Speed*DeltaTime;
	SetActorLocation(NewLocation);

	if (NewLocation.X < -600.f)
		this->Destroy();
}


