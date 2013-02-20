#define CANDYLIGHTS 5
#define MOVINGLIGHTS 5


struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

cbuffer EveryFrame
{
	matrix gWorldInvTranspose;
	matrix gTextureTransform;

	float3 gEyePos;
	PointLight  gLight;
	
	DirectionalLight gDirLight;
	matrix gWorld;
	matrix gWVP;

	float3 gPlayerPos;

	PointLight gMovingLights[MOVINGLIGHTS];
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Reflect;
};

cbuffer StaticLights
{
	PointLight gCandyLights[CANDYLIGHTS];
}

void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = L.Position - pos;

	float d = length(lightVec);
	
	if(d > L.Range)
		return;

	lightVec /= d;

	ambient = mat.Ambient * L.Ambient;

	float diffuseFactor = dot(lightVec, normal);

	if(diffuseFactor > 0.0f)
	{
		float3 v			= reflect(-lightVec, normal);
		float specFactor	= pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	ambient *= att;
	diffuse *= att;
	spec	*= att;
}

void ComputeDirectionalLight(Material mat, float3 normalStatic ,float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	DirectionalLight L;
	L.ambient = float4(0.1f,0.1f,0.1f,1.0f);
	L.diffuse = float4(0.1f,0.1f,0.1f,1.0f);
	L.specular = float4(0.01f,0.01f,0.01f,1.0f);
	L.direction = float3(-1,-1,0);
	
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ambient += mat.Ambient * L.ambient;

	float3 lightVec = -L.direction;

	float diffuseFactor = dot(lightVec, normalStatic);

	[flatten]
	if(diffuseFactor > 0.0f)
	{
		float specPower = max(mat.Specular.a,1.0f);

		float3 v = reflect(-lightVec,normalStatic);

		float specFactor = pow(max(dot(v,toEye),0.0f),specPower);

		diffuse += diffuseFactor * mat.Diffuse * L.diffuse;
		spec += specFactor * mat.Specular * L.specular;
	}
}