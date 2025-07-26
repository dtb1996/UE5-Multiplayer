// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedStage.h"
#include "StageData.generated.h"

USTRUCT(BlueprintType)
struct FStageData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText StageName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ATurnBasedStage> StageClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UWorld> StageAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeLimitSeconds = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USoundBase> MusicTrack;

    FStageData() = default;
};
