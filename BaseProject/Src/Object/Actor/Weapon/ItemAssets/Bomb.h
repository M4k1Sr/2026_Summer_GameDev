#pragma once
#include "../WeaponComponent.h"
#include "../../../Common/Transform.h"
#include "../../../../Utility/AsoUtility.h"
class ResourceManager;

class Bomb : public WeaponComponent
{
	// 固有のヒットボックス情報を持つクラス

public:

	// コンストラクタ
	Bomb(const WeaponData& data);
	// デストラクタ
	~Bomb() = default;

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

	// 投擲判定の取得
	bool IsThrowAct() const { return isThrow_; }
	bool IsHit() const { return isHit_; }

	// ヒットボックスがアクティブか
	bool IsAct() const { return isAct_; }

	// ヒットボックスが出ている時間を取得
	float GetActTimer() const { return actTimer_; }

	// セッター
	void SetDamage(float damage) { data_.damage = damage; }
	void SetThrowAct(bool isThrow) { isThrow_ = isThrow; }
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
	bool isThrow_;		// 投擲したか
	bool isHit_;			// ヒットしたか

	bool isAct_;			// ヒットボックスが出ているか
	float actTimer_;		// ヒットボックスが出ている時間

	void Throw(void);
};

