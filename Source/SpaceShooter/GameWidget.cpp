// Fill out your copyright notice in the Description page of Project Settings.

#include "GameWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

void UGameWidget::Load() 
{
	const FName TextBlockName = FName(TEXT("GameTextBlock"));

	if (ScoreText == nullptr) {
		ScoreText = (UTextBlock*)(WidgetTree->FindWidget(TextBlockName));
	}
}

void UGameWidget::SetScore(int Score)
{
	if (ScoreText != nullptr) {
		ScoreText->SetText(FText::FromString(FString(TEXT("Score: ")) + FString::FromInt(Score)) );
	}
}

void UGameWidget::OnGameOver(int Score)
{
	if (ScoreText != nullptr) {
		ScoreText->SetText(FText::FromString(FString(TEXT("Score: ")) + FString::FromInt(Score) + TEXT("\nPress R to restart")));
	}
}
