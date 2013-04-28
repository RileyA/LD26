uniform sampler2D rt;
uniform sampler2D depthRt;
varying vec2 uv;
uniform vec4 invTex;

void main() {

  // take 8 samples
  float samples[8];
  samples[0] = texture2D(depthRt, uv + vec2(-invTex.x, 0)).x;
  samples[6] = texture2D(depthRt, uv + vec2(0, invTex.y)).x;
  samples[7] = texture2D(depthRt, uv + vec2(0, -invTex.y)).x;
  samples[3] = texture2D(depthRt, uv + vec2(invTex.x, 0)).x;
  samples[1] = texture2D(depthRt, uv + vec2(-invTex.x, invTex.y)).x;
  samples[2] = texture2D(depthRt, uv + vec2(-invTex.x, -invTex.y)).x;
  samples[4] = texture2D(depthRt, uv + vec2(invTex.x, invTex.y)).x;
  samples[5] = texture2D(depthRt, uv + vec2(invTex.x, -invTex.y)).x;

  float result = texture2D(depthRt, uv).x * 8 -
    (samples[0] + samples[1] + samples[2] + samples[3]
      + samples[4] + samples[5] + samples[6] + samples[7]);
  result *= 3;

  //if (result < 0.3) {
  //  result = 0.0;
  //}

  gl_FragColor = texture2D(rt, uv); 
  gl_FragColor.x += result;//(1 - clamp(result, 0, 1));
  gl_FragColor.y += result;//(1 - clamp(result, 0, 1));
  gl_FragColor.z += result;//(1 - clamp(result, 0, 1));
  //gl_FragColor.y = result;
  //gl_FragColor.z = result;
  //gl_FragColor = texture2D(depthRt, uv).rrrr;
}
