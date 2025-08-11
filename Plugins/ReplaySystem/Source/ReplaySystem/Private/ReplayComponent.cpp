// Copyright 2025, Rolling Pixels. All Rights Reserved

#include "ReplayComponent.h"
#include "Net/UnrealNetwork.h"

UReplayComponent::UReplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UReplayComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UReplayComponent, ReplayFrames);
}

void UReplayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UReplayComponent::OnRep_ReplayFrames()
{
	OnReplayFramesReplicated();
}

