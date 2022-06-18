// Basic Texture Shader

#type vertex
#version 450 core

#include "assets/editor/shaders/Uniforms.glsl"

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec3 a_Tangent;
layout (location = 3) in vec2 a_TexCoord;

struct VertexOutput
{
	vec3 PositionWS;
	vec3 NormalWS;
	vec3 TangentWS;
	vec2 TexCoord;
};

struct CameraOutput
{
	vec3 Position;
	mat4 View;
};

layout (location = 0) out flat int v_EntityID;
layout (location = 1) out VertexOutput v_VertexOutput;
layout (location = 5) out CameraOutput v_CameraOutput;

void main()
{
	v_VertexOutput.PositionWS = vec3(u_Model.Transform * vec4(a_Position, 1.0f));
	v_VertexOutput.NormalWS = mat3(u_Model.NormalMatrix) * a_Normal;
	v_VertexOutput.TangentWS = mat3(u_Model.NormalMatrix) * a_Tangent;
	v_VertexOutput.TexCoord = a_TexCoord;

	v_CameraOutput.Position = u_Camera.Position;
	v_CameraOutput.View = u_Camera.View;

	v_EntityID = u_Model.EntityID;

	gl_Position = u_Camera.Projection * u_Camera.View * vec4(v_VertexOutput.PositionWS, 1.0f);
}

#type fragment
#version 450 core

#include "assets/editor/shaders/Uniforms.glsl"

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec4 o_EntityID;

#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 32

#define MAX_CASCADE_COUNT 4

struct LightBase
{
	vec4 Color;
	float Intensity;
	bool bCastShadow;
	int ShadowType;
	float DepthBias;
	float NormalBias;
	float FilterSize;
	float LightSize;
};

struct DirectionalLight
{
	LightBase Base;
	vec3 Direction;
	int CascadeCount;
	vec4 CascadeSplits; // DO NOT USE FLOAT ARRAY as float[4] will become vec4[4] due to padding!
	mat4 CascadeReferenceMatrix;
	vec4 CascadeOffsets[MAX_CASCADE_COUNT];
	vec4 CascadeScales[MAX_CASCADE_COUNT];
	float CascadeBlendThreshold;
};

struct PointLight
{
	LightBase Base;
	vec3 Position;
	float Radius;
};

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float Cutoff;
};

layout (std140, binding = 3) uniform Light
{
	DirectionalLight u_DirectionalLight;
	PointLight u_PointLights[MAX_POINT_LIGHTS];
	SpotLight u_SpotLights[MAX_SPOT_LIGHTS];
	int u_NumPointLights;
	int u_NumSpotLights;
};

[Property]layout (std140, binding = 4) uniform Material
{
	bool bUseNormalMapping;
	float Shininess;
}u_Material;

struct VertexInput
{
	vec3 PositionWS;
	vec3 NormalWS;
	vec3 TangentWS;
	vec2 TexCoord;
};

struct CameraInput
{
	vec3 Position;
	mat4 View;
};

layout (location = 0) in flat int v_EntityID;
layout (location = 1) in VertexInput v_VertexInput;
layout (location = 5) in CameraInput v_CameraInput;

layout (binding = 0) uniform sampler2D u_ScreenSpaceShadowMap;
[Property]layout (binding = 1) uniform sampler2D u_DiffuseTexture;
[Property]layout (binding = 2) uniform sampler2D u_SpecularTexture;
[Property]layout (binding = 3) uniform sampler2D u_NormalTexture;

vec4 CalculateLightInternal(LightBase base, vec3 lightDirection, vec3 normal)
{
	// Diffuse
	float diffuse = max(dot(normal, normalize(-lightDirection)), 0.0f);
	vec4 diffuseColor = diffuse * texture(u_DiffuseTexture, v_VertexInput.TexCoord);

	// Specular
	vec3 viewDirection = normalize(v_CameraInput.Position - v_VertexInput.PositionWS);
	vec3 reflectDirection = reflect(lightDirection, normal);
	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), u_Material.Shininess >= 1.0f ? u_Material.Shininess : 1.0f);
	vec4 specularColor = specular * texture(u_SpecularTexture, v_VertexInput.TexCoord);

	return (diffuseColor + specularColor) * base.Color * base.Intensity;
}

