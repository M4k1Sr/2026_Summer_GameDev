#include "WeaponComposite.h"
#include <algorithm>
#include "../../../Utility/AsoUtility.h"
#include "../../../Application.h"
#include "../../../Manager/SceneManager.h"

void WeaponComposite::Load(void)
{
	for (const auto& child : children_) {
		if (child) child->Load();
	}
}

void WeaponComposite::Update(void)
{
	for (const auto& child : children_) {
		Gravity();

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

void WeaponComposite::Gravity(void)
{
	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Application::GetInstance().GetGravityPow() * SceneManager::GetInstance().GetDeltaTime();

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

	jumpPow_ = VAdd(jumpPow_, gravity);

	// 重力速度の制限
	if (jumpPow_.y < MAX_FALL_SPEED)
	{
		jumpPow_.y = MAX_FALL_SPEED;
	}

}
