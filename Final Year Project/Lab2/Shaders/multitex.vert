
//
// mutlitex.vert: Vertex shader for multi-textured terrain
//

const vec3  LightDir = vec3( 0.0, 0.707, 0.707 );

varying float LightIntensity;
varying vec2 TexCoord0;
varying vec2 TexCoord1;
varying vec2 TexCoord2;
varying vec2 TexCoord3;
varying vec4 VertexColor;

void main(void)
{
    LightIntensity  = 0.3 + 0.7 * dot( gl_Normal, LightDir );
    gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;
	TexCoord0 = vec2(gl_MultiTexCoord0);
	TexCoord1 = vec2(gl_MultiTexCoord1);
	TexCoord2 = vec2(gl_MultiTexCoord2);
	TexCoord3 = vec2(gl_MultiTexCoord3);
	VertexColor = gl_Color;
}


