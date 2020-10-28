#include "TODMeteor.h"
#include "TODCharacter.h"
#include "Components/DecalComponent.h"

ATODMeteor::ATODMeteor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeteorTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("METEORTRIGGERSPHERE"));
	MeteorTrigger->SetupAttachment(RootComponent);
	MeteorTrigger->SetSphereRadius(110.0f);
	MeteorTrigger->SetCollisionProfileName(TEXT("Meteor"));

	ExplodeTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("EXPLODETRIGGERSPHERE"));
	ExplodeTrigger->SetupAttachment(MeteorTrigger);
	ExplodeTrigger->SetSphereRadius(300.0f);

	Decal = CreateDefaultSubobject <UDecalComponent>(TEXT("DECAL"));
	Decal->SetupAttachment(RootComponent);
	Decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Decal->SetVisibility(false);

	MeteorEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("METEOREFFECT"));
	MeteorEffect->SetupAttachment(MeteorTrigger);
	MeteorEffect->SetVisibility(false);

	// Meteor Effect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_METEORSKILLHITEFFECT(TEXT("/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_Meteora_01.Par_Meteora_01"));
	if (P_METEORSKILLHITEFFECT.Succeeded())
		BaseEffect = P_METEORSKILLHITEFFECT.Object;

	// 충돌 시 폭발 Effect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_METEOREXPLODESKILLHITEFFECT(TEXT("/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_Meteora_Explode_01.Par_Meteora_Explode_01"));
	if (P_METEOREXPLODESKILLHITEFFECT.Succeeded())
		MeteorHitEffect = P_METEOREXPLODESKILLHITEFFECT.Object;

	MeteorDamage = 1.0f;
}

void ATODMeteor::BeginPlay()
{
	Super::BeginPlay();
}

void ATODMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DecalLocation = GetActorLocation();
	DecalLocation.Z = 0.0f;
	Decal->SetWorldLocation(DecalLocation);
}

void ATODMeteor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeteorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATODMeteor::OnMeteorTriggerOverlap);
	ExplodeTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATODMeteor::OnExplodeTriggerOverlap);
}

void ATODMeteor::SetMeteor(float damage)
{
	MeteorEffect->SetTemplate(BaseEffect);
	MeteorDamage = damage;

	float RandTime = FMath::FRandRange(2.0f, 7.0f);
	GetWorldTimerManager().SetTimer(MeteorDropTimerHandle, this, &ATODMeteor::MeteorDropStart,
		RandTime, false);
}

void ATODMeteor::MeteorDropStart()
{
	MeteorEffect->SetVisibility(true);
	MeteorTrigger->SetSimulatePhysics(true);
	Decal->SetVisibility(true);

	MeteorTrigger->SetGenerateOverlapEvents(true);
	ExplodeTrigger->SetGenerateOverlapEvents(false);
}

void ATODMeteor::MeteorExplodeCheckEnd()
{
	ExplodeTrigger->SetGenerateOverlapEvents(false);
}

void ATODMeteor::OnMeteorTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATODCharacter* Player = Cast<ATODCharacter>(OtherActor);
	if (Player != nullptr)
	{
		print(FString::Printf(TEXT("Meteor Overlap is Player")));

		// 데미지 처리
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(MeteorDamage, DamageEvent, nullptr, this);
	}
	// Player가 맞지 않았다면 Explode로 2차 Damage 체크
	else
	{
		ExplodeTrigger->SetGenerateOverlapEvents(true);

		GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &ATODMeteor::MeteorExplodeCheckEnd,
			0.6f, false);
	}

	Decal->SetVisibility(false);

	MeteorEffect->SetTemplate(MeteorHitEffect);
	MeteorEffect->Activate(true);

	MeteorTrigger->SetSimulatePhysics(false);
	MeteorTrigger->SetGenerateOverlapEvents(false);
}

void ATODMeteor::OnExplodeTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATODCharacter* Player = Cast<ATODCharacter>(OtherActor);
	if (Player != nullptr)
	{
		print(FString::Printf(TEXT("Explode Overlap is Player")));

		// 데미지 처리
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(MeteorDamage, DamageEvent, nullptr, this);

		ExplodeTrigger->SetGenerateOverlapEvents(false);
	}
}