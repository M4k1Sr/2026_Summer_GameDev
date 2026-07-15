#include <string>
#include <fstream>
#include "./StageManager.h"
#include "./Stage.h"
#include "./Stage2.h"
#include "../Charactor/Player.h"
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"

StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}

void StageManager::Init(void)
{
	// オブジェクトデータ読み込み
	LoadCsvData();

	// ステージ1面
	StageChange(0);
}

void StageManager::Update(void)
{
	int nextStageId = -1;

	// 全ステージの更新
	for (auto& stage : stages_)
	{
		stage->Update();

		// もしステージがクリア状態になったら、次のステージIDを記録
		if (player_->IsClear())
		{
			nextStageId = stage->GetId() + 1;
		}
	}

	// ループの中でReleaseすると配列が壊れてクラッシュするため、ループの外で実行する
	if (nextStageId != -1)
	{
		StageChange(nextStageId);
	}
}

void StageManager::Draw(void)
{

	// 描画
	for (auto& stage : stages_)
	{
		stage->Draw();
	}

}

void StageManager::Release(void)
{
	// 解放
	for (auto& stage : stages_)
	{
		stage->Release();
		delete stage;
	}
	stages_.clear();

}

void StageManager::AddHitCollider(const ColliderBase* hitCollider)
{
	for (auto& stage : stages_)
	{
		stage->AddHitCollider(hitCollider);
	}
}

void StageManager::LoadCsvData(void)
{
	// ファイルの読込
	std::ifstream ifs = std::ifstream(Application::PATH_CSV + "StageData.csv");

	if (!ifs)
	{
		// エラーが発生
		return;
	}

	std::string line;
	std::vector<std::string> strSplit;
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

		StageBase::StageData data = StageBase::StageData();
		int idx = 0;

		// IDと種別をパース
		data.id = stoi(strSplit[idx++]);
		data.type = static_cast<StageBase::TYPE>(stoi(strSplit[idx++]));

		// 解析したデータをIDをキーにして倉庫（map）にしまっておく
		stageMasterMap_[data.id] = data;
	}

	ifs.close();
}

StageBase* StageManager::Create(const StageBase::StageData& data)
{
	StageBase* stage = nullptr;

	switch (data.type)
	{
	case StageBase::TYPE::FIRST:
		stage = new Stage(data);
		break;
	case StageBase::TYPE::SECOND:
		stage = new Stage2(data);
		break;

		// 増える毎に追加
	}

	if (stage != nullptr)
	{
		stage->SetStageManager(this);
		stage->Init();
		stages_.emplace_back(stage);
	}

	return stage;
}

void StageManager::StageChange(int nextStageId)
{
	// 今画面で動いている古いステージの実体をすべて破棄してメモリを空ける
	Release();

	// 倉庫（map）から、次に動かしたいステージの「データ（設計図）」を探す
	auto it = stageMasterMap_.find(nextStageId);
	if (it != stageMasterMap_.end())
	{
		// 見つかったら、素晴らしい場所に定義されている Create をここで叩く！
		Create(it->second);
	}
	else
	{
		// 倉庫に次のIDがない ＝ 全ステージをクリアした（ゲームクリア画面へ遷移など）の処理をここに書く
	}
}


