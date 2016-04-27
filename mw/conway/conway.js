// autor: ympeg
// konwencja: wszystko camelcase
window.requestAnimFrame = (function(){
	return  window.requestAnimationFrame       ||
          window.webkitRequestAnimationFrame ||
          window.mozRequestAnimationFrame    ||
          function( callback ){
            window.setTimeout(callback, 1000 / 60);
          };
})();

var GameOfLife = (function() {
	
	var opt = {
		cellSize: 10, 
		rule: 150,
		grid: false,
		boundary: 0,
		fillColor: '#D3AC75',
		rows: 50,
		cols: 70,
		periodic: true,
		speed: 50,
		timer: null,
		generation: 0,
		imageMode: false
	};

	var dom = {};
	var ruleTable = {};
	var currentRow;
	
	var currentState = new Array(opt.rows);
	
	function setup() {
		/* currentRow = new Array(opt.cols + 2).fill(0);
		var x = Math.floor(opt.cols / 2);
		currentRow[x] = 1; */
		
		for (var i = 0; i < opt.rows; i++) {
			currentState[i] = new Array(opt.cols).fill(0);
		}
		
		randomizeState();
	}
	// czy jest jakis piekny sposob na to zeby nie duplikowac tak tego kodu?
	function putGlider() {
		processEachCell(function (i, j) {
			currentState[i][j] = 0;
		});
		
		currentState[1][3] = 1;
		currentState[2][4] = 1;
		currentState[3][2] = 1;
		currentState[3][3] = 1;
		currentState[3][4] = 1;
		
		drawField();
	}
	
	function putPond () {
		processEachCell(function (i, j) {
			currentState[i][j] = 0;
		});
		
		currentState[5][6] = 1;
		currentState[5][5] = 1;
		currentState[6][7] = 1;
		currentState[7][7] = 1;
		currentState[8][6] = 1;
		currentState[8][5] = 1;
		currentState[7][4] = 1;
		currentState[6][4] = 1;
		
		drawField();
	}
	
	function putBeeQueen () {
		processEachCell(function (i, j) {
			currentState[i][j] = 0;
		});
		
		currentState[15][15] = 1;
		currentState[16][15] = 1;
		currentState[17][16] = 1;
		currentState[18][16] = 1;
		currentState[19][16] = 1;
		currentState[20][15] = 1;
		currentState[21][15] = 1;
		currentState[16][17] = 1;
		currentState[20][17] = 1;
		currentState[17][18] = 1;
		currentState[19][18] = 1;
		currentState[18][19] = 1;
		
		drawField();		
	}
	
	function putFrog() {
		processEachCell(function (i, j) {
			currentState[i][j] = 0;
		});
		
		currentState[5][5] = 1;
		currentState[6][5] = 1;
		currentState[7][5] = 1;
		currentState[6][6] = 1;
		currentState[7][6] = 1;
		currentState[8][6] = 1;
		
		drawField();
	}
	
	
	
	
	
	
	function putInfinite() {
		currentState[1][7] = 1;
		currentState[2][5] = 1;
		currentState[2][7] = 1;
		currentState[2][8] = 1;
		currentState[3][5] = 1;
		currentState[3][7] = 1;
		currentState[4][5] = 1;			
		currentState[5][3] = 1;			
		currentState[6][1] = 1;			
		currentState[6][3] = 1;			
	}
	
	function randomizeState() {
		processEachCell(function (i, j) {
				currentState[i][j] = Math.round(Math.random());
		});
	}
	
	function processEachCell(callback) {
		for (var i = 0; i < opt.rows; i++) {
			for (var j = 0; j < opt.cols; j++) {
				// if typeof
				callback(i, j);
			}
		}
	}
	
	function getMousePos(event) {
		var rect = dom.canvas.getBoundingClientRect();
        return {
          x: Math.floor((event.clientX - rect.left) / (rect.right - rect.left) * dom.canvas.width),
		  y: Math.floor((event.clientY - rect.top) / (rect.bottom - rect.top) * dom.canvas.height)
        };
	}
	
	function handleClick(event) {
		var pos = getMousePos(event);
		
		if (opt.imageMode) {
			dom.ctx.drawImage(opt.img, pos.x, pos.y);
			opt.imageMode = false;
		} else {
			var i = Math.floor(pos.x / opt.cellSize),
				j = Math.floor(pos.y / opt.cellSize);
			
			if (currentState[j][i] == 1) {
				currentState[j][i] = 0;
			} else {
				currentState[j][i] = 1;
			}
			drawField();	
		}
	}

	function clear() {
		dom.canvasContainer.removeChild(dom.canvas);
	}
	
	//document.addEventListener('keyup', step, false);

	document.addEventListener('keyup', function (e) {
		if (e.keyCode == 81) {
			recalculate();
			drawField();
		}
	}, false);
	
	run();
	
	function run() {
		buildCanvas();
		setup();
		
		dom.canvas.addEventListener('click', handleClick, false);
		
		drawField();
	}
	// todo REFACTOR
	function drawField () { //console.log('draw');
		var ctx = dom.canvas.getContext('2d');
		clearCanvas();
		ctx.fillStyle = opt.fillColor;
		processEachCell(function (i, j) {
			if (currentState[i][j] != 0) {
				ctx.fillRect(j * opt.cellSize, i * opt.cellSize, opt.cellSize, opt.cellSize);
			}
		});
		
		if (opt.grid) applyGrid();		
	}

	function buildCanvas() {
		var canvas = document.createElement('canvas');
		
		var cnt = document.getElementById('canvas-container');
		
		//canvas.width  = opt.cellSize * opt.cols;
		canvas.width = cnt.offsetWidth;
		opt.cellSize = canvas.width / opt.cols;
		canvas.height = opt.cellSize * opt.rows;
		
		cnt.appendChild(canvas);
		
		dom.canvas = canvas;
		dom.canvasContainer = cnt;
		dom.ctx = canvas.getContext('2d');
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
		var ctx = dom.ctx,
			  w = dom.canvas.width,
			  h = dom.canvas.height,
			  i = 0,
			  max = Math.max(w, h);
		
		ctx.strokeStyle = "#888";
		
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
		
		ctx.strokeStyle = "";
	} 

 	function recalculate() { //console.log('recalc');
		var nextState = new Array(opt.rows);
		for (var i = 0; i < opt.rows; i++) {
			nextState[i] = new Array(opt.cols).fill(0);
		}
		
		// quick and dirty ughhh
		var x = (opt.periodic ? 0 : 1)
		
		for (var i = x; i < opt.rows -x; i++) {
			for (var j = x; j < opt.cols -x; j++) {
				var c = currentState[i][j],
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
			}
		}
		currentState = nextState;
		updateGenerationInfo();
	}
	
	function updateGenerationInfo() {
		opt.generation++;
		dom.gencnt.textContent = opt.generation;
	}
	// returns how many ALIVE neighbours given cell has in terms of Moore's neighbourhood
	function moore(i, j, periodic) {
		
		periodic = periodic || opt.periodic;
		
		var out = 0,
			k = i - 1,
			l = j - 1,
			m = i + 1,
			n = j + 1;

		if (periodic) {			
			if (k < 0) {
				k = opt.rows - 1;
			}
			
			if (l < 0) {
				l = opt.cols - 1;
			}
			
			if (m >= opt.rows) {
				m = 0;
			}
			
			if (n >= opt.cols) {
				n = 0;
			}
		}
		
		out += currentState[k][l];
		out += currentState[k][j];
		out += currentState[k][n];
		
		out += currentState[i][l];
		out += currentState[i][n];
		
		out += currentState[m][l];
		out += currentState[m][j];
		out += currentState[m][n];
		
		return out;
	}

	function loop() {
		recalculate();
		drawField();
		
		//requestAnimFrame(loop);
		opt.timer = setTimeout(loop, opt.speed);
	}
	
	// why dis must be so ugly yhhhhh
	function setupPanel() {
		dom.cellsAmount = document.getElementById('cells-amount');
		dom.cellsAmount.value = opt.cols;
		
		dom.rule = document.getElementById('rule');
		dom.rule.value = opt.speed;
		
		dom.gencnt = document.getElementById('generation-counter');
		
		dom.imagelink = document.getElementById('imagelink');
		
		dom.iterations = document.getElementById('iterations');
		dom.iterations.value = opt.rows;
		
		dom.startBtn = document.getElementById('start');
		dom.stopBtn = document.getElementById('stop');
		
		dom.periodicBtn   = document.getElementById('periodic');
		dom.unperiodicBtn = document.getElementById('unperiodic');
		
		dom.grid = document.getElementById('grid');
		
		document.getElementById('cells-plus').addEventListener('click', function () { dom.cellsAmount.value++; handleCellsChange(); }, false);
		document.getElementById('cells-minus').addEventListener('click', function () { dom.cellsAmount.value--; handleCellsChange(); }, false);
		
		document.getElementById('rule-plus').addEventListener('click', function () { dom.rule.value++; handleRuleChange(); }, false);
		document.getElementById('rule-minus').addEventListener('click', function () { dom.rule.value--; handleRuleChange(); }, false);
		
		document.getElementById('iter-plus').addEventListener('click', function () { dom.iterations.value++; handleIterChange(); }, false);
		document.getElementById('iter-minus').addEventListener('click', function () { dom.iterations.value--; handleIterChange(); }, false);
		
		document.getElementById('pond').addEventListener('click', putPond, false);
		document.getElementById('beequeen').addEventListener('click', putBeeQueen, false);
		document.getElementById('glider').addEventListener('click', putGlider, false);
		document.getElementById('frog').addEventListener('click', putFrog, false);
		

		
		dom.cellsAmount.addEventListener('change', handleCellsChange, false);
		dom.rule.addEventListener('change', handleRuleChange, false);
		dom.iterations.addEventListener('change', handleIterChange, false);
		
		// dom.imagelink.addEventListener('change', handleImageLinkChange, false);
		
		dom.grid.addEventListener('change', function () {
			dom.grid.checked = opt.grid = !opt.grid;
			clear();
			run();
		}, false);
		
		dom.startBtn.addEventListener('click', function () {
			dom.startBtn.className = 'active-bc';
			dom.stopBtn.className = '';
			loop();
		}, false);
		
		dom.stopBtn.addEventListener('click', function (e) {
			dom.startBtn.className = '';
			dom.stopBtn.className = 'active-bc';
			clearTimeout(opt.timer);
		}, false);
		
		dom.periodicBtn.addEventListener('click', function () {
			dom.periodicBtn.className = 'active-bc';
			dom.unperiodicBtn.className = '';
			opt.periodic = true;
		}, false);
		
		dom.unperiodicBtn.addEventListener('click', function (e) {
			dom.periodicBtn.className = '';
			dom.unperiodicBtn.className = 'active-bc';
			opt.periodic = false;
		}, false);
		
		
		function handleCellsChange() {
			opt.cols = parseInt(dom.cellsAmount.value);
			clear();
			run();		
		}
		
		function handleRuleChange() {
			opt.speed = parseInt(dom.rule.value);
		}
		
/* 		function handleImageLinkChange() {
			console.log('imagelki');
			var img = new Image();
			var ctx = dom.canvas.getContext('2d');
			//img.setAttribute('crossOrigin', '');
			img.src = dom.imagelink.value + '?' + new Date().getTime();
			img.onload = function () {
				opt.imageMode = true;
				opt.img = img;
				dom.canvas.addEventListener('mousemove', function (e) {
					if (!opt.imageMode) return;
					
					var pos = getMousePos(e);
					clearCanvas();
					ctx.drawImage(opt.img, pos.x, pos.y);
					// jak to zrobic ladniej - research
					var data = ctx.getImageData(pos.x, pos.y, img.width, img.height);
					ctx.putImageData(data, 0, 0);
					console.log(pos.x, pos.y);
				}, false);
				
			}
		} */
		
		function handleIterChange() {
			opt.rows = parseInt(dom.iterations.value);
			clear();
			run();
		}
	}
	
	function setColor() {
		var colorPairs = {
			'gold':  '#D3AC75',
			'blue':  '#55C2ED',
			'rose':  '#E3A3DA',
			'green': '#9BE68E'
		};
		
		var picker = document.getElementById('color-picker');
		
		var buttons = picker.getElementsByTagName('li'); 
		
		[].forEach.call(buttons, function (el, index) {
			el.addEventListener('click', function (event) {
				resetClasses();
				this.className += ' active';
				opt.fillColor = colorPairs[this.id];
				drawField();
			}, false);
		});
		
		function resetClasses() {
			[].forEach.call(buttons, function (el) {
				el.className = '';
			});
		}
	}

	setupPanel();
	setColor();

	window.onresize = function () {
		clear();
		run();
	}

	// use it to optimize?
	function clearCanvas() {
		var ctx = dom.canvas.getContext('2d');
		ctx.clearRect(0, 0, dom.canvas.width, dom.canvas.height);
	}

	/*
	document.addEventListener('DOMContentLoaded', function() {
		
		init();
		
		document.getElementById('cells-apply').addEventListener('click', function () {
			var num = document.getElementById('amount').value;
			init(num);
			
		}, false);
	}); */
})(); 