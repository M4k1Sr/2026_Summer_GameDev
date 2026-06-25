#include "ObjectConveyer.h"
#include "../../../../Object/Collider/ColliderLine.h"
#include "../../../../Object/Collider/ColliderModel.h"
#include "../Object/ObjectBase.h"

ObjectConveyer::ObjectConveyer(const ObjectBase::ObjectData& data) : 
	ObjectBase(data)
{
}

ObjectConveyer::~ObjectConveyer(void)
{
}

void ObjectConveyer::Draw(void)
{
}

void ObjectConveyer::InitLoad(void)
{
}

void ObjectConveyer::InitTransform(void)
{
}

void ObjectConveyer::InitCollider(void)
{
	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::TILE, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// モデルとの衝突で使用するモデルコライダー
	ColliderModel* colModel = new ColliderModel(
		ColliderBase::TAG::TILE,
		&transform_);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);

}

void ObjectConveyer::InitAnimation(void)
{
}

void ObjectConveyer::InitPost(void)
{
}

void ObjectConveyer::UpdateProcess(void)
{
}

void ObjectConveyer::UpdateProcessPost(void)
{
}

void ObjectConveyer::UpdateProcessMove(void)
{
}
