#pragma once
#include <vector>
#include "./WeaponComponent.h"
class WeaponComposite : public WeaponComponent
{
public:

	// デストラクタ
	virtual ~WeaponComposite() override = default;

	// 子コンポーネント情報の促しと削除
	void Add(std::unique_ptr<WeaponComponent> component) override;
	void Remove(WeaponComponent* component) override;

	// 武器コンポーネントが複合かを判定
	bool IsWeaponComposite() const override { return true; }

protected:

	std::vector<std::unique_ptr<WeaponComponent>> children_;

private:




};

