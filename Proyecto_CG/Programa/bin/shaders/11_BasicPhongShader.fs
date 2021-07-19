#version 330 core

#define num_luces 2

out vec4 FragColor;

in vec2 TexCoords;
in vec3 ex_N; 

in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace[num_luces];
in vec3 Normal_cameraspace;

uniform sampler2D texture_diffuse1;

// Propiedades de la luz
uniform vec4 LightColor_0;
uniform vec4 LightPower_0;
uniform int  alphaIndex_0;
uniform float distance_0;
uniform vec4 LightColor_1;
uniform vec4 LightPower_1;
uniform int  alphaIndex_1;
uniform float distance_1;

uniform vec4 MaterialAmbientColor;
uniform vec4 MaterialDiffuseColor;
uniform vec4 MaterialSpecularColor;
uniform float transparency;

void main()
{    
    // textura
    vec4 texel = texture(texture_diffuse1, TexCoords);

    vec4 MaterialAmbColor = MaterialAmbientColor * MaterialDiffuseColor;
    vec3 n = normalize( Normal_cameraspace );
    vec3 E = normalize(EyeDirection_cameraspace);
    vec4 ex_color = vec4(0.0);
   
    vec4 LightColor[2];
    LightColor[0]=LightColor_0;
    LightColor[1]=LightColor_1;
    vec4 LightPower[2];
    LightPower[0]=LightPower_0;
    LightPower[1]=LightPower_1;
    int  alphaIndex[2];
    alphaIndex[0]=alphaIndex_0;
    alphaIndex[1]=alphaIndex_1;
    float distance[2];
    distance[0]=distance_0;
    distance[1]=distance_1;

    for(int i=0; i < num_luces ; i++){

        vec3 l = normalize( LightDirection_cameraspace[i] );
        float cosTheta = clamp( dot( n,l ), 0,1 );
        vec3 R = reflect(-l,n);
        float cosAlpha = clamp( dot( E,R ), 0,1 );

        ex_color += MaterialAmbColor +
                    MaterialDiffuseColor * LightColor[i] * LightPower[i] * cosTheta / (distance[i]*distance[i]) +
                    MaterialSpecularColor * LightColor[i] * LightPower[i] * pow(cosAlpha,alphaIndex[i]) / (distance[i]*distance[i]);

    }

    ex_color.a = transparency;

    FragColor = texel * ex_color;
}