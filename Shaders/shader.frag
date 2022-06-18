#version 330 core //Version

uniform sampler2D tex0;
in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;
in mat3 TBN;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

uniform float brightness;
uniform bool pointLight;
uniform vec3 directional;

uniform sampler2D norm_tex;

out vec4 FragColor; //Returns a Color

void main()
{
    if(pointLight){
    float distance = length(lightPos - fragPos); //Calculate the distance between the light source and obj
    float intensity = brightness / (pow(distance,2)); // calculate intensity  

    //vec3 normal = normalize(normCoord);
    vec3 normal = texture(norm_tex, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(normal,lightDir), 0.0);
    vec3 diffuse = diff * intensity * lightColor; //multiply intensity

    vec3 ambientCol = ambientColor * ambientStr;// * intensity; //multiply intensity

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
    vec3 specColor = spec * specStr * intensity * lightColor;  //multiply intensity


    FragColor =  vec4((specColor + ambientCol + diffuse) ,1.0) * (texture(tex0, texCoord));
    }
    else{
    vec3 normal = texture(norm_tex, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    vec3 lightDir = directional;

    float diff = max(dot(normal,lightDir), 0.0);
    vec3 diffuse = diff * lightColor; //multiply intensity

    vec3 ambientCol = ambientColor * ambientStr;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
    vec3 specColor = spec * specStr * lightColor;  //multiply intensity


    FragColor =  vec4((specColor + ambientCol + diffuse) ,1.0) * (texture(tex0, texCoord));
    }

    


}