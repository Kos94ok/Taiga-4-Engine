#version 130

uniform sampler2D texture;
uniform float iSampleCount;
uniform float sampleOffset;
uniform float camVectorX;
uniform float camVectorY;

void main()
{
	float sampleDiv = iSampleCount * iSampleCount;
	float sampleCount = iSampleCount;
	
	vec4 sum = vec4(0);
	for (int i = 1; i <= int(sampleCount); i++)
	{
		for (int y = 1; y <= int(sampleCount); y++)
		{
			vec2 offset = vec2(-camVectorX, -camVectorY) * vec2(i, y) * sampleOffset;
			sum += texture(texture, gl_TexCoord[0].xy + offset);
		}
	}

	gl_FragColor = (sum / (sampleDiv));
}