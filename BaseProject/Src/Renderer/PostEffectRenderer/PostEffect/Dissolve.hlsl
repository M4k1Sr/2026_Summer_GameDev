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
    AddressU = CLAMP; // 繰り返しではなく端を伸ばす
    AddressV = CLAMP;
};

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float noiseValue = noiseTexture.Sample(noiseSampler, PSInput.uv).r;
    
    // しきい値が 0.01 以下のときは、clip処理を無視する（描画する）
    if (thresholdParam.x > 0.01f)
   {
        clip(noiseValue - thresholdParam.x);
    }
    
    return PSInput.diffuse;
    
}
