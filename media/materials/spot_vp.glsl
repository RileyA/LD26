varying vec3 normal;
varying vec3 ldir;
uniform vec4 lp;

void main() {
  gl_Position = ftransform();
  normal = gl_Normal;
  ldir = lp.xyz - (gl_Vertex.xyz / gl_Vertex.w);
}
