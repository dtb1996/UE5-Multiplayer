// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "SteamMultiplayerPlayerController.h"
#include "SteamMultiplayerGameMode.h"
#include "SteamMultiplayerGameState.h"
#include "SteamMultiplayerPlayerState.h"
#include "EnhancedInputSubsystems.h"

ASteamMultiplayerPlayerController::ASteamMultiplayerPlayerController()
{
    bReplicates = true;
    bOnlyRelevantToOwner = true;
}

void ASteamMultiplayerPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetIgnoreMoveInput(true);
    SetIgnoreLookInput(true);

    //AGolfGameState* GS = GetWorld()->GetGameState<AGolfGameState>();
    //if (GS && GS->ActiveViewTarget)
    //{
    //    SetViewTargetWithBlend(GS->ActiveViewTarget, 0.5f);
    //}
}

void ASteamMultiplayerPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();

    // This is equivalent to client-side "possess"
    if (GetPawn())
    {
        UE_LOG(LogTemp, Log, TEXT("Client received pawn via OnRep_Pawn"));

        // Enable input, or set up input mappings
        if (IsLocalController())
        {
            //EnableTurnInput(); // or AddMappingContext()
        }
    }
}

void ASteamMultiplayerPlayerController::Server_NotifyFinishedTurn_Implementation(bool bSuccess, bool bWantsRetry)
{
    ClientEndTurn();

    ASteamMultiplayerGameMode* GameMode = Cast<ASteamMultiplayerGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->HandlePlayerTurnFinished(this, bSuccess, bWantsRetry);
    }
}

void ASteamMultiplayerPlayerController::ClientStartTurn_Implementation()
{
    //EnableTurnInput();

    ASteamMultiplayerPlayerState* PS = Cast<ASteamMultiplayerPlayerState>(PlayerState);
    if (PS)
    {
        PS->StartTurn();
    }
}

void ASteamMultiplayerPlayerController::ClientEndTurn_Implementation()
{
    //DisableTurnInput();
    ASteamMultiplayerPlayerState* PS = Cast<ASteamMultiplayerPlayerState>(PlayerState);
    if (PS)
    {
        PS->EndTurn();
    }
}

void ASteamMultiplayerPlayerController::SetTurnInputEnabled_Implementation(const bool bNewIsEnabled)
{
    //if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    //{
    //    Subsystem->AddMappingContext(GolfInputMappingContext, 0);
    //}

    SetIgnoreMoveInput(!bNewIsEnabled);
    SetIgnoreLookInput(!bNewIsEnabled);
}
