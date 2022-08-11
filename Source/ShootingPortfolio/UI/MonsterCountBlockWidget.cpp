#include "MonsterCountBlockWidget.h"

#include "Components/TextBlock.h"
#include "ShootingPortfolio/Monster/SpawnMonsterData.h"

void UMonsterCountBlockWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (MonsterName == nullptr || MonsterCount == nullptr)
		return;

	USpawnMonsterData* Data = Cast<USpawnMonsterData>(ListItemObject);
	if (Data == nullptr)
		return;

	MonsterName->SetText(FText::FromName(Data->GetMonsterName()));
	MonsterCount->SetText(FText::AsNumber(Data->GetSpawnCount()));
}