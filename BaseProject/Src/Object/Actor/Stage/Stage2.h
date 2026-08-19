#pragma once
#include "StageBase.h"

class Stage2 : public StageBase
{

protected:

    void InitLoad(void) override;

    void InitTransform(void) override;

    void InitCollider(void) override;

    void InitPost(void) override; // BGM再生用

private:

	// ステージ
	Transform mainStage_;

	// ステージの座標
	static constexpr VECTOR NEXT_STAGE_POS = { 0.0f, -100.0f, 0.0f };

	// ステージのスケール
	static constexpr VECTOR SECOND_STAGE_SCALE = { 2.0f, 1.0f, 2.0f };

};