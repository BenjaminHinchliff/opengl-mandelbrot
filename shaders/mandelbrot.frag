#version 330 core
out vec4 FragColor;

in vec2 texCoord;

//uniform vec2 center;
//uniform float scale;
//uniform int iter;
uniform vec2 resolution;
uniform int max_iter;

int get_iter(vec2 point) {
	vec2 a = vec2(0.0, 0.0);
	for (int i = 0; i < max_iter; i++) {
		if (length(a*a) > 4.0)
			return i;
		vec2 tmp = a;
		a.x = a.x*a.x - a.y*a.y;
		a.y = 2.0*tmp.x*tmp.y;
		a += point;
	}
	return 0;
}

void main()
{
	float aspect = resolution.x / resolution.y;
	vec2 position;
	position.x = aspect * texCoord.x;
	position.y = texCoord.y;
	position -= vec2(1.0, 0.5);
	position *= 2;
	int iter = get_iter(position);
	FragColor = vec4(float(iter) / max_iter, 0.0, 0.0, 1.0);
}
