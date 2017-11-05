#version 330

in vec3 position;
in vec2 texCoord;
out vec2 texCoordToFS;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main() {
	gl_Position = projection * view * model * vec4(position, 1);	
	texCoordToFS = texCoord;
}