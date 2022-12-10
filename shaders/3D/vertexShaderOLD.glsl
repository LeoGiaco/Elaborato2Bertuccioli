#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat3 normModel;
uniform int blinn = 0;
uniform int useDirLight = 0;
uniform int cartoon = 0;
uniform int shades = 5;

uniform float minHeight;
uniform float maxHeight;
float height01;

uniform int useTexture = 0;
uniform sampler1D samplerSurface;

vec3 texCol;

struct SpotLight 
{
	vec3 position;		// The light's position.
	vec3 direction;		// The light's direction.

	float cutOff;		// The light's inner angle.
	float outerCutOff;	// The light's outer angle.
	float power;		// The light's power.

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
// uniform SpotLight light;

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform PointLight pointLight;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

uniform vec3 viewPos;

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

out vec4 color;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec;
	if (blinn == 1) 
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);  
    	spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
    	spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}
    // combine results
    vec3 ambient = light.ambient * material.ambient * texCol;
    vec3 diffuse = light.diffuse * (diff * material.diffuse) * texCol;
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(fragPos - light.position);
	// Diffuse shading.
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading.
	float spec;
	if (blinn == 1) 
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);  
    	spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
    	spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}
	// Attenuation.
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	// Combine results.
	vec3 ambient = light.ambient * material.ambient * texCol;
	vec3 diffuse = light.diffuse * (diff * material.diffuse) * texCol;
	vec3 specular = light.specular * (spec * material.specular);

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);    							// Light's direction.
	vec3 halfwayDir = normalize(lightDir + viewDir);	
    float theta = dot(lightDir, normalize(-light.direction));						    // The angle of the light when it hits the fragment.
	float epsilon = light.cutOff - light.outerCutOff;								 
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	float diff = max(dot(normal, lightDir), 0.0);

	float spec;
	if (blinn == 1) 
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);  
    	spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
    	spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}

	float dist = length(light.position - fragPos);
	float attenuation = light.power / (light.constant + light.linear * dist +		// Attenuation (light's decrease of energy).
		light.quadratic * (dist * dist));

	vec3 ambient = light.ambient * material.ambient * texCol;  		// Ambient light (light always present).
	vec3 diffuse = light.diffuse * (diff * material.diffuse) * texCol;	                        // Diffuse light (light bouncing back with partial strength).
	vec3 specular = light.specular * (spec * material.specular);	                    // Specular light (light bouncing back full strength).

	return (ambient + (diffuse + specular) * intensity) * attenuation;
}

void main()
{    
    vec3 normalV = normalize(normModel * vertexNormal);

	vec4 pos = model * vec4(aPos, 1.0);
    height01 = clamp((length(aPos) - minHeight) / (maxHeight - minHeight), 0, 1);
    gl_Position = projection * view * pos;

    vec3 viewDir = normalize(viewPos - vec3(pos));

	vec3 result;
	texCol = useTexture == 0 ? vec3(1, 1, 1) : texture(samplerSurface, height01).xyz;
	
	if (useDirLight == 1) 
	{
		if (cartoon == 1)
		{
  			float intensity = max(0.2, dot(normalize(-dirLight.direction), normalV));
  			intensity = intensity - mod(intensity, 1.0 / shades);

			result = material.diffuse * texCol * (vec3(1.0) * intensity);
		}
		else
		{
			result = CalcDirLight(dirLight, normalV, viewDir);
		}
	}
	else 
	{
		if (cartoon == 1)
		{
  			float intensity =dot(normalize(pos.xyz - pointLight.position), normalV);
  			intensity = intensity - mod(intensity, 1.0 / shades);

			result = material.diffuse * texCol * (vec3(1.0) * intensity);
		}
		else
		{
			result = CalcPointLight(pointLight, normalV, pos.xyz, viewDir);
		}
	}


	color = vec4(result, 1.0);
}
