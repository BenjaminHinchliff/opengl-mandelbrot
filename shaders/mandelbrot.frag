#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 texCoord;

uniform vec2 resolution;
uniform int max_iter;
uniform sampler1D palette;

void square_plus_c(inout vec2 z, in vec2 c)
{
	float tmpX = z.x;
	z.x = z.x * z.x - z.y * z.y;
	z.y = 2.0 * tmpX * z.y;
	z += c;
}

int mandelbrot(in vec2 point, out vec2 z)
{
	z = vec2(0.0, 0.0);
	for (int i = 0; i < max_iter; ++i)
	{
		if (length(z) > 20.0)
		{
			return i;
		}
		square_plus_c(z, point);
	}
	return max_iter;
}

const float ln_2_0 = log(2.0); 

float mandelbrot_renorm(in vec2 point)
{
	vec2 z;
	int iters = mandelbrot(point, z);
	// short circut if it's within set
	if (iters == max_iter) return iters;
	// steps to narrow errors
	square_plus_c(z, point);
	square_plus_c(z, point);
	float mu = iters + 1 - log(log(length(z))) / ln_2_0;
	return mu;
}

void main()
{
	float aspect = resolution.x / resolution.y;
	vec2 position;
	position.x = aspect * texCoord.x;
	position.y = texCoord.y;
	position *= 2;
	position -= vec2(1.5, 1.0);
	position += vec2(-0.5, 0.0);
	// position /= 1.0 / 2.0;
	float mu = mandelbrot_renorm(position);
	float iter = mu / max_iter;
	FragColor = texture(palette, iter);
}
