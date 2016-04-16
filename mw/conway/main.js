// autor: ympeg
// konwencja: wszystko camelcase
var opt = {
	cellSize: 10, 
	cellsPerRow: 150,
	rowsCount: 60,
	rule: 150,
	grid: true
};

var elements = {};
var currentRow;
var ruleTable = {};

function setup() {
	currentRow = new Array(opt.cellsPerRow + 2).fill(0);
	var x = Math.floor(opt.cellsPerRow / 2);
	currentRow[x] = 1;
}

function clear() {
	elements.canvasContainer.removeChild(elements.canvas);
}

function run() {
	setup();
	buildCanvas();
	setRule();
	setBoundary(0);

	for (var i = 0; i < opt.rowsCount; i++) {
		drawCurrentRow(i);
		recalculate();
	}
	
	if (opt.grid) applyGrid();
}

run();

window.onresize = function () {
	clear();
	run();
}

function buildCanvas() {
	var canvas = document.createElement('canvas');
	
	var cnt = document.getElementById('canvas-container');
	
	//canvas.width  = opt.cellSize * opt.cellsPerRow;
	canvas.width = cnt.offsetWidth;
	opt.cellSize = canvas.width / opt.cellsPerRow;
	canvas.height = opt.cellSize * opt.rowsCount;
	
	cnt.appendChild(canvas);
	
	elements.canvas = canvas;
	elements.canvasContainer = cnt;
}

function setRule() {
	var bin = dec2bin(opt.rule, 8),
		len = bin.length;
	for (var i = 0; i < len; i++) {
		var index = dec2bin(i, 3);
		ruleTable[index] = bin[len - i-1];
	}
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
	currentRow[0] = val;
	currentRow[opt.cellsPerRow + 1] = val;
}

function dec2bin (dec, length) {
  var out = "";
  while(length--) out += (dec >> length ) & 1;    
  return out;
}

function recalculate() {
	var nextRow = currentRow.slice(0);
	//	3.log('next', nextRow);
	for (var i = 1; i <= opt.cellsPerRow; i++) {
		var res = "";
		res += currentRow[i-1];
		res += currentRow[i];
		res += currentRow[i+1];
		nextRow[i] = parseInt(ruleTable[res]);
	}
	currentRow = nextRow;
}

// why dis must be so ugly yhhhhh
function setupPanel() {
	elements.cellsAmount = document.getElementById('cells-amount');
	elements.cellsAmount.value = opt.cellsPerRow;
	
	document.getElementById('cells-plus').addEventListener('click', function () {
		elements.cellsAmount.value++;
		opt.cellsPerRow = parseInt(elements.cellsAmount.value);
		clear();
		run();
	}, false);

	document.getElementById('cells-minus').addEventListener('click', function () {
		elements.cellsAmount.value--;
		opt.cellsPerRow = parseInt(elements.cellsAmount.value);
		clear();
		run();
	}, false);
	
	elements.rule = document.getElementById('rule');
	elements.rule.value = opt.rule;
	
	document.getElementById('rule-plus').addEventListener('click', function () {
		elements.rule.value++;
		opt.rule = parseInt(elements.rule.value);
		clear();
		run();
	}, false);

	document.getElementById('rule-minus').addEventListener('click', function () {
		elements.rule.value--;
		opt.rule = parseInt(elements.rule.value);
		clear();
		run();
	}, false);
	
	elements.iterations = document.getElementById('iterations');
	elements.iterations.value = opt.rowsCount;
	
	document.getElementById('iter-plus').addEventListener('click', function () {
		elements.iterations.value++;
		opt.rowsCount = parseInt(elements.iterations.value);
		clear();
		run();
	}, false);

	document.getElementById('iter-minus').addEventListener('click', function () {
		elements.iterations.value--;
		opt.rowsCount = parseInt(elements.iterations.value);
		clear();
		run();
	}, false);
	
	elements.grid = document.getElementById('grid');
	
	elements.grid.addEventListener('change', function () {
		//console.log('grid przed', opt.grid);
		elements.grid.checked = opt.grid = !opt.grid;
		//console.log('grid po', opt.grid);
		clear();
		run();
	}, false);
	
}
// use it to optimize?
function clearCanvas() {
	var ctx = elements.canvas.getContext('2d');
	ctx.clearRect(0, 0, elements.canvas.width, elements.canvas.height);
}

setupPanel();




// (optional) wyklikanie stanu poczatkowego

/* 	document.getElementById('baton').addEventListener('click', function () {
		console.log(counter);
		console.log('pole', field);
		recalculate(field[counter]);
		counter++;
	}, false);
	
}
document.addEventListener('DOMContentLoaded', function() {
	
	init();
	
	document.getElementById('cells-apply').addEventListener('click', function () {
		var num = document.getElementById('amount').value;
		init(num);
		
	}, false);
}); */
