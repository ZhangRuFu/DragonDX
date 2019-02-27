struct AppInput
{
    float3 vertex : POSITION;
    float3 color : COLOR;
};

struct VertexToFragment
{
    float4 vertex : SV_POSITION;
    float3 color : TEXCOORD0;
};

VertexToFragment Vertex(AppInput i)
{
    VertexToFragment o;
    o.vertex = float4(i.vertex);
    o.color = i.color;
    return o;
}