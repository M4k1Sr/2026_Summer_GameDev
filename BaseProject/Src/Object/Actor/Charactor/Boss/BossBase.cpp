#include "../../../../Utility/AsoUtility.h"
#include "BossBase.h"

BossBase::BossBase(const BossBase::BossData& data)
	:
	CharactorBase(),
	type_(data.type),
	stateBase_(0)
{
	// ‰ŠúÀ•W‚Ìİ’è
	transform_.pos = data.defaultPos;
}

BossBase::~BossBase(void)
{
}

void BossBase::ChangeState(int state)
{

	stateBase_ = state;

	// Šeó‘Ô‘JˆÚ‚Ì‰Šúˆ—
	stateChanges_[stateBase_]();

}