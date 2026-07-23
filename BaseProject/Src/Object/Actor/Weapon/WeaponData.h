#pragma once
#include <dxlib.h> // VECTOR用

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
	NONE,

	// 片手
	CLUB,
	SWORD,

	// 両手
	GREAT_SWORD,
	AXE,

	// 槍
	LANCE,

	MAX
};

// アイテム列挙型
enum class ItemKind
{
	NONE,

	// ばくだん
	BOMB,

	// ビタロック
	STOP_TIMER,

	// マジックハンド
	MAGIC_HAND,

	MAX
};

// 武器データ構造体
struct WeaponData {

	// WeaponAssets用(主にプレイヤー、エネミ―用データ)
	WeaponType type = WeaponType::NONE;	// 武器タイプ
	WeaponKind weapon = WeaponKind::NONE;	// 武器種別
	ItemKind item = ItemKind::NONE;			// ItemAssets用
	float damage = 0.0f;				// 武器のダメージ量
	float criticalRate = 0.05f;			// 武器のクリティカル率(5%でクリティカル)
	float criticalBonus = 1.5f;			// クリティカルダメージ倍率
	VECTOR pos;	// 武器の座標
	VECTOR rot;	// 武器の回転
	VECTOR scl;	// 武器のスケール
	VECTOR dir;
	VECTOR localPos = { 0.0f, 0.0f, 0.0f };   // 握り位置の補正
	VECTOR localRot = { 0.0f, 0.0f, 0.0f };   // 向きの補正
	int modelId_ = -1;	// モデルID
	int ownerModelId = -1;     // 持ち主のモデルID
	int ownerFrameIndex = -1;  // 追従するフレーム番号
	float speed = 3.0f;		// 速度
	float dmgRange = 0.0f;	// Damage範囲


	// 武器データをゲーム別に追加
};
