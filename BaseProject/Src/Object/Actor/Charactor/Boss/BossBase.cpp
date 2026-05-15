#include "../../../../Utility/AsoUtility.h"
#include "../../../../Utility/MatrixUtility.h"
#include "../Player.h"
#include "BossBase.h"

BossBase::BossBase(const BossBase::BossData& data)
	:
	CharactorBase(),
	type_(data.type),
	stateBase_(0),
	player_()
{
	// ‰ŠúÀ•W‚Ìİ’è
	transform_.pos = data.defaultPos;
}

BossBase::~BossBase(void)
{
}

void BossBase::SetPlayer(Player* player)
{
	player_ = player;
}

void BossBase::ChangeState(int state)
{

	stateBase_ = state;

	// Šeó‘Ô‘JˆÚ‚Ì‰Šúˆ—
	stateChanges_[stateBase_]();

}


