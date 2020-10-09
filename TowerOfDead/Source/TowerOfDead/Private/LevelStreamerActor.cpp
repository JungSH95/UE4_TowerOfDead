#include "LevelStreamerActor.h"
#include "TODCharacter.h"
#include "Kismet/GameplayStatics.h"

ALevelStreamerActor::ALevelStreamerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	Tags.Add("Portal");

	OverlapVolume->SetGenerateOverlapEvents(false);
	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelStreamerActor::OverlapBegins);
}

void ALevelStreamerActor::SetNextLevel(FName stagelevel)
{
	if (stagelevel == NAME_None)
	{
		return;
	}

	LevelToLoad = stagelevel;
}

void ALevelStreamerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelStreamerActor::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	TODLOG_S(Warning);

	ATODCharacter* MyCharacter = Cast<ATODCharacter>(OtherActor);
	if (MyCharacter != nullptr && LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);

		/*
		// 언로드 방법
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo);
		*/
	}
}

