#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 ex_N; 
in vec3 EyeDirection_cameraspace;

uniform sampler2D texture_diffuse1;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{    
    
    // FragColor = texture(texture_diffuse1, TexCoords);
    
    vec4 MaterialAmbientColor = vec4(0.5, 0.5, 0.5, 1.0);
    vec3 lightDirection = vec3(0.0, -1.0, 0.0);

    vec3 LightPosition_cameraspace = ( view * vec4(lightDirection,1.0f)).xyz;

    vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
    vec3 l = normalize( LightDirection_cameraspace );

    float intensity = clamp(dot(ex_N, l),0, 1);
    vec4 MaterialDiffuseColor = intensity * vec4(1.0, 0.0, 0.0, 1.0);

    // Intensidad del punto brilloso especular
    vec4 LightPower = vec4(1.0f,1.0f,1.0f,1.0f);
    // Normal de la superficie en cooridenadas de cámara
    vec3 Normal_cameraspace = ( view * model * vec4(ex_N,0)).xyz; 
    // Normal a la superficie normalizada
    vec3 n = normalize( Normal_cameraspace );
    // Rayo reflejado en la superficie
    vec3 R = reflect(-l,n);
    // Normalizamos el vector de posición del usuario
    // Eye
    vec3 E = normalize(EyeDirection_cameraspace);
    // Transición del lóbulo de la componente especular
    float cosAlpha = clamp( dot( E,R ), 0, 1 );

    // Cálculo de la componente especular
    vec4 MaterialSpecularColor =  vec4(1.0, 0.0, 1.0, 1.0) * LightPower * pow(cosAlpha,5);

    vec4 texel = texture(texture_diffuse1, TexCoords);

    // FragColor = texel * (MaterialAmbientColor + MaterialDiffuseColor + MaterialSpecularColor);
    FragColor = texel * (MaterialAmbientColor + MaterialDiffuseColor + MaterialSpecularColor);
}