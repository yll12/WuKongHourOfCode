// Fill out your copyright notice in the Description page of Project Settings.


#include "SunWukongCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASunWukongCharacter::ASunWukongCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASunWukongCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASunWukongCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASunWukongCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASunWukongCharacter::JumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASunWukongCharacter::JumpReleased);
}

float ASunWukongCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsAlive()) {
		if (IsFacing(DamageCauser) && IsBlocking) {
			return 0; // No Damage Taken
		}
		else {
			calculateNewHealth(Damage);
			UpdateHealthWidget();
			if (Health > 0) {
				ResetCombo();
			}
			else {
				CharacterHasDied();
				DetachFromControllerPendingDestroy();
				GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
			}
		}
	}

	return Damage;
}

bool ASunWukongCharacter::IsAlive()
{
	return Health > 0;
}

void ASunWukongCharacter::calculateNewHealth(float Damage)
{
	if (Health > 0) {
		Health -= Damage;
	}
}

void ASunWukongCharacter::PerformCombo()
{
	if (IsAttacking) {
		SaveAttack = true;
	} else {
		IsAttacking = true;
		Attack();
	}
}

void ASunWukongCharacter::ResetCombo_Implementation()
{
	AttackCount = 0;
	SaveAttack = false;
	IsAttacking = false;
}

void ASunWukongCharacter::ComboAttackSave_Implementation()
{
	if (SaveAttack) {
		SaveAttack = false;
		Attack();
	}
}

void ASunWukongCharacter::HitReact_Implementation(AActor* damageCauser, const struct FImpactParticleSystemStruct &impactParticleSystem)
{
	if (IsFacing(damageCauser) && IsBlocking) {
		BlockedAttackDelegate.Broadcast();
		HitReactPlayBlockEffect();
	} else {
		FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetTargetLocation(), damageCauser->GetActorLocation());
		FRotator normalizedRotation = UKismetMathLibrary::NormalizedDeltaRotator(lookAtRotation, GetActorRotation());
		FRotator particleRotation = FRotator(impactParticleSystem.ImpactDesiredRotation.Pitch, 
			                                 impactParticleSystem.ImpactDesiredRotation.Yaw, 
			                                 impactParticleSystem.ImpactDesiredRotation.Roll);
		FRotator actualRotationRequired = UKismetMathLibrary::NormalizedDeltaRotator(normalizedRotation, particleRotation);
		struct FImpactParticleSystemStruct resultImpactSystem = impactParticleSystem;
		resultImpactSystem.ImpactDesiredRotation = actualRotationRequired;
		HitReactPlayGetHit(resultImpactSystem, CalculateDirection(normalizedRotation));
	}
}

Direction ASunWukongCharacter::CalculateDirection(FRotator normalizedRotation)
{
	if (-45 < normalizedRotation.Yaw && normalizedRotation.Yaw <= 45) {
		return Front;
	}
	else if (-135 < normalizedRotation.Yaw && normalizedRotation.Yaw <= -45) {
		return Left;
	}
	else if (45 < normalizedRotation.Yaw && normalizedRotation.Yaw <= 135) {
		return Right;
	}
	else {
		return Back;
	}
}

bool ASunWukongCharacter::IsFacing(AActor* actor)
{
	float ZDiff = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), actor->GetActorRotation()).Yaw;
	return ZDiff < -150 || ZDiff > 150;
}

void ASunWukongCharacter::JumpPressed()
{
	UCharacterMovementComponent* charactermovement = GetCharacterMovement();
	if (!charactermovement->IsFalling()) {
		Jump();
	}
}

void ASunWukongCharacter::JumpReleased()
{
	StopJumping();
}

void ASunWukongCharacter::Attack() {
	AttackingDelegate.Broadcast();
	switch (AttackCount) {
	case 0:
		AttackCount = 1;
		PerformPrimaryAttackA();
		break;
	case 1:
		AttackCount = 2;
		PerformPrimaryAttackB();
		break;
	case 2:
		AttackCount = 3;
		PerformPrimaryAttackC();
		break;
	case 3:
		AttackCount = 0;
		PerformPrimaryAttackD();
		break;
	}
}



