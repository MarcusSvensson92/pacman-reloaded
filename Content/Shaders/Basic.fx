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
	output.normalW = mul(float4(input.Normal,0.0f),gWVP);
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
	input.normalW = normalize(input.normalW);

	float4 color = float4(0,0,0,0);

	color = mTexture.Sample(linearSampler,input.Tex);

	SurfaceInfo sfi = {input.posW,input.normalW,color,input.diffuse};

	float3 lightColor;

	lightColor = CalcPointLight(sfi,gLight,gEyePos) + CalcDirectionalLight(sfi,input.normalW, gEyePos);

	return color;
	//return float4(lightColor,color.a);
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
	    
	    SetRasterizerState( CullBack );

		SetBlendState( AlphaBlending2, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }  
}