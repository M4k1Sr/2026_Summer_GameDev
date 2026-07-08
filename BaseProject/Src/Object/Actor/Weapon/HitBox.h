#pragma once
#include "./WeaponComponent.h"
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
class ResourceManager;

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
	WeaponType type = WeaponType::NONE;	// 武器タイプ
	WeaponKind kind = WeaponKind::None;	// 武器種別
	float damage = 0.0f;				// 武器のダメージ量
	float criticalRate = 0.05f;			// 武器のクリティカル率(5%でクリティカル)
	float criticalBonus = 1.5f;			// クリティカルダメージ倍率
	VECTOR pos = AsoUtility::VECTOR_ZERO;	// 武器の座標
	VECTOR rot = AsoUtility::VECTOR_ZERO;	// 武器の回転
	VECTOR scl = AsoUtility::VECTOR_ONE;	// 武器のスケール
	// 武器データをゲーム別に追加
};


class HitBox : public WeaponComponent
{
	// 固有のヒットボックス情報を持つクラス

public:

	// コンストラクタ
	HitBox(const WeaponData& data);
	// デストラクタ
	~HitBox() = default;

	// リソースロード
	void Load(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	// ダメージの取得
	float GetDamage() const { return data_.damage; }

	// 攻撃判定の取得
	bool IsAttackAct() const { return isAttackAct_; }
	bool IsHit() const { return isHit_; }

	// ヒットボックスがアクティブか
	bool IsAct() const { return isAct_; }

	// ヒットボックスが出ている時間を取得
	float GetActTimer() const { return actTimer_; }

	// セッター
	void SetDamage(float damage) { data_.damage = damage; }
	void SetAttackAct(bool isAttackAct) { isAttackAct_ = isAttackAct; }
	void SetHit(bool isHit) { isHit_ = isHit; }
	void SetAct(bool isAct) { isAct_ = isAct; }
	void SetActTimer(float timer) { actTimer_ = timer; }



private:

	// シングルトン参照
	ResourceManager& resMng_;

	// モデル制御の基本情報
	Transform transform_;

	// 武器が所持しているデータ
	WeaponData data_;

	// 攻撃判定
	bool isAttackAct_;	// 攻撃したか
	bool isHit_;			// ヒットしたか

	bool isAct_;			// ヒットボックスが出ているか
	float actTimer_;		// ヒットボックスが出ている時間

};

