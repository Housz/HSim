const express = require('express');
const fs = require('fs');


const router = express.Router();

let result = fs.readFileSync("a.txt", 'utf8');


router.get('/data', (req, res) => {

	console.log("data");
	res.send(result);
});



module.exports = router;
