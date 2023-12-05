const express = require('express');
const fs = require('fs');


const router = express.Router();

let result = fs.readFileSync("a.txt", 'utf8');



module.exports = router;
