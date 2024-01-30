#version 330 core

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material {
  // vec3 ambient;
  // vec3 diffuse;
  sampler2D diffuse;
  vec3 specular;
  float shininess;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
  // ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  vec3 diffuse = light.diffuse * (max(0.0f, dot(lightDir, norm)) * vec3(texture(material.diffuse, TexCoord)));

  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  vec3 specular = light.specular * (pow(max(0.0f, dot(reflectDir, viewDir)), material.shininess) * material.specular);

  FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
