#include "ShootingGameInstance.h"

UShootingGameInstance::UShootingGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CrosshairDataTable(TEXT("DataTable'/Game/Game/Blueprints/Player/DataTable/CrosshairDataTable.CrosshairDataTable'"));
	if (CrosshairDataTable.Succeeded())
		m_CrosshairDataTable = CrosshairDataTable.Object;
}

FCrosshairTexture* UShootingGameInstance::GetCrosshairTexture(FName _TypeName)
{
	return m_CrosshairDataTable->FindRow<FCrosshairTexture>(_TypeName, TEXT(""));
}