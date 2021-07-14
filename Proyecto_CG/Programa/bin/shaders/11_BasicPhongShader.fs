#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 ex_N; 

in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 Normal_cameraspace;

uniform sampler2D texture_diffuse1;
uniform vec4 LightColor;
uniform vec4 LightPower;
uniform int  alphaIndex;
uniform float distance;

uniform vec4 MaterialAmbientColor;
uniform vec4 MaterialDiffuseColor;
uniform vec4 MaterialSpecularColor;
uniform float transparency;

void main()
{    

    // Cálculo de componente difusa
    vec3 n = normalize( Normal_cameraspace );
    vec3 l = normalize( LightDirection_cameraspace );

    vec4 texel = texture(texture_diffuse1, TexCoords);
    
    float cosTheta = clamp( dot( n,l ), 0,1 );

    // Cálculo de componente difusa
    vec4 MaterialAmbColor = MaterialAmbientColor * MaterialDiffuseColor;

    // Cálculo de componente especular
    vec3 E = normalize(EyeDirection_cameraspace);
    vec3 R = reflect(-l,n);
    float cosAlpha = clamp( dot( E,R ), 0,1 );

    vec4 ex_color = MaterialAmbColor +
                    MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
                    MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,alphaIndex) / (distance*distance);
    ex_color.a = transparency;

    FragColor = texel * ex_color;
}