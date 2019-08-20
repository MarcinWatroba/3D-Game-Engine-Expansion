#version 330

in vec2 TexCoord;
in vec3 Normal;
in vec3 VertPos;

layout (location = 0) out vec4 colour;
layout (location = 1) out vec4 BrightColor;  

uniform vec2 tiling;

uniform samplerCube depthMap[3];

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Directional_Light
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Point_Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float radius;

	int ui_depth_Map;

	bool casts_Shadow;
};


uniform Directional_Light directional_Light;
uniform Point_Light point_Light[20];
uniform vec3 viewPos;
uniform Material material;
uniform int numOfLights;
uniform float farPlane;

vec3 point_Lights[20];
float shadows[20];

vec3 create_DirectionalLight(Directional_Light light, vec3 normal, vec3 viewDir);
vec3 create_PointLight(Point_Light light, vec3 normal, vec3 VertPos, vec3 viewDir);
float ShadowCalculation(samplerCube depthMap_In, Point_Light light_In);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - VertPos);
	vec3 ambient = point_Light[0].ambient * vec3(texture(material.diffuse, vec2(TexCoord.x * tiling.x, (1 - TexCoord.y) * tiling.y)));
	//float shadows = ShadowCalculation();
	
	vec3 result;
	result = ambient;
	for (int i = 0; i < numOfLights; i++) 
	{
		if(point_Light[i].ui_depth_Map < 4)
		shadows[i] = ShadowCalculation(depthMap[point_Light[i].ui_depth_Map], point_Light[i]);
		else
		shadows[i] = 0.0f;
	}

	for (int i = 0; i < numOfLights; i++) 
	{
		point_Lights[i] = create_PointLight(point_Light[i], norm, VertPos, viewDir);
		result += ((1.0 - shadows[i]) * point_Lights[i]);
	}



	 float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    //if(brightness > 1.0)
     BrightColor = vec4(result * brightness, 1.0);

	colour = vec4(result, 1.f);

	//vec3 fragToLight = VertPos - point_Light[1].position; 
		
    //float closestDepth = texture(3, fragToLight).r;
	//closestDepth *= 1000.0f;  

	//colour = vec4(vec3(closestDepth / farPlane), 1.0); 
}

vec3 create_DirectionalLight(Directional_Light light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);


	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vec2(TexCoord.x * tiling.x, (1 - TexCoord.y) * tiling.y)));
	return (diffuse + specular);
}

vec3 create_PointLight(Point_Light light, vec3 normal, vec3 vertPos, vec3 viewDir)
{
	float distance = length(light.position - vertPos);
	if(distance < light.radius * 10)
	{
	vec3 lightDir = normalize(light.position - vertPos);
	float diff = max(dot(normal, lightDir), 0.f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);




	float dist = max(distance - light.radius, 0);
	float intensity = dist/light.radius + 1;
   	float attenuation = 1.f / (intensity*intensity);


	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vec2(TexCoord.x * tiling.x, (1 - TexCoord.y) * tiling.y)));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vec2(TexCoord.x * tiling.x, (1 - TexCoord.y) * tiling.y)));


	diffuse *= attenuation;
	specular *= attenuation;

	return (diffuse + specular);
	}
	else
	return vec3(0.0);
}

float ShadowCalculation(samplerCube depthMap_In, Point_Light light_In)
{
    vec3 fragToLight = VertPos - light_In.position; 

    float closestDepth = texture(depthMap_In, fragToLight).r;
	closestDepth *= farPlane;  
	float currentDepth = length(fragToLight);  
	float bias = 0.06; 
    float shadow = currentDepth  > closestDepth + bias ? 1.0 : 0.0;

    return shadow;
}  