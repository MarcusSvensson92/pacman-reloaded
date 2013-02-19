cbuffer PerObject
{
	matrix vp;
	float2 frame;
}

Texture2D gTexture;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VSIn
{
	float3 PosL		: POSITION;
	float2 Tex		: TEXCOORD;
};

struct PSSceneIn
{
	float4 PosH		: SV_POSITION;
	float2 Tex		: TEXCOORD;
};

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
RasterizerState NoCulling
{
	CullMode = NONE;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------

PSSceneIn VSScene(VSIn input)
{
	PSSceneIn output;
	output.PosH = mul(float4(input.PosL, 1.0f), vp);
	output.Tex = input.Tex + frame;
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------

float4 PSScene(PSSceneIn input) : SV_Target
{	
	float4 texColor = gTexture.Sample(samAnisotropic, input.Tex);
	return texColor;
}

//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 BasicTech
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    
	    SetRasterizerState( NoCulling );
    }  
}