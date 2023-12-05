import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
// import { GUI } from 'dat.gui';
import { GUI } from 'three/addons/libs/lil-gui.module.min.js';
const gui = new GUI();
gui.add( document, 'title' );

import vertexShader from './shaders/vertex.glsl.js';
import fragmentShader from './shaders/fragment.glsl.js';

const canvas = document.querySelector( '#c' );
const renderer = new THREE.WebGLRenderer( { antialias: true, canvas } );

const fov = 75;
const aspect = 2; 
const near = 0.01;
const far = 1000;
const camera = new THREE.PerspectiveCamera( fov, aspect, near, far );
camera.position.z = 80;

const controls = new OrbitControls( camera, renderer.domElement );

const scene = new THREE.Scene();
{
	const color = 0xFFFFFF;
	const intensity = 3;
	const light = new THREE.DirectionalLight( color, intensity );
	light.position.set( - 1, 2, 4 );
	scene.add( light );

}


const uniforms = {
	u_time: {type: 'f', value: 0.0},
	u_resolution: {type: 'v2', value: new THREE.Vector2(window.innerWidth, window.innerHeight)
					.multiplyScalar(window.devicePixelRatio)},
	u_mouse: {type: 'v2', value: new THREE.Vector2()},
	// image: {type: 't', value: new THREE.TextureLoader().load('../nobita.jpg')}

	u_my: {type: 'f', value: 1}
}

const geometry = new THREE.PlaneGeometry(100, 100, 100, 100);
const material = new THREE.ShaderMaterial({
	// wireframe: true,
	uniforms: uniforms,
	vertexShader: vertexShader,
	fragmentShader: fragmentShader
});

let guiNode = { my_uniform: 1 };
gui.add( guiNode, 'my_uniform', 0, 1 ).onChange(()=>{
	material.uniforms.u_my.value = guiNode.my_uniform;
});

console.log(geometry);
console.log(material);



const mesh = new THREE.Mesh(geometry, material);
scene.add(mesh);


function resizeRendererToDisplaySize( renderer ) {

	const canvas = renderer.domElement;
	const pixelRatio = window.devicePixelRatio;
	const width = canvas.clientWidth * pixelRatio | 0;
	const height = canvas.clientHeight * pixelRatio | 0;
	const needResize = canvas.width !== width || canvas.height !== height;
	if ( needResize ) {

		renderer.setSize( width, height, false );

	}

	return needResize;

}

function render(  ) {

	// console.log(my_uniform);

	if ( resizeRendererToDisplaySize( renderer ) ) {

		const canvas = renderer.domElement;
		camera.aspect = canvas.clientWidth / canvas.clientHeight;
		camera.updateProjectionMatrix();

	}

	renderer.render( scene, camera );

	requestAnimationFrame( render );

}

requestAnimationFrame( render );