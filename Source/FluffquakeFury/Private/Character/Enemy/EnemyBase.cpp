// Copyright Alex Goulder


#include "Character/Enemy/EnemyBase.h"

#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "FluffquakeFury/FluffquakeFury.h"
#include "UI/FQFUserWidget.h"

AEnemyBase::AEnemyBase()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UFQFAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UFQFAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();


	if (UFQFUserWidget* FQFUserWidget = Cast<UFQFUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		FQFUserWidget->SetWidgetController(this);
	}

	if (const UFQFAttributeSet* FQFAS = Cast<UFQFAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FQFAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FQFAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(FQFAS->GetHealth());
		OnMaxHealthChanged.Broadcast(FQFAS->GetMaxHealth());
	}
	
}

void AEnemyBase::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UFQFAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();

}

void AEnemyBase::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyBase::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AEnemyBase::GetPlayerLevel()
{
	return Level;
}
