// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SteamMultiplayerGameMode.generated.h"

USTRUCT(BlueprintType)
struct FPlayerTurn
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	APlayerState* Player = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool bCompleted = false;

	UPROPERTY(BlueprintReadWrite)
	bool bWantsRetry = false;
};

UCLASS(Abstract)
class MULTIPLAYERTEST_API ASteamMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASteamMultiplayerGameMode();

	UFUNCTION(BlueprintCallable)
	void NotifyPlayerReady();

	UFUNCTION(BlueprintCallable)
	void HandleStageReady();

	void HandlePlayerTurnFinished(APlayerController* PC, const bool bSuccess, const bool bWantsRetry);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartNextStage();

private:
	bool CheckAllPlayersReady();

	void TryStartFirstTurn();

	void StartFirstTurn();

	void BeginPlayerTurn(int32 PlayerIndex);

	void AdvanceTurn();

	void RetryPlayer(APlayerController* PC);

	void AllTurnsComplete();

	void EnablePlayer(AController* PC);

	void DisablePlayer(AController* PC);

	void SetAllViewsToActivePlayer(APlayerController* ActivePlayer);

	int32 NumPlayersNotifiedReady = 0;

	TArray<FPlayerTurn> TurnOrder;

	int32 CurrentPlayerIndex = 0;

	bool bArePlayersReady = false;

	bool bIsHoleReady = false;

	bool bHasStartedFirstTurn = false;

	bool bIsAdvancingTurn = false;

	bool bIsRetryingCurrentPlayer = false;
};
