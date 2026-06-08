#pragma once
#include <memory>
#include "../PostEffectBase.h"
#include <vector>

class PostEffectManager
{
public:

	PostEffectManager(void);
	~PostEffectManager(void);
	void Init(void);
	void Load(void);
	void Update(void);
	void Draw(int mainScreen);
	void Release(void);

private:

	std::vector<std::unique_ptr<PostEffectBase>> effects;
	int currentScreen_;
	
	// 頂点
	VERTEX2DSHADER vertexs_[PostEffectBase::NUM_VERTEX];

	// 頂点インデックス
	WORD indexes_[PostEffectBase::NUM_VERTEX_IDX];
};

