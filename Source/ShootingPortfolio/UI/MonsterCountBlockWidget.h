#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "MonsterCountBlockWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTINGPORTFOLIO_API UMonsterCountBlockWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MonsterName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MonsterCount;

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};