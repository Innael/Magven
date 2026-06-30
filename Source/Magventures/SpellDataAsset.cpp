// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "SpellDataAsset.h"


FText USpellDataAsset::GetSchoolDisplayName() const
{
	// Находим наше C++ перечисление в системе рефлексии движка
	const UEnum* EnumPtr = StaticEnum<ECharacterSkill>();
	if (!EnumPtr)
		return FText::GetEmpty();

	// Берём DisplayName конкретного элемента, который записан в ассете
	return EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(SpellData.MagicSchool));
}

FText USpellDataAsset::GetTargetTypeDisplayName() const
{
	const UEnum* EnumPtr = StaticEnum<ESpellTargetType>();
	if (!EnumPtr)
		return FText::GetEmpty();

	return EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(SpellData.TargetType));
}