varying vec3 normal;
varying vec3 pos;
varying vec2 uv;

void main() {
  gl_Position = ftransform();
  normal = gl_Normal;
  pos = gl_Vertex.xyz / gl_Vertex.w;
  uv = gl_MultiTexCoord0;
}
