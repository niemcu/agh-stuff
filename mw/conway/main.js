// autor: ympeg
// konwencja: wszystko camelcase
var opt = {
	cellSize: 20, 
	cellsPerRow: 30,
	rowsCount: 10
};

var elements = {};

var currentRow = new Array(opt.cellsPerRow + 2).fill(0);
var x = Math.floor(opt.cellsPerRow / 2);

currentRow[x] = 1;

var ruleTable = {};


function setRule(rule = 90) {
	var bin = dec2bin(rule, 8),
		len = bin.length;
	for (var i = 0; i < len; i++) {
		var index = dec2bin(i, 3);
		ruleTable[index] = bin[len - i-1];
	}
}

function buildCanvas() { console.log('buildCanvas');
	var canvas = document.createElement('canvas');
	
	var cnt = document.getElementById('canvas-container');
	
	canvas.width  = opt.cellSize * opt.cellsPerRow;
	canvas.height = opt.cellSize * opt.rowsCount;
	
	cnt.appendChild(canvas);
	
	elements.canvas = canvas;
	
}

function applyGrid () {
	var ctx = elements.canvas.getContext('2d'),
		  w = elements.canvas.width,
		  h = elements.canvas.height,
		  i = 0,
		  max = Math.max(w, h);
	
	
	for (; i < max; i += opt.cellSize) {
		ctx.beginPath();
		ctx.moveTo(i, 0);
		ctx.lineTo(i, h);
		ctx.stroke();

		ctx.beginPath();
		ctx.moveTo(0, i);
		ctx.lineTo(w, i);
		ctx.stroke();
	}
} 

function drawCurrentRow(cursor) {
	var ctx = elements.canvas.getContext('2d');
	
	for (var i = 1; i <= opt.cellsPerRow; i++) {
		if (currentRow[i] != 0) {
			ctx.fillStyle = '#D3AC75';
			ctx.fillRect((i-1) * opt.cellSize, cursor * opt.cellSize, opt.cellSize, opt.cellSize);
		}
	}
}

// na razie przyjmujemy ze jest 0
function setBoundary(val = 0) {
	
}

function dec2bin (dec, length) {
  var out = "";
  while(length--) out += (dec >> length ) & 1;    
  return out;
}

function recalculate() {
	var nextRow = currentRow.slice(0);
	//	console.log('next', nextRow);
	for (var i = 1; i <= opt.cellsPerRow; i++) {
		var res = "";
		res += currentRow[i-1];
		res += currentRow[i];
		res += currentRow[i+1];
		nextRow[i] = parseInt(ruleTable[res]);
	}
	currentRow = nextRow;
}


buildCanvas();
setRule();

for (var i = 0; i < opt.rowsCount; i++) {
	console.log('curr', currentRow);
	drawCurrentRow(i);
	applyGrid();
	recalculate();
}

// for iteracje
// recalculate(); drawCurrentRow();

// var curr_field = new Field(canvas_width, canvas_height, cell_size)
// var next_field = new Field(canvas_width, canvas_height, cell_size)
// curr_field


// user podaje:
// ilosc komorek w rzedzie: x; czyli cell_size= cvs.width / x;
// ilosc iteracji (rzedow)
// rule 
// (optional) wyklikanie stanu poczatkowego
/* function init(width = 10) {
	var cvs = document.getElementById('game-canvas'),
			ctx = cvs.getContext('2d'),
			cell_size = Math.floor(cvs.width / width),
			field = new Array(cell_size),
			height = Math.floor(cvs.height / cell_size);
	
	ctx.clearRect(0, 0, cvs.width, cvs.height);				
	
	// init field
	for (var i = 0; i < height; i++) {
		field[i] = new Array(height);
		for (var j = 0; j < width; j++) {
			field[i][j] = Math.round(Math.random());
			// field[i][j] = 0;
		}
	}

	for (var i = 0; i < height; i++) {
		for (var j = 0; j < width; j++) {
			if (field[i][j] != 0) {
				ctx.fillStyle = '#D3AC75';
				ctx.fillRect(i * cell_size, j * cell_size, cell_size, cell_size);
			}
		}
	}
	console.log(dec2bin(90, 8));
	setRule(dec2bin(90, 8));
	console.log("elo");
	console.log(cvs.width, cvs.height);
	
	applyGrid(cvs, cell_size);

	
	var counter = 0;

	document.getElementById('baton').addEventListener('click', function () {
		console.log(counter);
		console.log('pole', field);
		recalculate(field[counter]);
		counter++;
	}, false);
	
}

var rule_table = [];
	// "111", "110", "101", "100", "011", "010", "001", "000"
	 // 0     1       0        1    1      0      1     0  = 90
function setRule(rule) {
	for(var i = 0; i < rule.length; i++) {
		// console.log(rule[i]);
		rule_table[i] = rule[i];
	}
}

// 250 ma dzialac

function recalculate(row, rule = 0) {
	console.log(rule_table);
	console.log(row);
	for (var i = 1; i < row.length; i++) {
		var val = 0;
		val += row[i-1] * 1;
		val += row[i]   * 2;
		val += row[i+1] * 4;
		// console.log('val', val);
		// console.log(rule_table[val]);
	}
}

document.addEventListener('DOMContentLoaded', function() {
	
	init();
	
	document.getElementById('cells-apply').addEventListener('click', function () {
		var num = document.getElementById('amount').value;
		init(num);
		
	}, false);
});

 */
 
 
 // "111", "110", "101", "100", "011", "010", "001", "000"
	 // 0     1       0        1    1      0      1     0  = 90


