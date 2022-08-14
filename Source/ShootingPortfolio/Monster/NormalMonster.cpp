#include "NormalMonster.h"

#include "ShootingPortfolio/UI/MonsterHPBarWidget.h"
#include "Components/ProgressBar.h"

ANormalMonster::ANormalMonster()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidget(TEXT("WidgetBlueprint'/Game/Game/Blueprints/UI/WBP_MonsterHPBar.WBP_MonsterHPBar_C'"));
	if (HPBarWidget.Succeeded())
	{
		m_HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
		m_HPBarWidgetComponent->SetupAttachment(GetMesh());
		m_HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		m_HPBarWidgetComponent->SetWidgetClass(HPBarWidget.Class);
		m_HPBarWidgetComponent->SetDrawSize(FVector2D(150.f, 20.f));
		m_HPBarWidgetComponent->SetVisibility(false);
	}
}

void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_HPBarWidgetComponent)
		m_HPBarWidget = Cast<UMonsterHPBarWidget>(m_HPBarWidgetComponent->GetWidget());

	UpdateHPBarWidget();
}

void ANormalMonster::UpdateHPBarWidget()
{
	if (m_HPBarWidget == nullptr)
		return;

	float HPPercent = m_Status.CurHP / m_Status.MaxHP;
	m_HPBarWidget->HPBar->SetPercent(HPPercent);
}

float ANormalMonster::TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser)
{
	float ActualDamage = Super::TakeDamage(_DamageAmount, _DamageEvent, _EventInstigator, _DamageCauser);

	if (m_Status.CurHP <= 0.f)
	{
		Destroy();
		return ActualDamage;
	}

	if (m_HPBarWidgetComponent)
		m_HPBarWidgetComponent->SetVisibility(true);

	UpdateHPBarWidget();

	return ActualDamage;
}

void ANormalMonster::Destroyed()
{
	Super::Destroyed();

	if (m_MonsterDieDelegate.IsBound())
		m_MonsterDieDelegate.Execute(this->GetClass()->GetDefaultObject());
}
