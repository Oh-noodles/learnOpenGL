#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
  float ambientStrength = 0.1f;
  vec3 ambient = lightColor * ambientStrength;

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  vec3 diffuse = max(0.0f, dot(lightDir, norm)) * lightColor;

  float specularStrength = 0.5f;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  vec3 specular = pow(max(0.0f, dot(reflectDir, viewDir)), 32) * lightColor * specularStrength;

  FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
}
