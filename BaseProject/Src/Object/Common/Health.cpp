#include "Health.h"

void Health::Init(int maxHp)
{
	maxHp_ = maxHp;
	nowHp_ = maxHp;	// ƒtƒ‹HP‚É‚·‚é
}

int Health::GetHp() const
{
	return nowHp_;
}

void Health::TakeDamage(int toDamage)
{
	// HP‚ª–³‚©‚Á‚½‚ç‰½‚à‚µ‚È‚¢
	if (maxHp_ < 0) {
		return;
	}
	
	// ƒ_ƒ[ƒW”»’è
	nowHp_ -= toDamage;

	if (nowHp_ == 0) {
		nowHp_ = 0;	// ƒ[ƒ‚É‚È‚Á‚½‚ç‚¸‚Á‚Æƒ[ƒó‘Ô
	}
}
