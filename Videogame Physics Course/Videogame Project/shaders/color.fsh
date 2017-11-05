#version 330

in vec3 color_to_fs;
out vec4 pixel_color;

void main(){
	pixel_color = vec4(color_to_fs, 1);
}