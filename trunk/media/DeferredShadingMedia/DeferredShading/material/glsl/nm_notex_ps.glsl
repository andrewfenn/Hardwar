/******************************************************************************
Copyright (c) W.J. van der Laan

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software  and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so, subject 
to the following conditions:

The above copyright notice and this permission notice shall be included in all copies 
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/
/** Deferred shading framework
	// W.J. :wumpus: van der Laan 2005 //
	
	Material shader: Single colour normal mapped
*/

#extension GL_ARB_draw_buffers : enable

varying vec3 normal;
varying vec3 tangent_;
varying vec3 binormal;

varying vec2 texCoord0;
varying float depth;

uniform sampler2D normTex;

uniform float specularity;
uniform vec3 colour;

void main()                    
{
	// Frame for normal mapping
	mat3 rotation = mat3(tangent_, binormal, normal);
	vec3 texnormal = (vec3(texture2D(normTex, texCoord0))-0.5)*2.0;
	
   	gl_FragData[0].rgb = colour;
	gl_FragData[0].a = specularity;
	// rotation*texnormal is reversed for GL because matrices are stored transposed internally
	// compared to HLSL
	gl_FragData[1].xyz = normalize(rotation*texnormal); // Do normalisation here, saves an operation per light
	gl_FragData[1].w = depth;
	
}
