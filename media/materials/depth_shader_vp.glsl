varying vec2 uv;

void main() {
  gl_Position = ftransform();
  uv = gl_MultiTexCoord0;
}