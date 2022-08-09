#include "ShootingGameInstance.h"

UShootingGameInstance::UShootingGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CrosshairDataTable(TEXT("DataTable'/Game/Game/Blueprints/Player/DataTable/CrosshairDataTable.CrosshairDataTable'"));
	if (CrosshairDataTable.Succeeded())
		m_CrosshairDataTable = CrosshairDataTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> MinionAttackInfoDataTable(TEXT("DataTable'/Game/Game/Blueprints/Monster/Minions/DataTable/MinionsAttackInfoDataTable.MinionsAttackInfoDataTable'"));
	if (MinionAttackInfoDataTable.Succeeded())
		m_MonsterAttackInfoDataTables.Add(TEXT("Minions"), MinionAttackInfoDataTable.Object);

	static ConstructorHelpers::FObjectFinder<UDataTable> SteelAttackInfoDataTable(TEXT("DataTable'/Game/Game/Blueprints/Monster/Steel/DataTable/SteelAttackInfoDataTable.SteelAttackInfoDataTable'"));
	if (SteelAttackInfoDataTable.Succeeded())
		m_MonsterAttackInfoDataTables.Add(TEXT("Steel"), SteelAttackInfoDataTable.Object);

	static ConstructorHelpers::FObjectFinder<UDataTable> KhaimeraAttackInfoDataTable(TEXT("DataTable'/Game/Game/Blueprints/Monster/Khaimera/DataTable/KhaimeraAttackInfoDataTable.KhaimeraAttackInfoDataTable'"));
	if (KhaimeraAttackInfoDataTable.Succeeded())
		m_MonsterAttackInfoDataTables.Add(TEXT("Khaimera"), KhaimeraAttackInfoDataTable.Object);
}

FCrosshairTexture* UShootingGameInstance::GetCrosshairTexture(const FName& _TypeName) const
{
	if (m_CrosshairDataTable == nullptr)
		return nullptr;

	return m_CrosshairDataTable->FindRow<FCrosshairTexture>(_TypeName, TEXT(""));
}

UDataTable* UShootingGameInstance::GetMonsterAttackInfoDataTable(const FName& _MonsterName) const
{
	if (m_MonsterAttackInfoDataTables.Contains(_MonsterName) == false)
		return nullptr;

	return m_MonsterAttackInfoDataTables[_MonsterName];
}
