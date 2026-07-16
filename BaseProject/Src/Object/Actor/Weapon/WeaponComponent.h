#pragma once
#include <memory>
#include "WeaponData.h"
class WeaponComponent
{
public:

	virtual ~WeaponComponent() = default;

	// 武器
	// ロード
	virtual void Load(void) = 0;
	// 更新
	virtual void Update(void) = 0;
	// 描画
	virtual void Draw(void) = 0;
	// 解放
	virtual void Release(void) = 0;

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