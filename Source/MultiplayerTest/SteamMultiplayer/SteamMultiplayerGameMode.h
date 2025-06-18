// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SteamMultiplayerGameMode.generated.h"

USTRUCT(BlueprintType)
struct FPlayerTurn
{
	GENERATED_BODY()

	UPROPERTY()
	APlayerState* Player = nullptr;

	UPROPERTY()
	bool bCompleted = false;

	UPROPERTY()
	bool bWantsRetry = false;
};

UCLASS(Abstract)
class MULTIPLAYERTEST_API ASteamMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASteamMultiplayerGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void NotifyPlayerReady();
	
	void CheckAllPlayersReady();

	void HandlePlayerTurnFinished(APlayerController* PC, const bool bSuccess, const bool bWantsRetry);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartNextStage();

private:
	void StartFirstTurn();

	void BeginPlayerTurn(int32 PlayerIndex);

	void AdvanceTurn();

	void RetryPlayer(APlayerController* PC);

	void AllTurnsComplete();

	void EnablePlayer(AController* PC);

	void DisablePlayer(AController* PC);

	void SetAllViewsToActivePlayer(APlayerController* ActivePlayer);

	int32 ExpectedPlayerCount = 1;

	int32 NumPlayersNotifiedReady = 0;

	bool bPlayersLoggedIn = false;

	TArray<FPlayerTurn> TurnOrder;

	int32 CurrentPlayerIndex = 0;
};
