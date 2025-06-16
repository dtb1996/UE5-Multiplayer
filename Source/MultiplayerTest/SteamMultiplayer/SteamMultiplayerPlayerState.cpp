// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "SteamMultiplayerPlayerState.h"
#include "SteamMultiplayerPlayerController.h"
#include "Net/UnrealNetwork.h"

ASteamMultiplayerPlayerState::ASteamMultiplayerPlayerState()
{
	bReplicates = true;
}

void ASteamMultiplayerPlayerState::BeginPlay()
{
	Super::BeginPlay();


}

void ASteamMultiplayerPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASteamMultiplayerPlayerState, CurrentState);
}

void ASteamMultiplayerPlayerState::SwitchState(const EPlayerStates NewState)
{
	CurrentState = NewState;

	UE_LOG(LogTemp, Log, TEXT("Player: %s - CurrentState: %s"), *GetPlayerName(), *UEnum::GetValueAsString(CurrentState));

	ASteamMultiplayerPlayerController* PC = Cast<ASteamMultiplayerPlayerController>(GetOwningController());

	if (!PC)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid Player Controller"));
		return;
	}

	switch (CurrentState)
	{
		case EPlayerStates::E_Countdown:
			Owner->GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ASteamMultiplayerPlayerState::OnCountdownFinished, CountdownTime, false);
			break;
		case EPlayerStates::E_InProgress:
			PC->SetTurnInputEnabled(true);
			break;
		case EPlayerStates::E_EndTurn:
			PC->SetTurnInputEnabled(false);
			SwitchState(EPlayerStates::E_None);
			break;
		default:
			break;
	}
}

void ASteamMultiplayerPlayerState::StartTurn()
{
	SwitchState(EPlayerStates::E_Countdown);
}

void ASteamMultiplayerPlayerState::EndTurn()
{
	SwitchState(EPlayerStates::E_EndTurn);
}

void ASteamMultiplayerPlayerState::OnCountdownFinished()
{
	SwitchState(EPlayerStates::E_InProgress);
}
