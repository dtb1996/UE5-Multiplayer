// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SteamMultiplayerPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerStates : uint8
{
	E_None          UMETA(DisplayName = "None"),
	E_Countdown     UMETA(DisplayName = "Countdown"),
	E_InProgress    UMETA(DisplayName = "InProgress"),
	E_GoalReach     UMETA(DisplayName = "GoalReached"),
	E_Results       UMETA(DisplayName = "DisplayingResults"),
	E_EndTurn       UMETA(DisplayName = "EndingTurn")
};

UCLASS(Abstract)
class MULTIPLAYERTEST_API ASteamMultiplayerPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASteamMultiplayerPlayerState();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	bool bIsReady = false;

	void SetIsReady(bool bReady);

	bool GetIsReady() const { return bIsReady; };

	UPROPERTY(Replicated)
	EPlayerStates CurrentState = EPlayerStates::E_None;

	void SwitchState(const EPlayerStates NewState = EPlayerStates::E_None);

	void StartTurn();

	void EndTurn();

private:
	float CountdownTime = 2.0f;

	FTimerHandle CountdownTimerHandle;

	UFUNCTION()
	void OnCountdownFinished();
};
