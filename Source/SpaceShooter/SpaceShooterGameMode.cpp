// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShooterGameMode.h"
#include "Enemy.h"
#include "GameWidget.h"

#include "Runtime/Engine/Classes/Engine/World.h"

void ASpaceShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	ChangeMenuWidget(StartingWidgetClass);

	((UGameWidget*)CurrentWidget)->Load();

}

void ASpaceShooterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTimer += DeltaTime;
	EnemyTimer -= DeltaTime;

	if (EnemyTimer <= 0.f) {
		float DifficultyPercentage = FMath::Min(GameTimer / TIME_TO_MINIMUM_INTERVAL, 1.0f);
		EnemyTimer = MAXIMUM_INTERVAL - (MAXIMUM_INTERVAL - MINIMUM_INTERVAL)*DifficultyPercentage;

		UWorld* World = GetWorld();
		if (ensure(World)) {
			FVector Location = FVector(600.f, FMath::RandRange(-800.f, 800.f), 70.f);
			World->SpawnActor<AEnemy>(EnemyBlueprint, FTransform(Location));
		}

	}
}

void ASpaceShooterGameMode::IncrementScore()
{
	Score += 100;
	((UGameWidget*)CurrentWidget)->SetScore(Score);
}

void ASpaceShooterGameMode::OnGameOver()
{
	((UGameWidget*)CurrentWidget)->OnGameOver(Score);
}

void ASpaceShooterGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget) {
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget) {
			CurrentWidget->AddToViewport();
		}
	}
}
