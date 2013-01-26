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
	float3 pos;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 att;
	float range;
};

struct SurfaceInfo
{
	float3 pos;
	float3 normal;
	float4 diffuse;
	float4 spec;
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

};

float3 CalcDirectionalLight(SurfaceInfo sfi,float3 normalStatic ,float3 toEye)
{
	DirectionalLight L;

	float3 lightColor = float3(0.0f,0.0f,0.0f);

	L.specular = float4(0.6f,0.6f,0.6f,0.1f);
	L.diffuse = float4(0.6f,0.6f,0.6f,0.1f);
	L.direction = float3(-1,-1,0);
	L.ambient = float4(0.1f,0.1f,0.1f,1.0f);

	lightColor += sfi.diffuse * L.ambient;

	float3 lightVec = -L.direction;

	float diffuseFactor = dot(lightVec,normalStatic);

	if(diffuseFactor > 0.0f)
	{
		float specPower = max(sfi.spec.a,1.0f);

		float3 v = reflect(-lightVec,normalStatic);

		float specFactor = pow(max(dot(v,toEye),0.0f),specPower);

		lightColor += diffuseFactor * sfi.diffuse * L.diffuse;
		lightColor += specFactor * sfi.spec * L.specular;
	}

	//return normalStatic;
	return lightColor;
}


float3 CalcPointLight(SurfaceInfo sfi,PointLight light,float3 eyePos)
{
	float3 lightColor = float3(0.0f,0.0f,0.0f);
	float3 lightVector = light.pos - sfi.pos;
	float d = length(lightVector);

	if(d> light.range)
	return float3(0.0f,0.0f,0.0f);

	lightVector /= d;

	lightColor += sfi.diffuse*light.ambient;

	float diffuseFactor = dot(lightVector,sfi.normal);

	if(diffuseFactor > 0.0f)
	{

		float specPower = max(sfi.spec.a,1.0f);
		float3 toEye = normalize(eyePos - sfi.pos);
		float3 r = reflect(-lightVector,sfi.normal);
		float specFactor = pow(max(dot(r,toEye),0.0f),specPower);

		lightColor += diffuseFactor * sfi.diffuse * light.diffuse;
		lightColor += specFactor * sfi.spec * light.spec;

	}

	return lightColor /dot(light.att, float3(1.0f,d,d*d));

}