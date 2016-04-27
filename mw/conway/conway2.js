document.addEventListener('DOMContentLoaded', prepare, false);

var opt = {
	rows: 20,
	cols: 20,
	cellSize: 10,
	fillColor: '#D3AC75'
};

var elements = {};

var currState, nextState;

function clearCanvas() {
	
}



function drawField () { console.log('draw');
	var ctx = elements.canvas.getContext('2d');
	
	ctx.clearRect(0, 0, elements.canvas.width, elements.canvas.height);
	
	ctx.fillStyle = opt.fillColor;
	for (var i = 0; i < opt.rows; i++) {
		for (var j = 0; j < opt.cols; j++) {
			if (currState[i][j] != 0) {
				ctx.fillRect(j * opt.cellSize, i * opt.cellSize, opt.cellSize, opt.cellSize);
				//console.log('komorka zywa, moore', i, j, moore(i, j));
			}
		}
	}
}

function buildCanvas() {
	var canvas = document.createElement('canvas');
	
	var cnt = document.getElementById('canvas-container');
	
	canvas.width = cnt.offsetWidth;
	opt.cellSize = canvas.width / opt.cols;
	canvas.height = opt.cellSize * opt.rows;
	
	cnt.appendChild(canvas);
	
	elements.canvas = canvas;
	elements.canvasContainer = cnt;
}

function initField() {
	 currState = new Array(opt.rows);
	 for (var i = 0; i < opt.rows; i++) {
		 currState[i] = new Array(opt.cols).fill(0);
	 }
}

function recalc() {
	nextState = new Array(opt.rows);
	for (var i = 0; i < opt.rows; i++) {
		 nextState[i] = new Array(opt.cols).fill(0);
	}
	// i rowne 1 bo warunki nieperiod.
	for (var i = 1; i < opt.rows - 1; i++) {
		for (var j = 1; j < opt.cols - 1; j++) {
			var c = currState[i][j],
				m = moore(i, j);
			
			if (c == 0) {
				if (m == 3) nextState[i][j] = 1;
			}
			
			if (c == 1) {
				if (m == 2 || m == 3) {
					nextState[i][j] = 1;
				} else {
					nextState[i][j] = 0;
				}
			}
			/* 
			if (c == 0 && m == 3) {
				nextState[i][j] = 1;
				console.log('dla kom', i, j, 'rodzimy sie');
			} else {
				nextState[i][j] = 0;
				console.log('dla kom', i, j, 'zostajemy niezywi');
			}
			
			if (c == 1 && (m == 2 || m == 3) ) {
				nextState[i][j] = 1;
				console.log('dla kom', i, j, 'zostajemy zywi');
			} else {
				nextState[i][j] = 0;
				console.log('dla kom', i, j, 'zdychamy z glodu');
			} */
		}
	}
	
	currState = nextState;
}

function prepare() {
	initField();
	buildCanvas();
	putGlider();
	drawField();
	setInterval(recalc, 100);
	setInterval(drawField, 50);
	console.log(currState);
}

function moore(i, j) {
	var out = 0;
	
	out += currState[i-1][j-1];
	out += currState[i-1][j];
	out += currState[i-1][j+1];
		
	out += currState[i][j-1];
	out += currState[i][j+1];
		
	out += currState[i+1][j-1];
	out += currState[i+1][j];
	out += currState[i+1][j+1];
	
	return out;
}

