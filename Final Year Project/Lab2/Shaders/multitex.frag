//
// multitex.frag - simple fragment shader for multitexturing terrain
//


varying float LightIntensity; 
varying vec4 VertexColor;
varying vec2 TexCoord0;
varying vec2 TexCoord1;
varying vec2 TexCoord2;
varying vec2 TexCoord3;

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform sampler2D Tex3;

void main (void)
{
	vec4 texcol0 = texture2D( Tex0, TexCoord0 );
	vec4 texcol1 = texture2D( Tex1, TexCoord1 );
	vec4 texcol2 = texture2D( Tex2, TexCoord2 );
	vec4 texcol3 = texture2D( Tex3, TexCoord3 );

	vec4 texColor = texcol0 * VertexColor.x + texcol1 * VertexColor.y + texcol2 * VertexColor.z + texcol3 * VertexColor.w;

    gl_FragColor = texColor * LightIntensity;

}
