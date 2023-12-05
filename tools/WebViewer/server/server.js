const fs = require('fs');
const express = require('express');

const app = express();

const router = require('./router')
app.use(router);


app.use(express.static('./viewer'));

let port = 4869;
app.listen(port, () => {
	console.log('running at http://127.0.0.1:' + port );
})