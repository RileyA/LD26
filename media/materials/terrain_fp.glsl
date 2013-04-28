varying vec3 normal;
varying vec3 pos;
uniform vec4 eye_pos; // object space

void main() {
  vec3 sun = normalize(vec3(0.5,0.5,0.3));
  float c = max(dot(normal, sun.xyz) * 0.6 + 0.4, 0.4);
  float eye_dist = length(pos - eye_pos);
  float fog = clamp((eye_dist - 150.0) / 150.0,0.0,1.0);
  gl_FragColor = mix(vec4(c, c, c, 1.0), vec4(1.0,1.0,1.0,1.0), fog) * 0.05; 
}
