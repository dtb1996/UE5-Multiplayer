// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "SteamMultiplayerGameState.h"
#include "Net/UnrealNetwork.h"

ASteamMultiplayerGameState::ASteamMultiplayerGameState()
{
	bReplicates = true;
}

void ASteamMultiplayerGameState::BeginPlay()
{
	Super::BeginPlay();


}

void ASteamMultiplayerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASteamMultiplayerGameState, ActiveViewTarget);
}
