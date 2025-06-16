// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
//#include "GameStates.h"
//#include "TurnState.h"
//#include "PlayerCourseStats.h"
//#include "GolfPlayerState.h"
#include "SteamMultiplayerGameState.generated.h"

UENUM(BlueprintType)
enum class EGameStates : uint8
{
	E_None          UMETA(DisplayName = "None"),
	E_Waiting       UMETA(DisplayName = "Waiting"),
	E_InProgress    UMETA(DisplayName = "InProgress"),
	E_CourseResults UMETA(DisplayName = "CourseResults"),
	E_LevelChange   UMETA(DisplayName = "LevelChange")
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentStateChanged, UTurnState*, NewState, EGameStates, NewStateType);

UCLASS(Abstract)
class MULTIPLAYERTEST_API ASteamMultiplayerGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
