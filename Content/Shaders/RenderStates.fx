//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------

SamplerState linearSampler
{

	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;

};

//snyggare men kräver sortering
BlendState AlphaBlending1
{
        AlphaToCoverageEnable = FALSE;
        BlendEnable[0] = TRUE;
        SrcBlend = SRC_ALPHA;
        DestBlend = INV_SRC_ALPHA;
        BlendOp = ADD;
        SrcBlendAlpha = SRC_ALPHA;
        DestBlendAlpha = INV_SRC_ALPHA;
        BlendOpAlpha = ADD;
        RenderTargetWriteMask[0] = 0x0F;
};

BlendState AlphaBlending2
{
        AlphaToCoverageEnable = TRUE; //<--- enda skillnaden
        BlendEnable[0] = TRUE;
        SrcBlend = SRC_ALPHA;
        DestBlend = INV_SRC_ALPHA;
        BlendOp = ADD;
        SrcBlendAlpha = SRC_ALPHA;
        DestBlendAlpha = INV_SRC_ALPHA;
        BlendOpAlpha = ADD;
        RenderTargetWriteMask[0] = 0x0F;
};

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
