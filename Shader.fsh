//
//  Shader.fsh
//  Untitled
//
//  Created by Schell Scivally on 2/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

//varying lowp vec4 vColor;
varying highp vec2 vTexcoord;

//uniform bool textured;
uniform sampler2D sampler;

void main()
{
//	if (textured) {
		gl_FragColor = texture2D(sampler, vTexcoord);
//	} else {
//		gl_FragColor = vColor;
//	}
}
