/***************************************************************************************************
 * @file  default.frag
 * @brief Default fragment shader
 **************************************************************************************************/

#version 460 core

#define NB_POINT_LIGHTS 4

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 color;

out vec4 fragColor;

uniform uint attributes;
uniform vec3 cameraPos;

uniform Material material;

uniform DirectionalLight directionalLight;
uniform SpotLight spotlight;
uniform PointLight[NB_POINT_LIGHTS] pointLights;

uniform bool globalLighting;

vec3 phongLighting();
vec3 calculateDirectionalLight(vec3 viewDirection);
vec3 calculateSpotLight(vec3 viewDirection);
vec3 calculatePointLights(vec3 viewDirection);

void main() {
    fragColor = vec4(1.0f);

    if(((attributes >> 1) & 1u) == 1u) { // Mesh has normals.
        fragColor = vec4(0.0f, 0.0, 0.0, 1.0f);
        vec3 viewDirection = normalize(cameraPos - position);

        if(globalLighting) {
            fragColor.rgb += calculateDirectionalLight(viewDirection);
        }
        fragColor.rgb += calculateSpotLight(viewDirection);
        fragColor.rgb += calculatePointLights(viewDirection);
    } else if(((attributes >> 2) & 1u) == 1u) { // Mesh doesn't have normals but has texture coords.
        fragColor *= texture(material.diffuse, texCoord);
    } else { // Mesh has neither normals or texture coords.
        fragColor = vec4(1.0f);
    }

    if(((attributes >> 3) & 1u) == 1u) {
        fragColor.rgb *= color;
    }

//    // Fog
//    fragColor.rgb *= exp(-0.02f * length(cameraPos - position));
}

vec3 calculateDirectionalLight(vec3 viewDirection) {
    vec3 lightDirection = normalize(-directionalLight.direction);

    vec3 map;
    if(((attributes >> 2) & 1u) == 1u) {
        map = vec3(texture(material.diffuse, texCoord));
    } else {
        map = vec3(1.0f);
    }

    vec3 reflectionDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDir), 0.0f), material.shininess);

    vec3 ambient = directionalLight.ambient * map;
    vec3 diffuse = directionalLight.diffuse * max(dot(normal, lightDirection), 0.0f) * map;
    vec3 specular = directionalLight.specular * spec * vec3(texture(material.specular, texCoord));

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(vec3 viewDirection) {
    vec3 lightDirection = normalize(spotlight.position - position);
    
    vec3 map;
    if(((attributes >> 2) & 1u) == 1u) {
        map = vec3(texture(material.diffuse, texCoord));
    } else {
        map = vec3(1.0f);
    }

    vec3 reflectionDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDir), 0.0f), material.shininess);

    float distance = length(spotlight.position - position);
    float attenuation = 1.0f / (spotlight.constant +
                                spotlight.linear * distance +
                                spotlight.quadratic * distance * distance);

    float theta = dot(lightDirection, normalize(-spotlight.direction));
    float epsilon = spotlight.cutOff - spotlight.outerCutOff;
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = spotlight.ambient * map;
    vec3 diffuse = spotlight.diffuse * max(dot(normal, lightDirection), 0.0f) * map;
    vec3 specular = spotlight.specular * spec * vec3(texture(material.specular, texCoord));

    return attenuation * intensity * (ambient + diffuse + specular);
}

vec3 calculatePointLights(vec3 viewDirection) {
    vec3 total = vec3(0.0f);
    
    vec3 map;
    if(((attributes >> 2) & 1u) == 1u) {
        map = vec3(texture(material.diffuse, texCoord));
    } else {
        map = vec3(1.0f);
    }

    for(int i = 0 ; i < NB_POINT_LIGHTS ; ++i) {
        vec3 lightDirection = normalize(pointLights[i].position - position);

        vec3 reflectionDir = reflect(-lightDirection, normal);
        float spec = pow(max(dot(viewDirection, reflectionDir), 0.0f), material.shininess);

        float distance = length(pointLights[i].position - position);
        float attenuation = 1.0f / (pointLights[i].constant +
                                    pointLights[i].linear * distance +
                                    pointLights[i].quadratic * distance * distance);

        vec3 ambient = pointLights[i].ambient * map;
        vec3 diffuse = pointLights[i].diffuse * max(dot(normal, lightDirection), 0.0f) * map;
        vec3 specular = pointLights[i].specular * spec * vec3(texture(material.specular, texCoord));

        total += attenuation * (ambient + diffuse + specular);
    }

    return total;
}