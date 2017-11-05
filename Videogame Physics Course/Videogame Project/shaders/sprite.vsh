#version 330

in vec2 position;
in vec2 texCoord;
out vec2 texCoordToFS;
uniform mat4 projection;
uniform mat4 model;

void main() {
	gl_Position = projection * model * vec4(position, 0, 1);	
	texCoordToFS = texCoord;
}