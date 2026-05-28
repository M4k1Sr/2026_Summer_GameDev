#include <string>
#include <fstream>
#include "AttackManager.h"
#include "./AttackBase.h"
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Collider/ColliderBase.h"

AttackManager::AttackManager(ObjectManager* objMng)
	: objMng_(objMng)
{
}

AttackManager::~AttackManager()
{
}

void AttackManager::Init(void)
{
	// オブジェクトデータ読み込み
	LoadCsvData();
}

void AttackManager::Update(void)
{
	// 更新	
	for (auto& attack : attacks_)
	{
		attack->Update();
	}

	// 死亡フラグが立っているオブジェクトのクリーンアップ処理
	auto it = std::remove_if(attacks_.begin(), attacks_.end(), [](AttackBase* attack) {
		if (attack->IsDead())
		{
			attack->Release(); // 解放処理を呼ぶ
			delete attack;     // メモリから完全に消去
			return true;       // リストから取り除く対象にする
		}
		return false;
		});

	// 実際に vector の配列サイズを縮める
	attacks_.erase(it, attacks_.end());
}

void AttackManager::Draw(void)
{
	// 描画
	for (auto& attack : attacks_)
	{
		attack->Draw();
	}
}

void AttackManager::Release(void)
{
	// 解放
	for (auto& attack : attacks_)
	{
		attack->Release();
		delete attack;
	}
	attacks_.clear();
}

void AttackManager::Create(const AttackBase::AttackParam& data,const VECTOR& startPos, const VECTOR& dir)
{
	// データの取得
	auto attack = new AttackBase(data, startPos, dir);

	attack->SetObjectManager(objMng_);

	attacks_.push_back(attack);
}

void AttackManager::AddHitCollider(const ColliderBase* hitCollider)
{
	for (auto& attack : attacks_)
	{
		attack->AddHitCollider(hitCollider);
	}
}

const AttackBase::AttackParam& AttackManager::GetMasterData(AttackBase::TYPE type) const
{
	return masterMap_.at(type);
}

void AttackManager::LoadCsvData(void)
{
	// ファイルの読込
	std::ifstream ifs = std::ifstream(Application::PATH_CSV + "AttackData.csv");

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

		AttackBase::AttackParam param;

		// 構造体に合わせて読込データを格納
		AttackBase::AttackParam data = AttackBase::AttackParam();

		int idx = 0;

		// ID
		data.id = stoi(strSplit[idx++]);

		// 種別
		data.type = static_cast<AttackBase::TYPE>(stoi(strSplit[idx++]));

		// 移動速度
		data.speed = stof(strSplit[idx++]);

		// 最大移動距離(消滅しない距離)
		data.maxDistance = stof(strSplit[idx++]);

		// 描画倍率
		data.scale = stof(strSplit[idx++]);		


		// オブジェクト保存
		masterMap_[data.type] = data;

	}

	ifs.close();
}

