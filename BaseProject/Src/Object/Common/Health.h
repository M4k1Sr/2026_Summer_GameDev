#pragma once

class Health
{
public:

	// Init
	void Init(int maxHp);

	// ゲッター
	int GetHp() const;

	// ダメージを受ける
	void TakeDamage(int toDamage);

private:

	// 最大HP
	int maxHp_;

	// 現在HP
	int nowHp_;

};

