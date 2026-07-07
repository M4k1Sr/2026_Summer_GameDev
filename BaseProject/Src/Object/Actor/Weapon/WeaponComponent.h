#pragma once
#include <memory>
class WeaponComponent
{
public:

	virtual ~WeaponComponent() = default;

	// 親コンポーネントの設定と取得
	void SetParent(WeaponComponent* parent) { this->parent_ = parent; }
	WeaponComponent* GetParent() const { return this->parent_; }

	// 子コンポーネント情報の促しと削除
	virtual void Add(std::unique_ptr<WeaponComponent> component) {}
	virtual void Remove(WeaponComponent* component) {}

	// 武器コンポーネントが複合かを判定
	virtual bool IsWeaponComposite() const { return false; }

protected:

	WeaponComponent* parent_ = nullptr;

};