#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/InputManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "./Manager/ServiceLocator.h"
#include "./Manager/SoundManager.h"
#include "./Renderer/EffectRenderer/Manager/EffectManager.h"
#include "./Renderer/UIRenderer/Manager/UIManager.h"	
#include "Common/FpsController.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_CSV = "Data/Csv/";
const std::string Application::PATH_SOUND = "Data/Sound/";
const std::string Application::PATH_SHADER = "Data/Shader/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("まきプロ");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// FPS制御初期化
	fpsController_ = new FpsController(FRAME_RATE);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// Effekseerの初期化
	InitEffekseer();

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// リソース管理初期化
	ResourceManager::CreateInstance();

	//// サウンド管理初期化
	//SoundManager::CreateInstance();
	
	//エフェクト管理初期化
	//EffectManager::CreateInstance();

	// マネージャ生成
	soundMng_ = new SoundManager();
	soundMng_->Init();

	effectMng_ = new EffectManager();
	UIMng_ = new UIManager();

	// サービスロケータに登録
	ServiceLocator::Provide(soundMng_);
	ServiceLocator::Provide(effectMng_);
	ServiceLocator::Provide(UIMng_);

	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	InputManager& inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();
	//SoundManager& soundManager = SoundManager::GetInstance();
	/*EffectManager::CreateInstance();
	EffectManager::GetInstance().Init();*/

	// ゲームループ
	while (ProcessMessage() == 0 )
	{

		// 裏画面をクリア
		ClearDrawScreen();

		inputManager.Update();
		sceneManager.Update();
		ServiceLocator::GetEffect().Play();
		ServiceLocator::GetUI().Update();

		sceneManager.Draw();
<<<<<<< HEAD
		//ServiceLocator::GetUI().Draw();
=======
		//ServiceLocator::GetUi().Draw();
>>>>>>> m4k
		/*EffectManager::GetInstance().Update();
		EffectManager::GetInstance().Draw();*/
#ifdef _DEBUG

		// 平均FPS描画
		fpsController_->Draw();

#endif // _DEBUG

		ScreenFlip();

		// 理想FPS経過待ち
		fpsController_->Wait();
	}

}

void Application::Destroy(void)
{
	// FPS制御メモリ解放
	delete fpsController_;

	// シーン管理解放
	SceneManager::GetInstance().Destroy();

	//SoundManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	//EffectManager::GetInstance().Destroy();

	delete soundMng_;
	delete effectMng_;
	delete UIMng_;

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	// インスタンスのメモリ解放
	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
	:
	isInitFail_(false),
	isReleaseFail_(false),
	fpsController_(nullptr)
{
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
