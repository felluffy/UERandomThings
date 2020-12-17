// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUps.h"
#include "Engine/World.h"

#include "WeaponBase.generated.h"


UENUM()
enum WeaponType
{
	Burst UMETA(DisplayName = "Burst"),
	Single UMETA(DisplayName = "Single"),
	Full UMETA(DisplayName = "Full"),
};

UENUM()
enum Status
{
	Idle UMETA(DisplayName = "Idle"),
	Shooting UMETA(DisplayName = "Shooting"),
	ToReload UMETA(DisplayName = "ToReload"),
	Reloading UMETA(DisplayName = "Reloading"),
	Jammed UMETA(DisplayName = "Jammed"),
	Equipping UMETA(DisplayName = "Equipping")
};

UENUM()
enum Rairity
{
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
};

USTRUCT(BlueprintType)
struct FWeaponAnim
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* Pawn1P;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* Pawn3P;
};

UCLASS()
class TESTING_API AWeaponBase : public APickUps
{
	GENERATED_BODY()
public:
	AWeaponBase();

//Components
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* MuzzleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TArray<class UActorComponent*> AttachmentsToWeapon;

//Interactable Events
public:
	virtual void BeginPlay() override; //stock full auto hitscan

	virtual void Tick(float DeltaSeconds)override; 
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();
	
	virtual void OnFire(); //default to line trace
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnZoomIn();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnZoomOut();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();
	
	virtual void OnReload();

	virtual void HandleReload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void UnEquip();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Equip();

	virtual void OnEquip();

	virtual void OnUnEquip();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void PlayAfterEffects();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void PlayImpactParticles(EPhysicalSurface SurfaceType);
	
	virtual void HandleRecoil();

	void DrawWeaponCrossHair(bool ShouldDraw);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void AttachMeshToPawn();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void ConsumeAmmo(int32 NumberOfAmmoToConsume);

	bool IsMagEmpty();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachAttachment(UActorComponent* AttachToAdd);

	virtual void OnPressActionButton() override;// { /*StartFire();*/ }

	virtual void OnPressSecondaryButton() override;// { /*zoomin/other*/ }

	virtual void OnPressMainButton() override;// { StartFire(); }

	virtual void OnReleaseActionButton() override;// { /*StartFire();*/ }

	virtual void OnReleaseSecondaryButton() override; // { /*zoomin/other*/ }

	virtual void OnReleaseMainButton() override; //{ StopFire(); }

	virtual void OnDrop() override;

	virtual void OnPickUp() override;

	float PlayWeaponAnimation(const FWeaponAnim& Animation, float InPlayRate = 1.f);

	void StopWeaponAnimation(const FWeaponAnim& Animation);

    virtual void OnThrowDropAddImpulse(FVector Impulse, FVector Location, FName BoneName) override;

protected:
	void FindWhereToShootFrom(FVector &SpawnLocation, FRotator &SpawnRotation);

	virtual void HandleProjectile(FActorSpawnParameters &ActorSpawnParams, FVector &SpawnLocation, FRotator &SpawnRotation);

	float CalculateDamageByBodyPart(FName HitBone, float DefaultDamage);

	float CalculateDamageAfterPenetration(EPhysicalSurface, float DefaultDamage);
//Weapon attributes
protected: 
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TEnumAsByte<WeaponType> Type;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TEnumAsByte<Rairity> RairityWep;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TEnumAsByte<Status> CurrentStatus;
	
	float fIntervalShootingTime = .2f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	int32 AmmoCount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	int32 MagazineCapacity;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	int32 MaxAmmo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	int32 CurrentAmmoInMagazine;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TArray<int32> MagazineAmmoCost;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	int32 LowAmmoThreshold;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	bool bInfiniteAmmo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float FireInterval;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	FTimerHandle FireTimer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	FTimerHandle ReloadTimer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float SpreadXAxis;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float SpreadYAxis;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	FName AttachmentSocketName;// = FText(FString(""));
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	FName MuzzleSocketName;// = FText(FString(""));

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
		FName RootBoneName = TEXT("");

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TArray<FName> AttachMentLocations;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TArray<UParticleSystem*> MuzzleParticle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TArray<UParticleSystem*> ShellParticle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float ShellThrowForce;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float WeaponRange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float WeaponDamage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float ZoomFOV;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float DefaultReloadTime = 2.0f;

	/*UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float ScaleOnPickedUp = 
	*/
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TArray<USoundBase*> FireSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TArray<USoundBase*> ReloadSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TArray<USoundBase*> LowAmmoSound;

	UPROPERTY(BluePrintReadWrite, EditDefaultsOnly, Category = "Properties")
	FVector AimOffsetForAnim;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TSubclassOf<class UCameraShake> FireShake;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float FireSoundScale = .2f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float FireSoundRange = 2000.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float ReloadSoundScale = .2f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float ReloadSoundRange = 2000.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float fFireShakeAlpha = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	bool bRecoil = false;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	bool bZoomedIn = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	bool bDelayHitScan = false;

	float fLastFireTime;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	float Spread;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TArray<int32> NumberOfAmmoPerShotAsMode = {1};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TMap<TEnumAsByte<EPhysicalSurface>, float> DamageMultiplier;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TMap<FName, float> OnBodyDamageMultiplier;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FWeaponAnim FireAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FWeaponAnim FireAnimationSights;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FWeaponAnim ReloadAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FWeaponAnim ReloadAnimationSights;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FWeaponAnim EquipAnimation;

	UPROPERTY(EditDefaultsOnly, Transient)
	class USoundAttenuation* EmptyClipAttenuation;

	UPROPERTY(EditDefaultsOnly, Transient)
	class USoundAttenuation* ReloadSoundAttenuation;
	
	UPROPERTY(EditDefaultsOnly, Transient)
	class USoundAttenuation* FireSoundAttenuation;

private:
	FRandomStream RS;
};