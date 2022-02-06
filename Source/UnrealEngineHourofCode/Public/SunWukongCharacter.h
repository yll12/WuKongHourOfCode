// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitReactInterface.h"
#include "SunWukongCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttacking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBlockedAttack);

UENUM()
enum Direction
{
	Front  UMETA(DisplayName = "Front"),
	Back   UMETA(DisplayName = "Back"),
	Left   UMETA(DisplayName = "Left"),
	Right  UMETA(DisplayName = "Right"),
};

UCLASS()
class UNREALENGINEHOUROFCODE_API ASunWukongCharacter : public ACharacter, public IHitReactInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASunWukongCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/** Properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackSystem)
		int AttackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackSystem)
		bool SaveAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackSystem)
		bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlockSystem)
		bool IsBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HealthSystem)
		float Health;

	UPROPERTY(BlueprintAssignable)
		FAttacking AttackingDelegate;

	UPROPERTY(BlueprintAssignable)
		FBlockedAttack BlockedAttackDelegate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to bind functionality to input
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void calculateNewHealth(float Damage);

	/** Attack System */
	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
		void PerformCombo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AttackSystem")
		void ResetCombo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AttackSystem")
		void ComboAttackSave();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AttackSystem")
		void PerformPrimaryAttackA();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AttackSystem")
		void PerformPrimaryAttackB();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AttackSystem")
		void PerformPrimaryAttackC();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AttackSystem")
		void PerformPrimaryAttackD();

	/** HitReact System */
	UFUNCTION(BlueprintNativeEvent, BlueprintInternalUseOnly, Category = "HitReactInterface")
		void HitReact(AActor* damageCauser, const struct FImpactParticleSystemStruct &impactParticleSystem);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HitReactInterface")
		void HitReactPlayBlockEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HitReactInterface")
		void HitReactPlayGetHit(const struct FImpactParticleSystemStruct &impactParticleSystem, Direction direction);

	/** Health System */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HealthSystem")
		void UpdateHealthWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HealthSystem")
		void CharacterHasDied();

	UFUNCTION(BlueprintPure, Category = "AttackSystem")
		bool IsAlive();

private:

	void JumpPressed();
	void JumpReleased();
	void Attack();
	Direction CalculateDirection(FRotator normalizedRotation);
	bool IsFacing(AActor* damageCauser);
};
