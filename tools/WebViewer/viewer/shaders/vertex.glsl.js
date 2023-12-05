const vertexShader =/*glsl*/`

varying vec3 vPosition;
varying vec3 vNormal;
flat varying vec2 vUV;



void main() {

	vPosition = position;
	vNormal = normal;
	vUV = uv;

	gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );
}`;


export default vertexShader;