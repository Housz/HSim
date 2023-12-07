const express = require('express');
const { exec } = require('child_process');
const fs = require('fs');


const router = express.Router();


router.get('/call/:scale', (req, res) => {

	console.log(req.params);
	exec(`my_scalar_field2.exe ` + req.params.scale, (err, stdout, stderr) => {
		if (err) {
			console.error(`Error: ${err.message}`);
			return;
		}

	});
	res.send("callres");
});





router.get('/data2d', (req, res) => {

	console.log("data2d");
	let result = fs.readFileSync("a.txt", 'utf8');
	res.send(result);
});

router.get('/data3d', (req, res) => {

	console.log("data3d");
	let result = fs.readFileSync("a.txt", 'utf8');
	res.send(result);
});



module.exports = router;
