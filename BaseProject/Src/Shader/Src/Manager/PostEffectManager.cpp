#include "PostEffectManager.h"
#include "../BlurShader.h"
#include "../MonoShader.h"
#include "../MosaicShader.h"
#include "../ScanLineShader.h"
#include "../../../Manager/SceneManager.h"
#include "../PostEffectBase.h"
#include <algorithm>
#include "../../../Common/Vector2.h"
#include "../../../Application.h"

PostEffectManager::PostEffectManager(void)
{
	currentScreen_ = -1;


}

PostEffectManager::~PostEffectManager(void)
{
}

void PostEffectManager::Init(void)
{

	Vector2 pos = Vector2(0, 0);
	Vector2 size = Vector2(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x + size.x);
	float eY = static_cast<float>(pos.y + size.y);

	// ÇSí∏ì_ÇÃèâä˙âª
	for (int i = 0; i < 4; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// ç∂è„
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// âEè„
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// âEâ∫
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	//ç∂â∫
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	/*
Å@Å`Å`Å`Å`Å`Å`
	0-----1
	|     |
	|     |
	3-----2
Å@Å`Å`Å`Å`Å`Å`
	0-----1
	|  Å^
	|Å^
	3
Å@Å`Å`Å`Å`Å`Å`
		  1
	   Å^ |
	 Å^   |
	3-----2
Å@Å`Å`Å`Å`Å`Å`
*/

//í∏ì_ÉCÉìÉfÉbÉNÉX
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;
	indexes_[cnt++] = 3;

	// É|ÉXÉgÉGÉtÉFÉNÉgÇÃí«â¡
	//effects.push_back(std::make_unique<MonoShader>());
	effects.push_back(std::make_unique<BlurShader>());
	//effects.push_back(std::make_unique<MosaicShader>());
	effects.push_back(std::make_unique<ScanLineShader>());

	//èáî‘í ÇËÇ…ï¿Ç◊ÇÈ
	std::sort(effects.begin(), effects.end(),
		[](const std::unique_ptr<PostEffectBase>& a, const std::unique_ptr<PostEffectBase>& b)
		{return a->GetShader().order < b->GetShader().order; });
}

void PostEffectManager::Load(void)
{
	for (auto& effect : effects)
	{
		effect->Load();
	}
}

void PostEffectManager::Update(void)
{
	//mainScreen = SceneManager::GetInstance().GetMainScreen();
}

void PostEffectManager::Draw(int mainScreen)
{
	currentScreen_ = mainScreen;
	for (auto& effect : effects)
	{
		effect->Draw(currentScreen_, vertexs_,indexes_);

		//ÇPÇ¬ëOÇÃÉXÉNÉäÅ[ÉìÇÇ‡ÇÁÇ§
		currentScreen_ = effect->GetShader().postEffectScreen_;
	}

	// ÉÅÉCÉìÇ…ñﬂÇ∑
	SetDrawScreen(mainScreen);

	DrawGraph(0, 0, currentScreen_, false);
}

void PostEffectManager::Release(void)
{
	for (auto& effect : effects)
	{
		effect->Release();
	}
}
