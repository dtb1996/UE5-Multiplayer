// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnBasedStage.generated.h"

UCLASS(Abstract)
class MULTIPLAYERTEST_API ATurnBasedStage : public AActor
{
	GENERATED_BODY()

public:
	ATurnBasedStage();

protected:
	virtual void BeginPlay() override;
};
