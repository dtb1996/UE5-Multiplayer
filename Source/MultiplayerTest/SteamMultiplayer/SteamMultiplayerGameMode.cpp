// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "SteamMultiplayerGameMode.h"
#include "SteamMultiplayerGameInstance.h"
#include "SteamMultiplayerGameState.h"
#include "SteamMultiplayerPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

ASteamMultiplayerGameMode::ASteamMultiplayerGameMode()
{
	bStartPlayersAsSpectators = true;
}

void ASteamMultiplayerGameMode::StartPlay()
{
	Super::StartPlay();

	USteamMultiplayerGameInstance* GameInstance = GetGameInstance<USteamMultiplayerGameInstance>();
	if (GameInstance)
	{
		ExpectedPlayerCount = GameInstance->GetExpectedPlayerCount();
	}
}

void ASteamMultiplayerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	USteamMultiplayerGameInstance* GameInstance = GetGameInstance<USteamMultiplayerGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	// Track logins
	if (GameState->PlayerArray.Num() == ExpectedPlayerCount)
	{
		bPlayersReady = true;

		StartTurns();
	}
}

void ASteamMultiplayerGameMode::HandlePlayerTurnFinished(APlayerController* PC, const bool bSuccess, const bool bWantsRetry)
{
	APlayerState* PS = PC ? PC->PlayerState : nullptr;
	int32 Index = TurnOrder.IndexOfByPredicate([&](const FPlayerTurn& Entry) {
		return Entry.Player == PS;
	});

	if (Index == INDEX_NONE)
	{
		return;
	}

	if (bSuccess && !bWantsRetry)
	{
		TurnOrder[Index].bCompleted = true;
		TurnOrder[Index].bWantsRetry = false;
		AdvanceTurn();
	}
	else
	{
		RetryPlayer(PC);
	}
}

void ASteamMultiplayerGameMode::StartNextStage_Implementation()
{

}

void ASteamMultiplayerGameMode::StartTurns()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting hole with %d players in PlayerArray"), GameState->PlayerArray.Num());

	TurnOrder.Empty();

	for (APlayerState* PS : GameState->PlayerArray)
	{
		UE_LOG(LogTemp, Log, TEXT(" - Player: %s"), *PS->GetPlayerName());
		TurnOrder.Add({ PS, false, false });
	}

	CurrentPlayerIndex = 0;
	BeginPlayerTurn(CurrentPlayerIndex);
}

void ASteamMultiplayerGameMode::BeginPlayerTurn(int32 PlayerIndex)
{
	if (!TurnOrder.IsValidIndex(PlayerIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid player index: %d"), PlayerIndex);
		return;
	}

	// Ensure all pawns are destroyed
	TArray<AActor*> FoundPawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), DefaultPawnClass, FoundPawns);

	for (AActor* Pawn : FoundPawns)
	{
		Pawn->Destroy();
	}

	APlayerState* PS = TurnOrder[PlayerIndex].Player;
	AController* PC = PS ? PS->GetOwner<AController>() : nullptr;

	if (!PC)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid player controller"));
		return;
	}

	// Find PlayerStart in the level
	AActor* PlayerStart = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass());

	if (!PlayerStart)
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerStart not found"));
		return;
	}

	// Spawn pawn manually
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PC;
	SpawnParams.Instigator = PC->GetPawn();

	FVector SpawnLocation = PlayerStart->GetActorLocation();
	FRotator SpawnRotation = PlayerStart->GetActorRotation();

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (!NewPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid NewPawn"));
		return;
	}

	PC->Possess(NewPawn);

	EnablePlayer(PC);

	// Disable all other players
	for (const FPlayerTurn& Entry : TurnOrder)
	{
		if (Entry.Player != PS)
		{
			AController* OtherPC = Entry.Player->GetOwner<AController>();
			if (OtherPC)
			{
				DisablePlayer(OtherPC);
			}
		}
	}

	// Set all views to active player
	APlayerController* ActivePlayer = Cast<APlayerController>(PS->GetOwner<AController>());

	if (ActivePlayer)
	{
		//SetAllViewsToActivePlayer(ActivePlayer);
	}

	// Notify Game State that turn has started
	ASteamMultiplayerGameState* SteamMultiplayerGameState = GetGameState<ASteamMultiplayerGameState>();
	if (SteamMultiplayerGameState)
	{
		//SteamMultiplayerGameState->StartCountdown();
	}
}

void ASteamMultiplayerGameMode::AdvanceTurn()
{
	++CurrentPlayerIndex;

	if (TurnOrder.IsValidIndex(CurrentPlayerIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("Advancing to next player, index: %d"), CurrentPlayerIndex);
		BeginPlayerTurn(CurrentPlayerIndex);
	}
	else
	{
		AllTurnsComplete();
	}
}

void ASteamMultiplayerGameMode::RetryPlayer(APlayerController* PC)
{
	if (PC->GetPawn())
	{
		PC->GetPawn()->Destroy();
	}

	//RestartPlayer(PC);
	BeginPlayerTurn(CurrentPlayerIndex);
}

void ASteamMultiplayerGameMode::AllTurnsComplete()
{
	UE_LOG(LogTemp, Log, TEXT("All players completed their turn! Advancing..."));
	StartNextStage();
}

void ASteamMultiplayerGameMode::EnablePlayer(AController* PC)
{
	//PC->SetIgnoreMoveInput(false);
	//PC->SetIgnoreLookInput(false);

	ASteamMultiplayerPlayerController* GPC = Cast<ASteamMultiplayerPlayerController>(PC);

	if (GPC)
	{
		GPC->ClientStartTurn();
	}
}

void ASteamMultiplayerGameMode::DisablePlayer(AController* PC)
{
	//PC->SetIgnoreMoveInput(true);
	//PC->SetIgnoreLookInput(true);

	ASteamMultiplayerPlayerController* GPC = Cast<ASteamMultiplayerPlayerController>(PC);

	if (GPC)
	{
		GPC->ClientEndTurn();
	}
}
