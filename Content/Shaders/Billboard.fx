#include "RenderStates.fx"

cbuffer cbPerFrame
{
	matrix gWorld;
	matrix gViewProj;
	float3 gCameraPositionW;
	
};

cbuffer cbPerObject
{
	float gAlphaValue;
	float3 gPlayerPos;
};

cbuffer cbConstants
{
	float2 gTexCoords[4] = { float2(0.f, 1.f),
							 float2(0.f, 0.f),
							 float2(1.f, 1.f),
							 float2(1.f, 0.f) };
};



Texture2D	gTexture;
bool		gAnimation;
uint		gFrame;
uint		gMaxFrames;
float3		gMoveVector;

//tar animationstexturer av spritesheets i enbart x-led
float2 Animation(int corner)
{
 	float mf = gMaxFrames;
	float step = 1/mf;

	float x,y;
	if (gMoveVector.z < 0)
	{
		if(corner == 2)
		{
			x = step*gFrame;
			y = 1.f;
		}
		else if(corner == 3)
		{
			x = step*gFrame;
			y = 0.f;
		}
		else if(corner == 0)
		{
			x = step+gFrame*step;
			y = 1.f;
		}
		else if(corner == 1)
		{
			x = step+gFrame*step;
			y = 0.f;
		}
	}
	else if (gMoveVector.x > 0)
	{
		if(corner == 1)
		{
			x = step*gFrame;
			y = 1.f;
		}
		else if(corner == 3)
		{
			x = step*gFrame;
			y = 0.f;
		}
		else if(corner == 0)
		{
			x = step+gFrame*step;
			y = 1.f;
		}
		else if(corner == 2)
		{
			x = step+gFrame*step;
			y = 0.f;
		}
	}
	else if (gMoveVector.x < 0)
	{
		if(corner == 2)
		{
			x = step*gFrame;
			y = 1.f;
		}
		else if(corner == 0)
		{
			x = step*gFrame;
			y = 0.f;
		}
		else if(corner == 3)
		{
			x = step+gFrame*step;
			y = 1.f;
		}
		else if(corner == 1)
		{
			x = step+gFrame*step;
			y = 0.f;
		}
	}
	else
	{	
		if(corner == 0)
		{
			x = step*gFrame;
			y = 1.f;
		}
		else if(corner == 1)
		{
			x = step*gFrame;
			y = 0.f;
		}
		else if(corner == 2)
		{
			x = step+gFrame*step;
			y = 1.f;
		}
		else if(corner == 3)
		{
			x = step+gFrame*step;
			y = 0.f;
		}
	}



	return float2(x,y);
}

SamplerState linSampler
{
	Filter	 = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VSIn
{
	float3 positionW : POSITION_W;
	float2 sizeW	 : SIZE_W;
};

struct GSIn
{
	float3 positionW : POSITION_W;
	float2 sizeW	 : SIZE_W;
};

struct PSIn
{
	float4 positionH : SV_POSITION;
	float3 positionW : POSITION_W;
	float3 normalW	 : NORMAL_W;
	float2 tex0		 : TEX0;
};

GSIn VS(VSIn input)
{
	GSIn output;
	output.positionW = mul(float4(input.positionW, 1.f), gWorld).xyz;
	output.sizeW	 = input.sizeW;
	return output;
}

[maxvertexcount(4)]
void GS(point GSIn input[1], inout TriangleStream<PSIn> stream)
{
	float3 up = float3(0.f, 1.f, 0.f);
	float3 look = gCameraPositionW - input[0].positionW;
	look = normalize(look);
	float3 right = cross(up, look);

	float halfWidth  = 0.5f * input[0].sizeW.x;
	float halfHeight = 0.5f * input[0].sizeW.y;

	float4 positions[4];
	positions[0] = float4(input[0].positionW + halfWidth * right - halfHeight * up, 1.f);
	positions[1] = float4(input[0].positionW + halfWidth * right + halfHeight * up, 1.f);
	positions[2] = float4(input[0].positionW - halfWidth * right - halfHeight * up, 1.f);
	positions[3] = float4(input[0].positionW - halfWidth * right + halfHeight * up, 1.f);

	PSIn output;
	[unroll]
	for (int i = 0; i < 4; i++)
	{
		if(gAnimation)
			output.tex0		 = float2(Animation(i));
		else
			output.tex0		 = gTexCoords[i];

		output.positionH = mul(positions[i], gViewProj);
		output.positionW = positions[i].xyz;
		output.normalW	 = look;

		stream.Append(output);
	}
}



float4 PS(PSIn input) : SV_TARGET
{
	float4 texColor = gTexture.Sample(linSampler, float3(input.tex0, 0.f));
	return float4(texColor.xyz, texColor.w * gAlphaValue);
}

technique11 BillboardTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(CompileShader(gs_4_0, GS()));
		SetPixelShader(CompileShader(ps_4_0, PS()));

		SetRasterizerState(NULL);

		SetBlendState(AlphaBlending2, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
	}
}