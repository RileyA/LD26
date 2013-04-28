varying vec2 depth;

void main()
{
	float finalDepth = depth.x;//(depth.x / depth.y);
	gl_FragColor = vec4(finalDepth, finalDepth, finalDepth, 1);
}

