#include "WeaponComposite.h"
#include <algorithm>

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
