varying vec3 normal;
varying vec3 pos;
uniform vec4 eye_pos; // object space
uniform sampler2D diffuseMap;
varying vec2 uv;

void main() {
  //vec3 sun = normalize(vec3(0.5,0.5,0.3));
  //float c = max(dot(normal, sun.xyz), 0);

  float eye_dist = length(pos - eye_pos);
  float fog = clamp((eye_dist - 150.0) / 150.0,0.0,1.0);

  gl_FragColor = mix(texture2D(diffuseMap, uv) * 0.8, vec4(1.0,1.0,1.0,1.0), fog); 
}
