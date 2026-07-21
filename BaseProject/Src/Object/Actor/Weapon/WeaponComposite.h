#pragma once
#include <vector>
#include "./WeaponComponent.h"

class WeaponComposite : public WeaponComponent
{
public:

	// デストラクタ
	virtual ~WeaponComposite() override = default;

	// 共通関数
	void Load(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// 子コンポーネント情報の促しと削除
	void Add(std::unique_ptr<WeaponComponent> component) override;
	void Remove(WeaponComponent* component) override;

	// 武器コンポーネントが複合かを判定
	bool IsWeaponComposite() const override { return true; }

private:

	static constexpr float MAX_FALL_SPEED = -10.0f;

	std::vector<std::unique_ptr<WeaponComponent>> children_;

	// ジャンプ量
	VECTOR jumpPow_;

	void Gravity(void);


};

