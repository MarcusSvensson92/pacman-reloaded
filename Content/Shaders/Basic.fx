#include "Lights.fx"
#include "RenderStates.fx"

Texture2D mTexture;

struct VSIn
{
	float3 Pos			: POSITION;
	float3 Normal		: NORMAL;
	float2 Tex			: TEXCOORD;
	float4 diffuse		: DIFFUSE;
	float4 spec			: SPECULAR;
};

struct PSSceneIn
{
	float4 Pos			: SV_Position;
	float2 Tex			: TEXCOORD;
	float3 posW			: POSITION;
	float3 normalW		: NORMAL;
	float4 diffuse		: DIFFUSE;
	float4 spec			: SPECULAR;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSIn input)
{
	PSSceneIn output = (PSSceneIn)1;

	output.posW = mul(float4(input.Pos,1.0f),gWorld);
	output.normalW = mul(float4(input.Normal,0.0f),gWorld);
	output.Pos = mul(float4(input.Pos, 1), gWVP);

	output.Tex = mul(float4(input.Tex.x,input.Tex.y, 0.0f, 1.0f), gTextureTransform);

	output.spec = input.spec;
	output.diffuse = input.diffuse;
	
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	Material gMaterial;
	gMaterial.Ambient = float4(0.5f, 0.5f, 0.5f, 1.0f);
	gMaterial.Diffuse = float4(0.5f, 0.5f, 0.5f, 1.0f);
	gMaterial.Specular = float4(0.2f, 0.2f, 0.2f, 1.0f);

	input.normalW = normalize(input.normalW);

	float3 toEyeW = normalize(gEyePos - input.posW);
	
	float4 A,D,S,ambient,diffuse,specular;

	ComputeDirectionalLight( gMaterial, input.normalW, toEyeW, ambient, diffuse, specular );

	[loop]
	for( uint i = 0;i < CANDYLIGHTS; i++ )
	{
		ComputePointLight(gMaterial, gCandyLights[i], input.posW, input.normalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		specular += S;
	}

	[loop]
	for( uint i = 0;i < MOVINGLIGHTS; i++ )
	{
		ComputePointLight(gMaterial, gMovingLights[i], input.posW, input.normalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		specular += S;
	}

	float4 texColor = mTexture.Sample(linearSampler, input.Tex);

	float4 litColor = texColor*(ambient + diffuse) + specular;

	litColor.a = gMaterial.Diffuse.a;

	return litColor;
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
	    
	    SetRasterizerState(CullBack);

		SetBlendState( AlphaBlending2, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }  
}