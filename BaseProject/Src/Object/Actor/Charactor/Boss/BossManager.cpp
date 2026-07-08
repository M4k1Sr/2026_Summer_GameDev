#include <string>
#include <fstream>
#include "../../../../Application.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../Charactor/Player.h"
#include "./BossBase.h"
#include "./BossPixie.h"
#include "./BossGoblin.h"
#include "./BossManager.h"
#include "../../../../Manager/SoundManager.h"
#include "../../../../Manager/ServiceLocator.h"

BossManager::BossManager(void)
{
}

BossManager::~BossManager(void)
{
}

void BossManager::Init(void)
{
	ServiceLocator::GetSound().LoadBank(BANK_ID::BOSS);

	// オブジェクトデータ読み込み
	LoadCsvData();

}

void BossManager::Update(void)
{
	// 更新
	for (auto& boss : bosses_)
	{
		boss->Update();
	}
}

void BossManager::Draw(void)
{
	// 描画
	for (auto& boss : bosses_)
	{
		boss->Draw();
	}

}

void BossManager::Release(void)
{
	// 解放
	for (auto& boss : bosses_)
	{
		boss->Release();
		delete boss;
	}
	bosses_.clear();

	// ボスサウンド削除
	ServiceLocator::GetSound().UnloadBank(BANK_ID::BOSS);

}

void BossManager::SetPlayer(Player* player)
{
	player_ = player;
}

void BossManager::AddHitCollider(const ColliderBase* hitCollider)
{
	for (auto& boss : bosses_)
	{
		boss->AddHitCollider(hitCollider);
	}
}

void BossManager::LoadCsvData(void)
{
	// ファイルの読込
	std::ifstream ifs = std::ifstream(Application::PATH_CSV + "BossData.csv");

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

		BossBase* boss = nullptr;

		// 構造体に合わせて読込データを格納
		BossBase::BossData data = BossBase::BossData();

		int idx = 0;

		// ID
		data.id = stoi(strSplit[idx++]);

		// 種別
		data.type = static_cast<BossBase::BOSS_TYPE>(stoi(strSplit[idx++]));

		// 初期座標
		data.defaultPos =
		{
		stof(strSplit[idx++]),
		stof(strSplit[idx++]),
		stof(strSplit[idx++])
		};

		// オブジェクト生成
		Create(data);

	}

	ifs.close();
}

BossBase* BossManager::Create(const BossBase::BossData& data)
{
	BossBase* boss = nullptr;

	switch (data.type)
	{
	case BossBase::BOSS_TYPE::PIXIE:
		boss = new BossPixie(data);
		break;
	case BossBase::BOSS_TYPE::GOBLIN:
		boss = new BossGoblin(data);
		break;

		// 増える毎に追加
	}

	if (boss != nullptr)
	{

		if (player_ != nullptr)
		{
			boss->SetPlayer(player_);
		}

		boss->Init();
		bosses_.emplace_back(boss);
	}

	return boss;
}


