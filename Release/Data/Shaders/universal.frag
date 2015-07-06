#version 130

#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D texMain;
uniform sampler2D texLight;
uniform sampler2D texShadow;
uniform sampler2D texLightMult;
uniform float boostR;
uniform float boostG;
uniform float boostB;
uniform float checkForShadow;
uniform float shadowBrightness;
uniform float ambientLight;
uniform float enableBetterLight;

void main()
{
	float texX = gl_TexCoord[0].x;
	float texY = gl_TexCoord[0].y;
		
	vec2 coord = vec2(texX, texY);
	float texR = texture(texMain, coord).r * boostR;
	float texG = texture(texMain, coord).g * boostG;
	float texB = texture(texMain, coord).b * boostB;
	vec4 boost = vec4(boostR, boostG, boostB, 1.00);
	vec4 finalColor;
	vec4 shadowColor = vec4(0, 0, 0, 0);

	float lightLevel = max(texture(texLight, coord).r, max(texture(texLight, coord).g, texture(texLight, coord).b)) * 0.80;
	
	finalColor = vec4(texR, texG, texB, 1.00);

	// Apply light
	if (enableBetterLight == 1.0)
	{
		finalColor = finalColor * 1.0 * (texture(texLight, coord) + texture(texLightMult, coord) * (ambientLight + 0.50));
	}
	else
	{
		finalColor = finalColor * 1.0 * texture(texLight, coord);
	}
	
	// Better shadows
	if (checkForShadow == 1.00)
	{
		vec4 texColor = texture(texMain, gl_TexCoord[0].xy);
		vec4 shadColor = texture(texShadow, gl_TexCoord[0].xy);
		if (abs(texColor.r - shadColor.r) > 0.02 || abs(texColor.g - shadColor.g) > 0.02 || abs(texColor.b - shadColor.b) > 0.02)
		//if (shadColor.r <= 0.20 && shadColor.g <= 0.20 && shadColor.b <= 0.20)
		{
			float md = max(0.0, min(1.0, lightLevel - ambientLight));
			float tr = min(1.0, shadowBrightness * 1.2 + md);
			shadowColor = vec4(tr, tr, tr, 1.0);
			finalColor *= shadowColor;
		}
	}

	// Flush changes	
	gl_FragColor = finalColor;
}