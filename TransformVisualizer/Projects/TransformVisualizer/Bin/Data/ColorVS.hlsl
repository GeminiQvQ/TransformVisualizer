cbuffer VSConstantBuffer : register(b0)
{
	float4x4 g_world_transform;
	float4x4 g_view_transform;
	float4x4 g_projection_transform;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	int color_index : COLOR_INDEX;
};

VSOutput vs_main(float3 position : POSITION, int color_index : COLOR_INDEX)
{
	VSOutput output;
	output.position = float4(position, 1.0f);
	output.position = mul(output.position, transpose(g_world_transform));
	output.position = mul(output.position, transpose(g_view_transform));
	output.position = mul(output.position, transpose(g_projection_transform));
	output.color_index = color_index;
	return output;
}