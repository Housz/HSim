#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
uniform vec4 ourColor;

uniform vec3 lightPos;

uniform vec3 viewPos;

void main() {
    vec4 lightColor = vec4(1.0);
    // vec3 lightPos = vec3(5, 5, 5);

    // ambient
    float ambientStrength = 0.5;

    vec4 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = vec4(diff * lightColor.rgb, 1.0);

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * lightColor;  

    // vec4 result = (ambient + diffuse + specular) * ourColor;
    vec4 result = (ambient + diffuse) * ourColor;

    // linear fog
    float fog_maxdist = 100.0;
    float fog_mindist = 0.1;
    vec4 fog_colour = vec4(0.6, 0.6, 0.6, 1.0);
    float dist = length(FragPos.xyz);
    float fog_factor = (fog_maxdist - dist) /
        (fog_maxdist - fog_mindist);
    fog_factor = clamp(fog_factor, 0.0, 1.0);

    FragColor = mix(fog_colour, result, fog_factor);

    // FragColor = result;
}