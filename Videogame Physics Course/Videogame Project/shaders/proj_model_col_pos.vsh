#version 330

in vec3 vertex_position;
in vec3 vertex_color;
uniform mat4 model_matrix;
uniform mat4 projection_matrix;

out vec3 color_to_fs;

void main(){
	color_to_fs = vertex_color;
	gl_Position = projection_matrix * model_matrix * vec4(vertex_position, 1);
}