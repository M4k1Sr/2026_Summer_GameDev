#pragma once
#include "./WeaponComponent.h"
#include "../../Common/Transform.h"

// 武器のタイプ列挙型
enum class WeaponType
{
	NONE,

	// 片手剣タイプ
	ONE_HAND,

	// 両手剣タイプ
	TWO_HAND,

	// 槍タイプ
	SPEAR,

	MAX
	
};

// 武器列挙型
enum class WeaponKind
{
	None,

	// 片手
	Club,
	Sword,

	// 両手
	GreatSword,
	Axe,

	// 槍
	Lance,

	Max
};

// 武器データ構造体
struct WeaponData {
	WeaponType type;
	WeaponKind kind;
	float damage;
	float knockback;
};


class HitBox : public WeaponComponent
{
	// 固有のヒットボックス情報を持つクラス

public:

	// コンストラクタ
	HitBox(WeaponData data);
	// デストラクタ
	~HitBox() = default;

protected:

private:

	// ヒットボックスのダメージ
	float damage_;
	// ヒットボックスのノックバック
	float knockback_;

	// 攻撃判定
	bool isAttackAct_;	// 攻撃したか
	bool isHit_;			// ヒットしたか

	bool isAct_;			// ヒットボックスが出ているか
	float actTimer_;		// ヒットボックスが出ている時間

};