vec4 CalculateDirectionalLight(vec3 normal)
{
	// Diffuse + specular
	vec4 color = CalculateLightInternal(u_DirectionalLight.Base, u_DirectionalLight.Direction, normal);

	vec2 screenUV = vec2(gl_FragCoord.x / u_Global.ScreenSize.x, gl_FragCoord.y / u_Global.ScreenSize.y);
	// Shadow
	vec4 shadow = texture(u_ScreenSpaceShadowMap, screenUV);

	// Ambient
	vec4 ambientColor = texture(u_DiffuseTexture, v_VertexInput.TexCoord) * 0.3f;

	return ambientColor + shadow * color;
}

vec4 CalculatePointLight(PointLight pointLight, vec3 normal)
{
	if (pointLight.Radius <= 0.0f) return vec4(0.0f);

	vec3 lightDirection = v_VertexInput.PositionWS - pointLight.Position;
	float lightDistance = length(lightDirection);
	lightDirection = normalize(lightDirection);

	// Diffuse + specular
	vec4 color = CalculateLightInternal(pointLight.Base, lightDirection, normal);

	// Shadow
	float shadow = 0.0f;
	//float shadow = u_DirectionalLight.Base.bCastShadow ? CalculateShadow(base, lightDirection, normal) : 0.0f;

	// Ambient
	vec4 ambientColor = texture(u_DiffuseTexture, v_VertexInput.TexCoord) * 0.3f;

	float normalizedDistance = lightDistance / pointLight.Radius;
	float attenuation = clamp(1.0f / (1.0f + 25.0f * normalizedDistance * normalizedDistance) *
								clamp((1.0f - normalizedDistance) * 5.0f, 0.0f, 1.0f)
							, 0.0f, 1.0f);

	return (ambientColor + (1.0f - shadow) * color) * attenuation;
}

vec4 CalculateSpotLight(SpotLight spotLight, vec3 normal)
{
	vec3 lightDirection = normalize(v_VertexInput.PositionWS - spotLight.Base.Position);
	float spotFactor = dot(lightDirection, spotLight.Direction);

	if (spotFactor > spotLight.Cutoff)
	{
		vec4 color = CalculatePointLight(spotLight.Base, normal);
		float intensity = (1.0f - (1.0f - spotFactor) / (1.0f - spotLight.Cutoff));
		return color * intensity;
	}
	else
	{
		return vec4(0.0f);
	}
}

vec3 GetFinalNormalWS(vec3 vertexNormal, vec3 vertexTangent, vec3 textureNormal)
{
	const vec3 N = normalize(vertexNormal);
	vec3 T = normalize(vertexTangent);
	T = normalize(T - dot(T, N) * N); // Re-orthogonalize T with respect to N
	const vec3 B = cross(N,T);
	
	const mat3 TBN = mat3(T, B, N);
	return u_Material.bUseNormalMapping ? TBN * normalize(textureNormal * 2.0f - 1.0f) : N;
}

void main()
{
	vec3 textureNormal = texture(u_NormalTexture, v_VertexInput.TexCoord).rgb;
	vec3 normalWS = GetFinalNormalWS(v_VertexInput.NormalWS, v_VertexInput.TangentWS, textureNormal);

	vec4 totalLight = CalculateDirectionalLight(normalWS);
	for (int i = 0; i < u_NumPointLights; ++i)
	{
		totalLight += CalculatePointLight(u_PointLights[i], normalWS);
	}
	for (int i = 0; i < u_NumSpotLights; ++i)
	{
		totalLight += CalculateSpotLight(u_SpotLights[i], normalWS);
	}

	o_Color = totalLight;
	// Force set opaque mode due to rgb texture's alpha is 0
	o_Color.a = 1.0f;

	o_EntityID = vec4(v_EntityID, 0.0f, 0.0f, 1.0f);
}
