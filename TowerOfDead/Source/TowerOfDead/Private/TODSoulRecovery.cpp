#include "TODSoulRecovery.h"
#include "TODCharacter.h"
#include "TODPlayerController.h"
#include "Components/WidgetComponent.h"

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

	KeyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("KEYWIDGET"));
	KeyWidget->SetupAttachment(RootComponent);
	KeyWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	KeyWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BASEGROUND(TEXT("/Game/ParagonProps/Agora/Props/Meshes/Tower_Base.Tower_Base"));
	if (SM_BASEGROUND.Succeeded())
		BaseGroundObject->SetStaticMesh(SM_BASEGROUND.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_BASEEFFECT(TEXT("/Game/ParagonMinions/FX/Particles/PlayerBuffs/P_DroppedBuff_GoldBuff.P_DroppedBuff_GoldBuff"));
	if (P_BASEEFFECT.Succeeded())
		BaseEffect->SetTemplate(P_BASEEFFECT.Object);

	static ConstructorHelpers::FClassFinder<UUserWidget> KEYWIDGET(TEXT("/Game/UI/Recovery_UI.Recovery_UI_C"));
	if (KEYWIDGET.Succeeded())
	{
		KeyWidget->SetWidgetClass(KEYWIDGET.Class);
		KeyWidget->SetDrawSize(FVector2D(130.0f, 45.0f));
	}

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