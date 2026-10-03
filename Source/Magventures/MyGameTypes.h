// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h" // Нужно для корректной работы UENUM в некоторых версиях
#include "MyGameTypes.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	NotWeapon UMETA(DisplayName = "NotWeapon"),
	Hand UMETA(DisplayName = "Hand"),
	Sword UMETA(DisplayName = "Sword"),
	Axe UMETA(DisplayName = "Axe"),
	Mace UMETA(DisplayName = "Mace"),
	Spear UMETA(DisplayName = "Spear"),
	Dagger UMETA(DisplayName = "Dagger"),
	Staff UMETA(DisplayName = "Staff"),
	Bow UMETA(DisplayName = "Bow"),
	Crossbow UMETA(DisplayName = "Crossbow"),
	Throwing UMETA(DisplayName = "Throwning")
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	NotArmor UMETA(DisplayName = "NotArmor"),
	Mail UMETA(DisplayName = "Mail"),
	Helm UMETA(DisplayName = "Helm"),
	Gauntlets UMETA(DisplayName = "Gauntlets"),
	Boots UMETA(DisplayName = "Boots"),
	Shield UMETA(DisplayName = "Shield"),
};

UENUM(BlueprintType)
enum class EAmmunitionType : uint8
{
	NotAmmunition UMETA(DisplayName = "NotAmmunition"),
	Arrow UMETA(DisplayName = "Arrow"),
	Bolt UMETA(DisplayName = "Bolt"),
};

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	None UMETA(DisplayName = "None"),
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Potion UMETA(DisplayName = "Potion"),
	Consumable UMETA(DisplayName = "Consumable"), // Cвитки и т.п.
	Ammunition UMETA(DisplayName = "Ammunition"),
	Quest UMETA(DisplayName = "Quest Item"),
	Misc UMETA(DisplayName = "Miscellaneous")
};

UENUM(BlueprintType)
enum class EPotionType : uint8
{
	HealingPotion UMETA(DisplayName = "Healing Potion"),
	StaminaPotion UMETA(DisplayName = "Stamina Potion"),
	ManaPotion UMETA(DisplayName = "Mana Potion"),
};

UENUM(BlueprintType)
enum class ESpellTargetType : uint8
{
	SingleAlly UMETA(DisplayName = "Один союзник"),
	AllAllies UMETA(DisplayName = "Весь отряд"),
	SingleEnemy UMETA(DisplayName = "Один враг"),
	EnemyGroup UMETA(DisplayName = "Группа врагов"),
	AllEnemies UMETA(DisplayName = "Все враги"),
	ConeSector UMETA(DisplayName = "Сектор перед собой"), // Бьет веером на ближнюю дистанцию
	RadialSplash UMETA(DisplayName = "Радиальная зона") // Выбирается точка вдали, бьет по радиусу вокруг нее
};

UENUM(BlueprintType)
enum class ECharacterSkill : uint8
{
	CloseCombatSkill UMETA(DisplayName = "Ближний бой", Category = "General"),
	RangedCombatSkill UMETA(DisplayName = "Дальний бой", Category = "General"),
	DualWeaponsSkill UMETA(DisplayName = "Двойное оружие", Category = "General"),
	ArtifactsKnowledge UMETA(DisplayName = "Знание артефактов", Category = "General"),

	SwordSkill UMETA(DisplayName = "Меч", Category = "Weapon"),
	DaggerSkill UMETA(DisplayName = "Ножи и кинжалы", Category = "Weapon"),
	BluntSkill UMETA(DisplayName = "Дробящее оружие", Category = "Weapon"),
	AxeSkill UMETA(DisplayName = "Топоры", Category = "Weapon"),
	PolearmsSkill UMETA(DisplayName = "Древковое", Category = "Weapon"),
	StaffSkill UMETA(DisplayName = "Посохи", Category = "Weapon"),
	ShieldSkill UMETA(DisplayName = "Щит", Category = "Weapon"),
	BowSkill UMETA(DisplayName = "Лук", Category = "Weapon"),
	CrossbowSkill UMETA(DisplayName = "Арбалет", Category = "Weapon"),
	ThrowingSkill UMETA(DisplayName = "Метательное", Category = "Weapon"),
	UnarmedSkill UMETA(DisplayName = "Бой без оружия", Category = "Weapon"),

