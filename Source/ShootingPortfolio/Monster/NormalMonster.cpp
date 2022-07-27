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

void ANormalMonster::ReceiveDamage(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, class AController* _InstigatorController, AActor* _DamageCauser)
{
	Super::ReceiveDamage(_DamagedActor, _Damage, _DamageType, _InstigatorController, _DamageCauser);

	UpdateHPBarWidget();
}
