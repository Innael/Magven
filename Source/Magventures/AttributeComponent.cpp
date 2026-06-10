// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::InitializeCharacterSkills()
{
	// Очищаем карту на случай повторного вызова
	CharacterSkills.Empty();

	// Получаем доступ к метаданным нашего перечисления через рефлексию UE
	const UEnum* SkillEnum = StaticEnum<ECharacterSkill>();
	if (!SkillEnum)
		return;

	// Получаем максимальное численное значение в перечислении
	int32 MaxSkillsCount = SkillEnum->GetMaxEnumValue();

	// Циклом проходим по всем элементам перечисления
	for (int32 i = 0; i <= MaxSkillsCount; ++i)
	{
		// Проверяем, что этот индекс действительно существует в Enum (игнорируем служебные индексы UE)
		if (SkillEnum->IsValidEnumValue(i))
		{
			ECharacterSkill CurrentSkill = static_cast<ECharacterSkill>(i);

			// Задаем базовое значение 0.0f для каждого навыка
			CharacterSkills.Add(CurrentSkill, 0.0f);
		}
	}

	// Здесь можно вручную переписать стартовые навыки для конкретного класса персонажа
	// Например, если это Воин, то можно накинуть бонусов:
	// CharacterSkills[ECharacterSkill::SwordSkill] = 10.0f;
}

float UAttributeComponent::GetSkillValue(ECharacterSkill SkillType) const
{
	// Метод Find() ищет ключ в TMap и возвращает указатель на значение, либо nullptr, если ключа нет
	const float* FoundValue = CharacterSkills.Find(SkillType);

	// Если указатель существует (навык найден) — возвращаем то, что внутри него.
	// Если указатель равен nullptr — безопасно возвращаем 0.0f.
	return FoundValue ? *FoundValue : 0.0f;
}

FText UAttributeComponent::GetSkillDisplayName(ECharacterSkill SkillType) const
{
	const UEnum* EnumPtr = StaticEnum<ECharacterSkill>();
	if (!EnumPtr)
		return FText::GetEmpty();

	// Получаем индекс элемента в перечислении
	int32 EnumIndex = EnumPtr->GetIndexByValue(static_cast<int64>(SkillType));

	// Извлекаем DisplayName (то, что мы написали в UMETA)
	return EnumPtr->GetDisplayNameTextByIndex(EnumIndex);
}

FString UAttributeComponent::GetSkillCategory(ECharacterSkill SkillType) const
{
	const UEnum* EnumPtr = StaticEnum<ECharacterSkill>();
	if (!EnumPtr)
		return TEXT("");

	int32 EnumIndex = EnumPtr->GetIndexByValue(static_cast<int64>(SkillType));

	// Извлекаем значение метатега Category в виде строки
	return EnumPtr->GetMetaData(TEXT("Category"), EnumIndex);
}

bool UAttributeComponent::CheckSkill(ECharacterSkill Skill, float Difficulty) {
	return CharacterSkills.FindRef(Skill) >= Difficulty;
}

void UAttributeComponent::CommitSkillPoints(ECharacterSkill SkillType, int32 AddedPoints)
{
	if (AddedPoints <= 0)
		return;

	if (CharacterSkills.Contains(SkillType))
	{
		CharacterSkills[SkillType] += static_cast<float>(AddedPoints);
	}

	// Так как игрок обязан потратить ВСЁ, мы можем смело обнулять остаток очков компонента
	AvailableSkillPoints = 0;
}