	ControlOverPower UMETA(DisplayName = "Контроль силы", Category = "Magic"),
	FireMagicSkill UMETA(DisplayName = "Магия огня", Category = "Magic"),
	WaterMagicSkill UMETA(DisplayName = "Магия воды", Category = "Magic"),
	AirMagicSkill UMETA(DisplayName = "Магия воздуха", Category = "Magic"),
	EarthMagicSkill UMETA(DisplayName = "Магия земли", Category = "Magic"),
	LifeMagicSkill UMETA(DisplayName = "Магия жизни", Category = "Magic"),
	NatureMagicSkill UMETA(DisplayName = "Магия природы", Category = "Magic"),
	MindMagicSkill UMETA(DisplayName = "Магия разума", Category = "Magic"),
	DeathMagicSkill UMETA(DisplayName = "Магия смерти", Category = "Magic"),

	LocksAndTrapsSkill UMETA(DisplayName = "Замки и ловушки", Category = "Special")
};

UENUM(BlueprintType)
enum class ESpellProjectileType : uint8
{
	None UMETA(DisplayName = "Без снаряда (Мгновенно)"),
	RedBeam UMETA(DisplayName = "Красный луч"),
	YellowBeam UMETA(DisplayName = "Жёлтый луч"),
	BlueBeam UMETA(DisplayName = "Синий луч"),
	Fireball UMETA(DisplayName = "Огненный шар"),
	IceArrow UMETA(DisplayName = "Ледяная стрела"),
	Lightning UMETA(DisplayName = "Молния"),
	PoisonCloud UMETA(DisplayName = "Облако яда")
	// Сюда вы сможете дописывать любые новые типы визуальных снарядов
};

UENUM(BlueprintType)
enum class EAllySpellEffectType : uint8
{
	None UMETA(DisplayName = "Нет"),
	Healing UMETA(DisplayName = "Лечение"),
	Rest UMETA(DisplayName = "Отдых"),
	CureNegative UMETA(DisplayName = "Снятие негативных эффектов"),
	Buff UMETA(DisplayName = "Позитивный эффект"),
	ManaRegen UMETA(DisplayName = "Восстановление маны"),
	Resurrection UMETA(DisplayName = "Воскрешение")
};

UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	Poison UMETA(DisplayName = "Яд"),
	Stun UMETA(DisplayName = "Оглушение"),
	Blind UMETA(DisplayName = "Слепота"),
	Bleed UMETA(DisplayName = "Кровотечение"),
	Regeneration UMETA(DisplayName = "Регенерация"),
	Malaise UMETA(DisplayName = "Недомогание"),
	Irritation UMETA(DisplayName = "Раздражение"),
	Paralysis UMETA(DisplayName = "Паралич"),
	Slowdown UMETA(DisplayName = "Замедление")
};

USTRUCT(BlueprintType)
struct FActiveStatusEffect
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EStatusEffectType EffectType;

	UPROPERTY(BlueprintReadWrite)
	int32 RemainingDuration; // Оставшиеся раунды

	UPROPERTY(BlueprintReadWrite)
	int32 Power; // Сила эффекта (например, сколько ХП отнимает за тик)

	UPROPERTY(BlueprintReadWrite)
	AP_Character* Caster; // Кто наложил эффект (важно для расчета опыта или логов)
};

USTRUCT(BlueprintType)
struct FSpellStatusEffectChance
{
	GENERATED_BODY()

	/** Какой статус-эффект пытаемся наложить */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	EStatusEffectType EffectType;

	/** Базовый шанс наложения в процентах (от 0 до 100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float ApplicationChance;

	/** Базовая длительность эффекта в раундах */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect", meta = (ClampMin = "1"))
	int32 BaseDuration;

	/** Базовая мощность эффекта (урон за тик или сила дебаффа) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	int32 BasePower;
};