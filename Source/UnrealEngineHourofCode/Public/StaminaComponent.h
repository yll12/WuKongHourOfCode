// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfStamina);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALENGINEHOUROFCODE_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
		void SpendAttackStamina();

	UFUNCTION()
		void SpendStamina(float value);

	UFUNCTION()
		void SpendBlockedAttackStamina();

	UFUNCTION()
		void DelayRegenStamina();

	FTimerHandle resumeStaminaRegenHandle;

	UPROPERTY(BlueprintReadOnly)
		bool CanRegenStamina;

	UFUNCTION()
		void SetCanRegenStamina(bool flag);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float DefaultStamina;

	UPROPERTY(BlueprintReadOnly)
		float Stamina;

	UPROPERTY(BlueprintAssignable)
		FOutOfStamina OutOfStaminaDelegate;

	/** Functions */
	UFUNCTION(BlueprintPure, Category = "Stamina")
		bool HasStamina();

};
