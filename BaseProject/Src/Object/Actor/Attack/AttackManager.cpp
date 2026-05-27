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

void AttackManager::AddHitCollider(const ColliderBase* hitCollider)
{
	for (auto& attack : attacks_)
	{
		attack->AddHitCollider(hitCollider);
	}
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

}

void AttackManager::SpawnFireBall(const VECTOR& startPos, const VECTOR& dir)
{
}
