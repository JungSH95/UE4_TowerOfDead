#include "TODMeteor.h"

ATODMeteor::ATODMeteor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeteorTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("METEORTRIGGERSPHERE"));
	MeteorTrigger->SetupAttachment(RootComponent);
	MeteorTrigger->SetSphereRadius(110.0f);

	MeteorEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("METEOREFFECT"));
	MeteorEffect->SetupAttachment(MeteorTrigger);

	// Meteor Effect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_METEORSKILLHITEFFECT(TEXT("/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_Meteora_01.Par_Meteora_01"));
	if (P_METEORSKILLHITEFFECT.Succeeded())
		MeteorEffect->SetTemplate(P_METEORSKILLHITEFFECT.Object);

	// Ãæµ¹ ½Ã Æø¹ß Effect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_METEOREXPLODESKILLHITEFFECT(TEXT("/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_Meteora_Explode_01.Par_Meteora_Explode_01"));
	if (P_METEOREXPLODESKILLHITEFFECT.Succeeded())
		MeteorHitEffect = P_METEOREXPLODESKILLHITEFFECT.Object;
}

void ATODMeteor::BeginPlay()
{
	Super::BeginPlay();
	
	MeteorTrigger->SetSimulatePhysics(true);
}

void ATODMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATODMeteor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeteorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATODMeteor::OnMeteorTriggerOverlap);
}

void ATODMeteor::StartMeteor()
{

}

void ATODMeteor::OnMeteorTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	print(OtherActor->GetName());

	MeteorEffect->SetTemplate(MeteorHitEffect);
}