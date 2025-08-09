// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "SteamMultiplayerGameInstance.generated.h"

UCLASS(Abstract)
class MULTIPLAYERTEST_API USteamMultiplayerGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Game Info")
	void SetExpectedPlayerCount(const int NumPlayers) { ExpectedPlayerCount = NumPlayers; };

	UFUNCTION(BlueprintCallable, Category = "Game Info")
	int GetExpectedPlayerCount() const { return ExpectedPlayerCount; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ExpectedPlayerCount = 1;
};
