#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
 
	// UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
		// 描画しない(アルファテスト)
        discard;
    }
    float4 dstCol = srcCol * PSInput.diffuse;
    
    float color = 0.33f;
    // モノクロ処理
    float3 grayScaleWeight = float3(color, color, color);
    
    // 内積（dot）を使って、RGBから単一の輝度（Y）を計算
    float gray = dot(dstCol.rgb, grayScaleWeight);
    
    dstCol.rgb = float3(gray, gray, gray);
    
    return dstCol;
 
}