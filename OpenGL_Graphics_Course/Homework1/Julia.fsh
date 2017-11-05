#version 330

out vec4 pixel_color;
uniform vec2 position_window;


void main(){
	float incre = 2.4/position_window[0];	
	float i = gl_FragCoord[0];
	float j = position_window[1] - gl_FragCoord[1] - 1;
	vec2 x = vec2(-1.2+(i*incre), -1.2+(j*incre));
	vec2 z = vec2(-0.74543, 0.11301);
	vec3 gray;
	
	int count = 0;
	while(length(x) <= 2.0 && count < 128){
		vec2 x_squared;
		x_squared[0] = x[0]*x[0] - x[1]*x[1];
		x_squared[1] = 2.0*x[0]*x[1];
		x = x_squared + z;
		count++;
	}
	
	if(length(x) <= 2.0){
		gray = vec3(0.0, 0.0, 0.0);
	}
	else{
		gray = vec3(count/128.0, count/128.0, count/128.0);
	}
	
	pixel_color = vec4(gray, 1);
}