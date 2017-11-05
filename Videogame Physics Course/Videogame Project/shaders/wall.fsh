#version 330

in vec2 texCoordToFS; 
uniform sampler2D image; 
out vec4 pixelColor;

void main() {
	pixelColor = texture(image, texCoordToFS);
	if(pixelColor.x >= 0.8 && pixelColor.y >= 0.8 && pixelColor.z >= 0.8) discard;
}