#version 130

#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D texture;
uniform float iSampleCount;
uniform float sampleOffset;
uniform float camVectorX;
uniform float camVectorY;

void main()
{
	float sampleDiv = (iSampleCount * 2) * (iSampleCount * 2);
	float sampleCount = iSampleCount;
	float vecMod = max(abs(camVectorX), abs(camVectorY));
	
	vec4 sum = vec4(0);
	for (int i = 1; i < int(sampleCount * 2 + 1); i++)
	{
		for (int y = 1; y < int(sampleCount * 2 + 1); y++)
		{
			vec2 offset = vec2(camVectorX, camVectorY) * vec2(i, y) * sampleOffset;
			//vec2 offset = vec2(i, y) * sampleOffset * vecMod;
			sum += texture2D(texture, gl_TexCoord[0].xy + offset);
		}
	}

	gl_FragColor = (sum / (sampleDiv));
}