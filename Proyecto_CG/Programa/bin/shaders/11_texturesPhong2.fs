#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 ex_N; 
in vec3 EyeDirection_cameraspace;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPosition;
uniform vec3 lightDirection;

void main()
{    
    
    vec4 MaterialAmbientColor = vec4(0.1, 0.1, 0.1, 1.0);
    //vec3 lightDirection = vec3(0.0, -1.0, 0.0);
    //vec3 lightPosition  = vec3(0.0,10.0, 0.0);

    vec3 normTex = texture(texture_normal1, TexCoords).xyz;

    vec3 LightPosition_cameraspace = ( view*vec4(lightPosition,1.0f)).xyz;
    vec3 LightDirection_cameraspace = ( view*vec4(lightDirection,1.0f)).xyz;

    vec3 l = normalize( LightDirection_cameraspace );

    float intensity = clamp(dot(normTex, -l),0, 1);
    vec4 MaterialDiffuseColor = intensity * vec4(0.5, 0.5, 0.5, 1.0);

    // Normal de la superficie en cooridenadas de cámara
    vec3 Normal_cameraspace = ( view *model* vec4(normTex,1)).xyz; 

    // Normal a la superficie normalizada
    vec3 n = normalize( Normal_cameraspace );
    
    // Rayo reflejado en la superficie
    vec3 R = reflect(-l,n);
    // Normalizamos el vector de posición del usuario
    // Eye
    vec3 E = normalize(EyeDirection_cameraspace);

    // Transición del lóbulo de la componente especular
    float cosAlpha = clamp( dot( E,R ), 0, 1 );

    // Intensidad del punto brilloso especular
    vec4 LightPower = vec4(1.0f,1.0f,1.0f,1.0f) * 20.0f;

    // Cálculo de la componente especular
    
    vec4 MaterialSpecularColor =  vec4(0.5, 0.5, 0.5, 1.0) * pow(cosAlpha,5)*LightPower;

    vec4 texel = texture(texture_diffuse1, TexCoords);
    
    FragColor = texel * (MaterialAmbientColor + MaterialDiffuseColor+MaterialSpecularColor);
}