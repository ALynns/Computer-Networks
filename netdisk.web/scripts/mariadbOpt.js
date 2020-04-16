function getConnect(){
	var mysql = require('mysql');

	var connection = mysql.createConnection({
		host	:'localhost',
		user	:'u1751763',
		password:'u1751763',
		database:'db1751763'
	});
	return connection;
}