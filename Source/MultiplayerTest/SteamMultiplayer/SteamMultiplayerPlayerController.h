// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "SteamMultiplayerPlayerController.generated.h"

UCLASS()
class MULTIPLAYERTEST_API ASteamMultiplayerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASteamMultiplayerPlayerController();

	virtual void BeginPlay() override;

	virtual void OnRep_Pawn() override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_NotifyFinishedTurn(bool bSuccess, bool bWantsRetry);
	void Server_NotifyFinishedTurn_Implementation(bool bSuccess, bool bWantsRetry);

	UFUNCTION(Client, Reliable)
	void ClientStartTurn();

	UFUNCTION(Client, Reliable)
	void ClientEndTurn();

	UFUNCTION(BlueprintNativeEvent)
	void SetTurnInputEnabled(const bool bNewIsEnabled);
};
