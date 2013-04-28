varying vec2 depth;

void main()
{
	gl_Position = ftransform();
	depth.x = (gl_Position.z - 0.01) * 0.01;
	//depth.y = gl_Position.w;
}
