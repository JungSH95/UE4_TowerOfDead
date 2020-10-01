#pragma once

#include "TowerOfDead.h"
#include "Components/ActorComponent.h"
#include "TODCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWEROFDEAD_API UTODCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTODCharacterStatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
		
private:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float CurrentHP;
};
