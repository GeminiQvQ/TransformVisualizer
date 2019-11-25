cbuffer PSConstantBuffer : register(b1)
{
	float4 g_color[6];
};

struct VSOutput
{
	float4 position : SV_POSITION;
	int color_index : COLOR_INDEX;
};

float4 ps_main(VSOutput input) : SV_TARGET
{
	return g_color[input.color_index];
}