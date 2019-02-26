struct VertexToFragment
{
    float4 vertex : SV_POSITION;
    float3 color : TEXCOORD0;
};

float4 FragmentShader(VertexToFragment i) : SV_TARGET
{
    return float4(i.color, 1);
}