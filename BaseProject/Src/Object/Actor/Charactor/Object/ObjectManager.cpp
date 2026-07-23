#include <string>
#include <fstream>
#include "../../../../Application.h"
#include "../../../../Utility/AsoUtility.h"
#include "./ObjectBase.h"
#include "./ObjectBox.h"
#include "./ObjectTile.h"
#include "./ObjectBossGimmick.h"
#include "./ObjectTarai.h"
#include "./NdlFloor.h"
#include "./ObjectBurner.h"
#include "./ObjectConveyer.h"
#include "./ObjectCage.h"
//#include "./ObjectBossCage.h"
#include "./ObjectArray.h"
#include "./ObjectManager.h"

ObjectManager::ObjectManager(void)
{
}

ObjectManager::~ObjectManager(void)
{
}

void ObjectManager::Init(void)
{
	// オブジェクトデータ読み込み
	LoadCsvData();

}

void ObjectManager::Update(void)
{
	// 更新
	for (auto& object : objects_)
	{
		// ステージ一致により更新
		if (object->GetStageType() == 0 || object->GetStageType() == currentStageType_)
		{
			object->Update();
		}
	}
}

void ObjectManager::Draw(void)
{
	// 描画
	for (auto& object : objects_)
	{
		// ステージ一致により描画
		if (object->GetStageType() == 0 || object->GetStageType() == currentStageType_)
		{
			object->Draw();
		}
	}

}

void ObjectManager::Release(void)
{
	// 解放
	for (auto& object : objects_)
	{
		object->Release();
		delete object;
	}
	objects_.clear();

}

void ObjectManager::AddHitCollider(const ColliderBase* hitCollider)
{
	for (auto& object : objects_)
	{
		object->AddHitCollider(hitCollider);
	}
}

void ObjectManager::LoadCsvData(void)
{
	// ファイルの読込
	std::ifstream ifs = std::ifstream(Application::PATH_CSV + "ObjectData.csv");

	if (!ifs)
	{
		// エラーが発生
		return;
	}

	// ファイルを１行ずつ読み込む
	std::string line;// 1行の文字情報

	std::vector<std::string> strSplit; // 1行を1文字の動的配列に分割

	bool isHeader = true;

	while (getline(ifs, line))
	{
		if (isHeader)
		{
			isHeader = false;
			continue;
		}

		// １行をカンマ区切りで分割
		strSplit = AsoUtility::Split(line, ',');

		ObjectBase* enemy = nullptr;

		// 構造体に合わせて読込データを格納
		ObjectBase::ObjectData data = ObjectBase::ObjectData();

		int idx = 0;

		// ID
		data.id = stoi(strSplit[idx++]);

		// 種別
 		data.type = static_cast<ObjectBase::TYPE>(stoi(strSplit[idx++]));

		// 初期座標
		data.defaultPos =
		{
		stof(strSplit[idx++]),
		stof(strSplit[idx++]),
		stof(strSplit[idx++])
		};
		
		// 移動タイプ
		data.moveType = stoi(strSplit[idx++]);

		// ステージ別で動かすオブジェクトを設定
		data.stageType = stoi(strSplit[idx++]);

		// オブジェクト生成
		Create(data);

	}

	ifs.close();
}

ObjectBase* ObjectManager::Create(const ObjectBase::ObjectData& data)
{
	ObjectBase* object = nullptr;

	switch (data.type)
	{
	case ObjectBase::TYPE::BOX:
		//object = new ObjectBox(data);
		break;
	case ObjectBase::TYPE::TILE:
		object = new ObjectTile(data);
		break;
	case ObjectBase::TYPE::BOSS_GIMMICK:
		object = new ObjectBossGimmick(data);
		break;
	case ObjectBase::TYPE::TARAI:
		object = new ObjectTarai(data);
		break;
	case ObjectBase::TYPE::NEEDLE_FLOOR:
		object = new NdlFloor(data);
		break;
	case ObjectBase::TYPE::BURNER:
		//object = new Burner(data);
		break;
	case ObjectBase::TYPE::CONVEYER:
		//object = new ObjectConveyer(data);
		break;
	case ObjectBase::TYPE::BREAK_CAGE:
		object = new ObjectCage(data);
		break;
	case ObjectBase::TYPE::BOSS_CAGE:
		//object = new ObjectBossCage(data);
		break;
		// 増える毎に追加
	}

	if (object != nullptr)
	{
		object->SetObjectManager(this); 
		object->Init();
		objects_.emplace_back(object);
	}

	return object;
}

