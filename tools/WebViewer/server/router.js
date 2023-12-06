const express = require('express');
const { exec } = require('child_process');
const fs = require('fs');


const router = express.Router();

let result = fs.readFileSync("a.txt", 'utf8');


router.get('/data', (req, res) => {

	console.log("data");
	res.send(result);
});

router.get('/call', (req, res) => {

	console.log("call");
	exec(`my_scalar_field2.exe 0.07`, (err, stdout, stderr) => {
		if (err) {
			console.error(`Error: ${err.message}`);
			return;
		}

	});
	res.send("callres");
});




module.exports = router;
