//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------

float mFogRange = 100;

SamplerState linearSampler
{

	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;

};

BlendState AlphaBlending1
{
        AlphaToCoverageEnable = TRUE;
        BlendEnable[0] = TRUE;

        SrcBlend = ONE;
		SrcBlendAlpha = SRC_ALPHA;

		BlendOp = ADD;

        DestBlend = ONE;
		DestBlendAlpha = SRC_ALPHA;
        
        BlendOpAlpha = SUBTRACT;
        RenderTargetWriteMask[0] = 0x0F;
};

BlendState AlphaBlending2
{
        AlphaToCoverageEnable = TRUE;
        BlendEnable[0] = TRUE;
        SrcBlend = SRC_ALPHA;
        DestBlend = INV_SRC_ALPHA;
        BlendOp = ADD;
        SrcBlendAlpha = SRC_ALPHA;
        DestBlendAlpha = INV_SRC_ALPHA;
        BlendOpAlpha = ADD;
        RenderTargetWriteMask[0] = 0x0F;
};

float4 Fog(float3 eyePos, float3 objPos)
{
	
	float3 distance = eyePos-objPos;

	float d = mFogRange - length(distance);

	float norm = (d/mFogRange);

	if(norm > 0.8f)
	{norm = 1;}

	return norm;

}

RasterizerState NoCulling
{
	CullMode = None;
	FillMode = Solid;
};

RasterizerState CullBack
{
    CullMode = Back;
    FillMode = Solid;
};