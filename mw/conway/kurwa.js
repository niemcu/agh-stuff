var currentState = new Array(8);

for (var i = 0; i < 8; i++) {
	currentState[i] = new Array(8).fill(0);
}
		currentState[1][3] = 1;
		currentState[2][4] = 1;
		currentState[3][2] = 1;
		currentState[3][3] = 1;
		currentState[3][4] = 1;
		 
for (var i = 1; i < 8 -1; i++) {
			for (var j = 1; j < 8 -1; j++) {
				var x = currentState[i][j],
					m = moore(i, j);
			}
		}
		 
function moore(i, j) {
		var out = 0;
		if (i == 2 && j == 3) {
			console.log('elo');
			console.log('calosc', currentState);
			console.log('KURWA', currentState[3][2]);
			console.log(currentState[i-1][j-1]);
			console.log(currentState[i-1][j]);
			console.log(currentState[i-1][j+1]);
			console.log(currentState[i][j-1]);
			console.log(currentState[i][j+1]);
			console.log(currentState[i+1][j-1]);
			console.log(currentState[i+1][j]);
			console.log(currentState[i+1][j+1]);
			console.log('bye');
		}
		out += currentState[i-1][j-1];
		out += currentState[i-1][j];
		out += currentState[i-1][j+1];
		
		out += currentState[i][j-1];
		out += currentState[i][j+1];
		
		out += currentState[i+1][j-1];
		out += currentState[i+1][j];
		out += currentState[i+1][j+1];
		
		console.log('moore', i, j, 'to', out);

		return out;
}
