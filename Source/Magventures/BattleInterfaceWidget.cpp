// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "BattleInterfaceWidget.h"
#include "Components/RichTextBlock.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"

void UBattleInterfaceWidget::AddLogEntry(const FString& Entry){
	if (!LogBox)
		return;

	// Создаём TextBlock
	URichTextBlock* NewText = NewObject<URichTextBlock>(LogBox);
	if (NewText)
	{
		NewText->SetTextStyleSet(DT_BattleLog);

		NewText->SetText(FText::FromString(Entry));
		NewText->SetAutoWrapText(true);
		NewText->SetJustification(ETextJustify::Left);
		//NewText->SetPadding(FMargin(5.f, 2.f, 5.f, 2.f));

		if (LogBox->GetChildrenCount() >= MaxLogLines)
		{
			LogBox->RemoveChildAt(0);
		}

		// Добавляем в VerticalBox
		LogBox->AddChild(NewText);

		// Форсируем обновление геометрии, чтобы ScrollBox узнал о новом размере контента
		BatLogScrollBox->ForceLayoutPrepass();
		BatLogScrollBox->ScrollToEnd();
	}
}

void UBattleInterfaceWidget::SetSelectedZone(int32 ZoneIndex)
{
	if (SelectedZone != ZoneIndex)
	{
		SelectedZone = ZoneIndex;
		// Если нужно, можно вызвать событие Blueprint
		// OnSelectedZoneChanged.Broadcast(SelectedZone);
	}
}