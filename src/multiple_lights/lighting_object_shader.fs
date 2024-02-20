#version 330 core

struct DirLight {
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

struct Material {
  // vec3 ambient;
  // vec3 diffuse;
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

#define NR_POINT_LIGHTS 4

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 result = calcDirLight(dirLight, norm, viewDir);
  for (int i = 0; i < NR_POINT_LIGHTS; i++) {
    result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
  }
  result += calcSpotLight(spotLight, norm, FragPos, viewDir);
  FragColor = vec4(result, 1.0f);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  // ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
  // diffuse
  vec3 diffuse = light.diffuse * (max(0.0f, dot(light.direction, normal)) * vec3(texture(material.diffuse, TexCoord)));
  // specular
  vec3 reflectDir = reflect(-light.direction, normal);
  vec3 specular = light.specular * (pow(max(0.0f, dot(reflectDir, viewDir)), material.shininess) * vec3(texture(material.specular, TexCoord)));
  return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  vec3 lightDir = normalize(light.position - fragPos);
  // ambient
  vec3 ambient = attenuation * light.ambient * vec3(texture(material.diffuse, TexCoord));
  // diffuse
  vec3 diffuse = attenuation * light.diffuse * (max(0.0f, dot(lightDir, normal)) * vec3(texture(material.diffuse, TexCoord)));
  // specular
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 specular = light.specular * (pow(max(0.0f, dot(reflectDir, viewDir)), material.shininess) * vec3(texture(material.specular, TexCoord)));
  return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(0.0f, dot(normal, lightDir));
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(0.0f, dot(viewDir, reflectDir)), material.shininess);
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;
  return ambient + diffuse + specular;
}
