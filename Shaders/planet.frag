//Point Light
#version 330 core //Version
uniform sampler2D tex0;

in vec2 texCoord;

out vec4 FragColor; //Returns a Color
//Simple shader that colors the model Red
void main()
{
    FragColor = texture(tex0, texCoord);
}