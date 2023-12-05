const fragmentShader =/*glsl*/`

varying vec3 vPosition;
varying vec3 vNormal;
flat varying vec2 vUV;

uniform float u_my;


void main() {
	
	gl_FragColor = vec4( u_my, 0.0, 0.0, 1.0 );
	// gl_FragColor = vec4( vUv.xxx, 1.0 );
	// gl_FragColor = vec4( vUV.x, vUV.y, vUV.x, 1.0 );


	
}`;

export default fragmentShader;