import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
// import { GUI } from 'dat.gui';
import { GUI } from 'three/addons/libs/lil-gui.module.min.js';
const gui = new GUI();
gui.add(document, 'title');

import vertexShader from './shaders/vertex.glsl.js';
import fragmentShader from './shaders/fragment.glsl.js';

const canvas = document.querySelector('#c');
const renderer = new THREE.WebGLRenderer({ antialias: true, canvas });

const fov = 75;
const aspect = 2;
const near = 0.01;
const far = 1000;
const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
camera.position.z = 80;

const controls = new OrbitControls(camera, renderer.domElement);

const scene = new THREE.Scene();
{
	const color = 0xFFFFFF;
	const intensity = 3;
	const light = new THREE.DirectionalLight(color, intensity);
	light.position.set(- 1, 2, 4);
	scene.add(light);

}



const width = 100;
const height = 100;

const geometry = new THREE.PlaneGeometry(100, 100, 100, 100);


const size = width * height;
const data = new Uint8Array(4 * size);

// for (let i = 0; i < size; i++) {

// 	const stride = i * 4;

// 	let color = new THREE.Color().setHSL(Math.random(), 0.5, 0.5);
// 	const r = Math.floor(color.r * 255);
// 	const g = Math.floor(color.g * 255);
// 	const b = Math.floor(color.b * 255);

// 	data[stride] = r;
// 	data[stride + 1] = g;
// 	data[stride + 2] = b;
// 	data[stride + 3] = 255;

// }

// const texture = new THREE.DataTexture(data, width, height);
// texture.needsUpdate = true;

let response = await fetch('/data');
if (response.ok) { // 如果 HTTP 状态码为 200-299
	// 获取 response body（此方法会在下面解释）
	let dataText = await response.text();

	let dataArray = dataText.split(/\s+/);

	let colorArray = dataArray.map((value) => mapRange(value, -1, 1, 0, 255));


	for (let i = 0; i < size; i++) {

		const stride = i * 4;

		let r = colorArray[i];

		data[stride] = r;
		data[stride + 1] = r;
		data[stride + 2] = r;
		data[stride + 3] = 255;

	}

	const texture = new THREE.DataTexture(data, width, height);
	texture.needsUpdate = true;

	let matTex = new THREE.MeshPhongMaterial({
		map: texture, 
	});
	// let matShader = new THREE.ShaderMaterial({
	// 	// wireframe: true,
	// 	vertexShader: vertexShader,
	// 	fragmentShader: fragmentShader,
	// 	uniforms: {
	// 		texture: { value: texture }
	// 	},
	// });

	const mesh = new THREE.Mesh(geometry, matTex);
	scene.add(mesh);


} else {
	alert("HTTP-Error: " + response.status);
}


const uniforms = {
	u_time: { type: 'f', value: 0.0 },
	u_resolution: {
		type: 'v2', value: new THREE.Vector2(window.innerWidth, window.innerHeight)
			.multiplyScalar(window.devicePixelRatio)
	},
	u_mouse: { type: 'v2', value: new THREE.Vector2() },
	// image: {type: 't', value: new THREE.TextureLoader().load('../nobita.jpg')}

	u_my: { type: 'f', value: 1 }
}

// const geometry = new THREE.PlaneGeometry(100, 100, 100, 100);
// const material = new THREE.ShaderMaterial({
// 	// wireframe: true,
// 	uniforms: uniforms,
// 	vertexShader: vertexShader,
// 	fragmentShader: fragmentShader
// });



let guiNode = { my_uniform: 1 };
gui.add(guiNode, 'my_uniform', 0, 1).onChange(() => {
	material.uniforms.u_my.value = guiNode.my_uniform;
});

// let matTex = new THREE.MeshPhongMaterial({
// 	map: texture, 
// });

// const mesh = new THREE.Mesh(geometry, matTex);
// scene.add(mesh);


function mapRange(value, fromMin, fromMax, toMin, toMax) {
	return (value - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}







function resizeRendererToDisplaySize(renderer) {

	const canvas = renderer.domElement;
	const pixelRatio = window.devicePixelRatio;
	const width = canvas.clientWidth * pixelRatio | 0;
	const height = canvas.clientHeight * pixelRatio | 0;
	const needResize = canvas.width !== width || canvas.height !== height;
	if (needResize) {

		renderer.setSize(width, height, false);

	}

	return needResize;

}

function render() {

	// console.log(my_uniform);

	if (resizeRendererToDisplaySize(renderer)) {

		const canvas = renderer.domElement;
		camera.aspect = canvas.clientWidth / canvas.clientHeight;
		camera.updateProjectionMatrix();

	}

	renderer.render(scene, camera);

	requestAnimationFrame(render);

}

requestAnimationFrame(render);
