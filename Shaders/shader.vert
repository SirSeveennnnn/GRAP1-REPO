#version 330 core //Version Number

//the position variable has attribute position 0
//Accesses the vertex position and assigns it to aPos
layout(location = 0) in vec3 aPos; 

//Creates a Transform variable
//We'll assign the Transformation matrix here
uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

layout(location = 1) in vec3 vertexNormal;
out vec3 normCoord;
out vec3 fragPos;

layout(location = 2) in vec2 aTex;
out vec2 texCoord;

layout(location = 3) in vec3 m_tan;
layout(location = 4) in vec3 m_btan;

out mat3 TBN;

void main()
{
    texCoord = aTex;

    //gl_Position is predefined
    //This is the final position of the matrix when drawn
    gl_Position =   projection * view*
                    transform * //Multiply the matrix with the position
                    vec4(aPos, 1.0); // Turns our 3x1 matrix into a 4x1

    mat3 modelMatrix = mat3(transpose(inverse(transform)));
    normCoord = modelMatrix * vertexNormal;

    vec3 T = normalize(modelMatrix * m_tan);
    vec3 B = normalize(modelMatrix * m_btan);
    vec3 N = normCoord;

    TBN = mat3(T,B,N);

    fragPos = vec3(transform * vec4(aPos,1.0));


}