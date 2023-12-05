const fragmentShader =/*glsl*/`

varying vec3 vPosition;
varying vec3 vNormal;
flat varying vec2 vUV;

uniform sampler2D texture;



void main() {

	vec4 texelColor = texture2D(texture, vUV); // 用贴图坐标获取颜色
	gl_FragColor = texelColor;

	// vec4 color = texture( diffuse, vec3( vUV, depth ) );
	
	// gl_FragColor = vec4( u_my, 0.0, 0.0, 1.0 );
	// gl_FragColor = vec4( vUV.xxx, 1.0 );
	// gl_FragColor = vec4( vUV.x, vUV.y, vUV.x, 1.0 );


	
}`;

export default fragmentShader;