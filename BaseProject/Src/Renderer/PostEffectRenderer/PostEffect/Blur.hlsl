#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color; 
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{

    float2 texelSize = float2(1.0f / 1024.0f, 1.0f / 640.0f) * (g_color.y * 0.5);
    
    float4 blurCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // 十字方向の4点＋中心の計5点をサンプリングして平均化する
    blurCol += tex.Sample(texSampler, PSInput.uv);
    blurCol += tex.Sample(texSampler, PSInput.uv + float2(-texelSize.x, 0.0f));
    blurCol += tex.Sample(texSampler, PSInput.uv + float2(texelSize.x, 0.0f));
    blurCol += tex.Sample(texSampler, PSInput.uv + float2(0.0f, -texelSize.y));
    blurCol += tex.Sample(texSampler, PSInput.uv + float2(0.0f, texelSize.y));
    blurCol /= 5.0f; 

    if (blurCol.a < 0.01f)
    {
        discard;
    }
 
    
    float4 dstCol = blurCol * PSInput.diffuse;
 
   
    return dstCol;
}