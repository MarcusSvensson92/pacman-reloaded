struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;

	float3 attenuation;
	float pad; // Prevents packing-errors
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
};

float4 CalcPointLight(PointLight light, Material material,
					  float3 position, float3 normal,
					  float3 toCamera, float4 color)
{
	float4 litColor = float4(0.f, 0.f, 0.f, 0.f);

	float3 lightVector = light.position - position;
	float d = length(lightVector);
	if (d > light.range)
		return float4(0.f, 0.f, 0.f, 0.f);
	lightVector /= d;

	// Calculate ambient light
	litColor += light.ambient * material.ambient * color;

	float diffuseFactor = dot(lightVector, normal);

	[flatten]
	if (diffuseFactor > 0.f)
	{
		float3 reflection	  = reflect(-lightVector, normal);
		float  specularFactor = pow(max(dot(reflection, toCamera), 0.f), material.specular.w);

		float attenuation = 1.f / dot(light.attenuation, float3(1.f, d, d * d));

		// Calculate diffuse light
		litColor += light.diffuse  * material.diffuse  * diffuseFactor  * attenuation * color;
		// Calculate specular light
		litColor += light.specular * material.specular * specularFactor * attenuation;
	}

	return litColor;
}