ObjectTile* ObjectManager::GetTileAt(const VECTOR& pos)
{

	for (auto& object : objects_)
	{
		// ステージ番号と一致しないものはスキップ
		if (object->GetStageType() != 0 && object->GetStageType() != currentStageType_)
		{
			continue;
		}

		if (auto tile = dynamic_cast<ObjectTile*>(object))
		{
			VECTOR tilePos = tile->GetPos();

			// XZ平面のみで距離計算
			float dx = tilePos.x - pos.x;
			float dz = tilePos.z - pos.z;
			float distXZ = sqrtf(dx * dx + dz * dz);

			// XZの範囲内ならOKとする（高さYは無視）
			if (distXZ < 130.0f)
			{
				return tile;
			}
		}
	}

	return nullptr;
}

// ボスギミックスイッチ
ObjectBossGimmick* ObjectManager::GetBossGimmick(const VECTOR& pos)
{
	for (auto& object : objects_)
	{
		// ステージ番号と一致しないものはスキップ
		if (object->GetStageType() != 0 && object->GetStageType() != currentStageType_)
		{
			continue;
		}

		if (auto bossGimmick = dynamic_cast<ObjectBossGimmick*>(object))
		{
			VECTOR bossGimmickPos = bossGimmick->GetPos();

			// XZ平面のみで距離計算
			float dx = bossGimmickPos.x - pos.x;
			float dz = bossGimmickPos.z - pos.z;

			float distXZ = dx * dx + dz * dz;

			// XZの範囲内ならOKとする（高さYは無視）
			if (distXZ < 10000.0f)
			{
				return bossGimmick;
			}
		}
	}

	return nullptr;

}

// タライ
ObjectTarai* ObjectManager::GetTarai(const VECTOR& pos)
{
	for (auto& object : objects_)
	{
		// ステージ番号と一致しないものはスキップ
		if (object->GetStageType() != 0 && object->GetStageType() != currentStageType_)
		{
			continue;
		}

		if (auto tarai = dynamic_cast<ObjectTarai*>(object))
		{
			VECTOR taraiPos = tarai->GetPos();

			// XZ平面のみで距離計算
			float dx = taraiPos.x - pos.x;
			float dz = taraiPos.z - pos.z;

			float distXZ = dx * dx + dz * dz;

			// XZの範囲内ならOKとする（高さYは無視）
			if (distXZ < 100000000.0f)
			{
				return tarai;
			}
		}
	}

	return nullptr;

}

// ボス用の檻
//ObjectBossCage* ObjectManager::GetBossCage(const VECTOR& pos)
//{
//	for (auto& object : objects_)
//	{
//		// ステージ番号と一致しないものはスキップ
//		if (object->GetStageType() != 0 && object->GetStageType() != currentStageType_)
//		{
//			continue;
//		}
//
//		if (auto tarai = dynamic_cast<ObjectBossCage*>(object))
//		{
//			VECTOR taraiPos = tarai->GetPos();
//
//			// XZ平面のみで距離計算
//			float dx = taraiPos.x - pos.x;
//			float dz = taraiPos.z - pos.z;
//
//			float distXZ = dx * dx + dz * dz;
//
//			// XZの範囲内ならOKとする（高さYは無視）
//			if (distXZ < 100000000.0f)
//			{
//				return tarai;
//			}
//		}
//	}
//
//	return nullptr;
//
//}

NdlFloor* ObjectManager::GetNdl(const VECTOR& pos)
{
	for (auto& object : objects_)
	{
		// ステージ番号と一致しないものはスキップ
		if (object->GetStageType() != 0 && object->GetStageType() != currentStageType_)
		{
			continue;
		}

		if (auto ndl = dynamic_cast<NdlFloor*>(object))
		{
			ndl->GetStart();

			VECTOR ndlPos = ndl->GetPos();

			// XZ平面のみで距離計算
			float dx = ndlPos.x - pos.x;
			float dz = ndlPos.z - pos.z;
			float distXZ = sqrtf(dx * dx + dz * dz);

			// XZの範囲内ならOKとする（高さYは無視）
			if (distXZ < 130.0f)
			{
				return ndl;
			}
		}
	}

	return nullptr;
}

bool ObjectManager::IsTaraiFalling(void)
{
	for (auto& object : objects_)
	{
		// ステージ番号と一致しないものはスキップ
		if (object->GetStageType() != 0 && object->GetStageType() != currentStageType_)
		{
			continue;
		}

		if (auto tarai = dynamic_cast<ObjectTarai*>(object))
		{
			// ★ isGimmick_ だけでなく、タイマーが残っている間も true にする
			if (tarai->IsCameraFocusing())
			{
				return true;
			}
		}
	}
	return false;
}

//bool ObjectManager::IsCageFalling(void)
//{
//	for (auto& object : objects_)
//	{
//		// ステージ番号と一致しないものはスキップ
//		if (object->GetStageType() != 0 && object->GetStageType() != currentStageType_)
//		{
//			continue;
//		}
//
//		if (auto tarai = dynamic_cast<ObjectBossCage*>(object))
//		{
//			// ★ isGimmick_ だけでなく、タイマーが残っている間も true にする
//			if (tarai->IsCameraFocusing())
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}
