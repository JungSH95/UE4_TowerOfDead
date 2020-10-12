#include "LevelStreamerActor.h"
#include "TODCharacter.h"
#include "TODGameMode.h"
#include "TODStageManager.h"
#include "Kismet/GameplayStatics.h"

ALevelStreamerActor::ALevelStreamerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	PortalEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	PortalEffect->SetupAttachment(OverlapVolume);

	Tags.Add("Portal");

	OverlapVolume->SetGenerateOverlapEvents(false);
	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelStreamerActor::OverlapBegins);

	NowLevel = "";
}

void ALevelStreamerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelStreamerActor::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ATODCharacter* MyCharacter = Cast<ATODCharacter>(OtherActor);
	if (MyCharacter != nullptr && LevelToLoad != "")
	{
		OverlapVolume->SetGenerateOverlapEvents(false);

		ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode != nullptr)
			gameMode->PlayFadeOut();

		if (LevelToLoad != NowLevel)
		{
			FLatentActionInfo LatentInfo;
			UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
		}
		else
			// 해당 스테이지 재 시작.
			StageManager->ReSetStage();

		/*
		// 언로드 방법
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo);
		*/
	}
}

void ALevelStreamerActor::SetNextLevel(FName stagelevel)
{
	if (stagelevel == NAME_None)
		return;

	LevelToLoad = stagelevel;
}

void ALevelStreamerActor::SetPortalEffectActive(bool isActive)
{
	if (PortalEffect == nullptr)
		return;

	PortalEffect->SetVisibility(isActive);
	PortalEffect->Activate(isActive);
}

void ALevelStreamerActor::SetNowLevel(ATODStageManager* manager, FName level)
{
	if (level == NAME_None || manager == nullptr)
		return;

	NowLevel = level;
	StageManager = manager;
}