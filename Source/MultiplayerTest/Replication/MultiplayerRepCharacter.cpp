// Copyright 2025, Bit Gaming Studio. All Rights Reserved


#include "MultiplayerRepCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MultiplayerRepProjectile.h"

AMultiplayerRepCharacter::AMultiplayerRepCharacter()
{
	// Initialize the character's health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//Initialize projectile class
	ProjectileClass = AMultiplayerRepProjectile::StaticClass();
	//Initialize fire rate
	FireRate = 0.25f;
	bIsFiringWeapon = false;
}

void AMultiplayerRepCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerRepCharacter, CurrentHealth);
}

void AMultiplayerRepCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void AMultiplayerRepCharacter::OnHealthUpdate()
{
	// Client-specific functionality
	if (IsLocallyControlled())
	{
		FString HealthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, HealthMessage);

		if (CurrentHealth <= 0)
		{
			FString DeathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DeathMessage);
		}
	}

	// Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString HealthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, HealthMessage);
	}

	// Code that occurs on all machines
	/*
		Any special functionality that should occur as a result of damage or death should be placed here.
	*/
}

void AMultiplayerRepCharacter::SetCurrentHealth(float HealthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(HealthValue, 0.0f, MaxHealth);

		/*
			Calls OnHealthUpdate to ensure, that the server and clients both have parallel calls to this
			function. This is necessary, because the server will not receive the RepNotify.
		*/
		OnHealthUpdate();
	}
}

float AMultiplayerRepCharacter::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(DamageApplied);
	return DamageApplied;
}

void AMultiplayerRepCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &AMultiplayerRepCharacter::StopFire, FireRate, false);
		HandleFire();
	}
}

void AMultiplayerRepCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

void AMultiplayerRepCharacter::HandleFire_Implementation()
{
	FVector SpawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = this;

	AMultiplayerRepProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AMultiplayerRepProjectile>(SpawnLocation, SpawnRotation, SpawnParameters);
}
