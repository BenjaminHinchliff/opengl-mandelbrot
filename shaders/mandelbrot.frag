#version 330 core
out vec4 FragColor;

in vec2 texCoord;

//uniform vec2 center;
//uniform float scale;
//uniform int iter;
uniform vec2 resolution;
uniform int max_iter;
uniform sampler1D palette;

int mandelbrot(vec2 point)
{
	vec2 z = vec2(0.0, 0.0);
	for (int i = 0; i < max_iter; i++)
	{
		if (length(z * z) > 4.0)
		{
			return i;
		}
		float tmpX = z.x;
		z.x = z.x * z.x - z.y * z.y;
		z.y = 2.0 * tmpX * z.y;
		z += point;
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
	int iter = mandelbrot(position);
	FragColor = texture(palette, float(iter) / max_iter);
}
