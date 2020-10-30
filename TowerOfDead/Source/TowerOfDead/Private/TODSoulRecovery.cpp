#include "TODSoulRecovery.h"
#include "TODCharacter.h"
#include "TODPlayerController.h"

ATODSoulRecovery::ATODSoulRecovery()
{
	PrimaryActorTick.bCanEverTick = false;

	BaseGroundObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = BaseGroundObject;

	BaseEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	BaseEffect->SetupAttachment(RootComponent);

	PlayerCheckTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("PLAYERCHECKTRIGGER"));
	PlayerCheckTrigger->SetupAttachment(RootComponent);
	PlayerCheckTrigger->SetGenerateOverlapEvents(true);
	PlayerCheckTrigger->SetSphereRadius(300.0f);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BASEGROUND(TEXT("/Game/ParagonProps/Agora/Props/Meshes/Tower_Base.Tower_Base"));
	if (SM_BASEGROUND.Succeeded())
		BaseGroundObject->SetStaticMesh(SM_BASEGROUND.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_BASEEFFECT(TEXT("/Game/ParagonMinions/FX/Particles/PlayerBuffs/P_DroppedBuff_GoldBuff.P_DroppedBuff_GoldBuff"));
	if (P_BASEEFFECT.Succeeded())
		BaseEffect->SetTemplate(P_BASEEFFECT.Object);

	FVector NewLocation = GetActorLocation();
	NewLocation.Z += 200.0f;

	BaseEffect->SetWorldLocation(NewLocation);
	PlayerCheckTrigger->SetWorldLocation(NewLocation);
}

void ATODSoulRecovery::BeginPlay()
{
	Super::BeginPlay();
}

void ATODSoulRecovery::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PlayerCheckTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATODSoulRecovery::OnPlayerCheckTriggerBeginOverlap);
	PlayerCheckTrigger->OnComponentEndOverlap.AddDynamic(this, &ATODSoulRecovery::OnPlayerCheckTriggerEndOverlap);
}

void ATODSoulRecovery::OnPlayerCheckTriggerBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATODCharacter* Player = Cast<ATODCharacter>(OtherActor);
	if (Player != nullptr)
	{
		ATODPlayerController* pController = Cast<ATODPlayerController>(Player->GetController());
		if (pController != nullptr)
			pController->SetIsCanObjectInteraction(true);
	}
}

void ATODSoulRecovery::OnPlayerCheckTriggerEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATODCharacter* Player = Cast<ATODCharacter>(OtherActor);
	if (Player != nullptr)
	{
		ATODPlayerController* pController = Cast<ATODPlayerController>(Player->GetController());
		if (pController != nullptr)
			pController->SetIsCanObjectInteraction(false);
	}
}