// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AdvancedFriendsGameInstance.h"
#include "LocalGameInstance.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MULTIPLAYERTEST_API ULocalGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StartRecording();

	UFUNCTION(BlueprintCallable)
	void StopRecording();

	UFUNCTION(BlueprintCallable)
	void PlayRecording();

private:
	void SeekLastFiveSeconds();

	FTimerHandle CheckReplayReadyTimer;
};
