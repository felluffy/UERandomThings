// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TProjectile.generated.h"



UCLASS()
class TESTING_API ATProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	
public:	
	// Sets default values for this actor's properties
	ATProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Explode();

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComponent; }
	
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(BlueprintCallable)
	virtual void SetProjectileProperties(bool BreakIntoBits = false,float InitialSpeed_ = 300.0f, float LaunchSpeed_ = 300, float MaxSpeed_ = 600, 
										float MinSpeed_ = 100, bool bShouldBounce_ = true,bool bShouldRotationFollowVelocity_ = true,
										float GravityScale_ = .1f, float LifeSpanTime_ = 5, float Damage_ = 30, 
										bool bIsEexplosive_ = false, float BlastDamage_ = 30.f, float BlastRadius_ = 1, float DamageFalloff_ = 30, 
										class UParticleSystemComponent* TrailParticleComp_ = NULL, bool Penetrable_ = false);

	UFUNCTION(BlueprintCallable)
	virtual void OnHitPenetrate();

	UFUNCTION(BlueprintCallable)
	virtual void ApplyPostHitEffects(const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	virtual void ProjectileBreak(bool Should = false, FVector HitNormal = FVector::ZeroVector, int32 NumberToBreakApartInto=0);

	UFUNCTION(BlueprintCallable)
	float GetProjectileMass() { return ProjectileMesh->GetMass(); };
	
private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float ProjectileDamage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsPenetrable;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float LifeSpan;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsEexplosive;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float BlastRadius;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float BlastDamage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float DamageFalloff;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FTimerHandle ExplodeHandle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* TrailParticleComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bAppliesDot;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float DOTRadius;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bBreakIntoPieces;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> BreakApartClass;

};
