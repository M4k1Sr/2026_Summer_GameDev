// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 【ここを追加】定数バッファを定義（ModelRenderer.hのPS用スロットは4なので、b4を指定）
cbuffer DissolveBuffer : register(b4)
{
    float4 thresholdParam; // 必ずfloat4で定義する
};

// 【ここを追加】ノイズテクスチャ用のスロット
Texture2D noiseTexture : register(t1);
SamplerState noiseSampler : register(s1)
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    // 1. UVを10倍にしてから frac() を通すことで、画像を10x10のタイル状に並べる
    float2 tiledUV = frac(PSInput.uv * 10000.0f);

    // 2. そのタイル化したUVでノイズをサンプリング
    float noiseValue = noiseTexture.Sample(noiseSampler, tiledUV).r;
    
    // edgeWidth を 0.05f から 0.2f くらいまで広げてみてください
    float edgeWidth = 0.2f;
    float edgeEffect = smoothstep(thresholdParam.x + edgeWidth, thresholdParam.x, noiseValue);
    
    // 判定範囲を少し広げて、消える瞬間に自然なエッジを作る
    clip(noiseValue - (thresholdParam.x - edgeWidth));
    
    return PSInput.diffuse;

    //エフェクトがかかっているか微妙なときのテスト用
    //上の処理をコメントアウトして下の処理を有効にする
    //return float4(1.0f, 0.0f, 0.0f, 1.0f); // 赤色で描画

}