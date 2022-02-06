// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"
#include "SunWukongCharacter.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DefaultStamina = 100;
	Stamina = DefaultStamina;
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* owner = GetOwner();

	// Ideally stamina component should only depend on health/attack components
	// AActor has a GetComponentByClass method
	ASunWukongCharacter* sunWuKongReference = Cast<ASunWukongCharacter>(owner);

	if (sunWuKongReference) {
		sunWuKongReference->AttackingDelegate.AddDynamic(this, &UStaminaComponent::SpendAttackStamina);
		sunWuKongReference->BlockedAttackDelegate.AddDynamic(this, &UStaminaComponent::SpendBlockedAttackStamina);
	}
	
}

void UStaminaComponent::SpendBlockedAttackStamina()
{
	SpendStamina(10);
	DelayRegenStamina();

}

void UStaminaComponent::SpendAttackStamina()
{
	SpendStamina(20);
	DelayRegenStamina();

}

void UStaminaComponent::SpendStamina(float value) {
	Stamina -= value;
	if (Stamina < 0) {
		OutOfStaminaDelegate.Broadcast();
	}
}

void UStaminaComponent::DelayRegenStamina()
{
	CanRegenStamina = false;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("SetCanRegenStamina"), true);
	//Calling MyUsefulFunction after 2 seconds without looping
	GetWorld()->GetTimerManager().SetTimer(resumeStaminaRegenHandle, TimerDel, 2.f, false);
}

void UStaminaComponent::SetCanRegenStamina(bool flag)
{
	CanRegenStamina = flag;
}

// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CanRegenStamina && Stamina < DefaultStamina) {
		Stamina = FMath::Min(Stamina + 0.5f, 100.0f);
	}
}

bool UStaminaComponent::HasStamina()
{
	return Stamina > 0;
}

