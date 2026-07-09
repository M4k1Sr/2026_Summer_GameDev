#include "WeaponComposite.h"
#include <algorithm>

void WeaponComposite::Load(void)
{
	for (const auto& child : children_) {
		if (child) child->Load();
	}
}

void WeaponComposite::Update(void)
{
	for (const auto& child : children_) {
		if (child) child->Update();
	}
}

void WeaponComposite::Draw(void)
{
	for (const auto& child : children_) {
		if (child) child->Draw();
	}
}

void WeaponComposite::Release(void)
{
	for (const auto& child : children_) {
		if (child) child->Release();
	}

	children_.clear();
}

void WeaponComposite::Add(std::unique_ptr<WeaponComponent> component)
{
	if (component) {
		component->SetParent(this);
		children_.push_back(std::move(component));
	}
}

void WeaponComposite::Remove(WeaponComponent* component)
{
	// vectorから指定されたコンポーネントの削除
	auto it = std::remove_if(children_.begin(), children_.end(),
		[component](const std::unique_ptr<WeaponComponent>& child) {
			return child.get() == component;
		});

	// 見つかった場合コンポーネントの削除
	if (it != children_.end()) {
		(*it)->SetParent(nullptr);
		children_.erase(it, children_.end());
	}
}
