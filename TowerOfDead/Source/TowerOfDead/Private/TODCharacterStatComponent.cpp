#include "TODCharacterStatComponent.h"

UTODCharacterStatComponent::UTODCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


void UTODCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UTODCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

