// Copyright 2025, Rolling Pixels. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReplayComponent.generated.h"

USTRUCT(BlueprintType)
struct FReplayFrame
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite)
	float TimeStamp;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReplayStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReplayFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class REPLAYSYSTEM_API UReplayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UReplayComponent();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnReplayStarted OnReplayStarted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnReplayFinished OnReplayFinished;

protected:
	UFUNCTION(BlueprintCallable)
	void SetReplayFrames(TArray<FReplayFrame> Frames) { ReplayFrames = Frames; };

	UFUNCTION(BlueprintImplementableEvent)
	void OnReplayFramesReplicated();

	UFUNCTION()
	void OnRep_ReplayFrames();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ReplayFrames)
	TArray<FReplayFrame> ReplayFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FReplayFrame> SavedFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxReplayDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecordInterval = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeSinceLastRecord = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsReplaying = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReplayTime = 0.0f;
};
