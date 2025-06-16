// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalGameInstance.h"
//#include "ReplaySubsystem.h"
#include "Engine/DemoNetDriver.h"
#include "TimerManager.h"

void ULocalGameInstance::StartRecording()
{
	StartRecordingReplay(FString("gameplay"), FString("gameplay"));
}

void ULocalGameInstance::StopRecording()
{
	StopRecordingReplay();
}

void ULocalGameInstance::PlayRecording()
{
	PlayReplay(FString("gameplay"));

	
	GetWorld()->GetTimerManager().SetTimer(
		CheckReplayReadyTimer,
		this,
		&ULocalGameInstance::SeekLastFiveSeconds,
		0.2f,
		true
	);
}

void ULocalGameInstance::SeekLastFiveSeconds()
{
	if (UDemoNetDriver* DemoDriver = GetWorld()->GetDemoNetDriver())
	{
		float TotalTime = DemoDriver->GetDemoTotalTime();
		if (TotalTime > 5.0f)
		{
			float SeekTime = TotalTime - 5.0f;
			DemoDriver->GotoTimeInSeconds(SeekTime);
			UE_LOG(LogTemp, Log, TEXT("Seeking to %.2f / %.2f seconds."), SeekTime, TotalTime);

			// Stop polling once we've done the seek
			GetWorld()->GetTimerManager().ClearTimer(CheckReplayReadyTimer);
		}
	}
}
