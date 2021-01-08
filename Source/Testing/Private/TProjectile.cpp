// Fill out your copyright notice in the Description page of Project Settings.


#include "TProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstance.h"
#include "GameFramework/DamageType.h"

// Sets default values
ATProjectile::ATProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	CollisionComponent->InitSphereRadius(10.f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->SetNotifyRigidBodyCollision(true);


	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;

	TrailParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("Trail Particle");
	

	RootComponent = CollisionComponent;
	ProjectileMesh->SetupAttachment(RootComponent);
	TrailParticleComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->Friction = 1.f;


}

// Called when the game starts or when spawned
void ATProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentHit.AddDynamic(this, &ATProjectile::OnHit);
	if (GetInstigator() != NULL)
		CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	
}

// Called every frame
void ATProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATProjectile::Explode()
{
	ExplodeHandle.Invalidate();
	UE_LOG(LogTemp, Warning, TEXT("Kaboom"))
}

void ATProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetOwner())
		return;

	if(TrailParticleComp)
		TrailParticleComp->Deactivate();

	if (bIsEexplosive)
		Explode();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("On projectile hit"));

	if ((OtherActor != NULL) && (OtherComp != NULL))
	{

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("On projectile daamage"));
		if(ProjectileDamage)
		{
			AController* Con = this->GetInstigator() == NULL ? NULL : this->GetInstigator()->GetController();
			UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileDamage, GetActorLocation(), Hit, Con, this, UDamageType::StaticClass());		
		}

		UPrimitiveComponent* Prim_ = Cast<UPrimitiveComponent>(OtherActor);
		if (OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulseAtLocation(GetVelocity() / (Prim_ == NULL ? 1 : Prim_->GetMass()), GetActorLocation());
	}
	
	ProjectileMovement->ProjectileGravityScale = 1;
	FHitResult hit;
	ApplyPostHitEffects(hit);

	//Destroy();
}



void ATProjectile::SetProjectileProperties(bool BreakIntoBits /*=false*/, float InitialSpeed_ /*= 300.0f*/, float LaunchSpeed_ /*= 300*/, float MaxSpeed_ /*= 600*/, float MinSpeed_ /*= 100*/, bool bShouldBounce_ /*= true*/, bool bShouldRotationFollowVelocity_ /*= true*/, float GravityScale_ /*= .1f*/, float LifeSpanTime_ /*= 5*/, float Damage_ /*= 30*/, bool bIsEexplosive_ /*= false*/, float BlastDamage_ /*= 30.f*/, float BlastRadius_ /*= 1*/, float DamageFalloff_ /*= 30*/, class UParticleSystemComponent* TrailParticleComp_ /*= NULL*/, bool Penetrable_ /*= false*/)

{
	if (ProjectileMovement != NULL)
	{
		ProjectileMovement->InitialSpeed = InitialSpeed_;
		ProjectileMovement->MaxSpeed = MaxSpeed_;
		ProjectileMovement->bShouldBounce = bShouldBounce_;
		ProjectileMovement->ProjectileGravityScale = GravityScale_;
		ProjectileMovement->bRotationFollowsVelocity = bShouldRotationFollowVelocity_;

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile movement missing"));
	}

	ProjectileDamage = Damage_;
	this->BlastDamage = BlastDamage_;
	this->BlastRadius = BlastRadius_;
	this->DamageFalloff = DamageFalloff_;
	this->bIsEexplosive = bIsEexplosive_;
	this->bIsPenetrable = Penetrable_;
	this->SetLifeSpan(LifeSpanTime_ + 1);

	ProjectileMovement->SetVelocityInLocalSpace({LaunchSpeed_, 0.f, 0.f});

	if (TrailParticleComp != NULL)
		this->TrailParticleComp = TrailParticleComp_;

	if (bIsEexplosive == true)
	{
		GetWorldTimerManager().SetTimer(ExplodeHandle, this, &ATProjectile::Explode, 0, false, LifeSpanTime_);
	}

}

void ATProjectile::OnHitPenetrate()
{

}


void ATProjectile::ApplyPostHitEffects(const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Apply Post Effects"))
}


void ATProjectile::ProjectileBreak(bool Should, FVector HitNormal, int32 NumberToBreakApartInto)
{
	// if(!Should)
	// 	return;
	// FRandomStream RS;
	// for (int i = 0; i != NumberToBreakApartInto; i++)
	// {
	// 	//@TODO check if it doesn't overflow
	// 	FActorSpawnParameters SpawnParameters;
	//
	// 	GetWorld()->SpawnActor<AActor>(BreakApartClass, RootComponent->GetComponentLocation(), UKismetMathLibrary::MakeRotFromX(RS.VRandCone(HitNormal, 45, 45)), SpawnParameters);
	// 	//add to pool
	// }
	
}