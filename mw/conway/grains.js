// autor: ympeg
// konwencja: wszystko camelcase
var Grains = (function() {

	var opt = {
		cellSize: 10,
		grid: false,
		boundary: 0,
		fillColor: '#D3AC75',
		rows: 80,
		cols: 160,
		periodic: true,
		speed: 80,
		timer: null,
		generation: 0,
		imageMode: false,
		nucleidsCount: 40,
		emptyCells: 0,
		neighbourhood: moore
	};

	var colors = [];
	colors[0] = '#FFFFFF';

	var dom = {};

	var currentState = new Array(opt.rows);

	function setup() {
		for (var i = 0; i < opt.rows; i++) {
			currentState[i] = new Array(opt.cols).fill(0);
		}
		randomizeState();
	}

	function dumpCell(event) {
		var pos = getMousePos(event);

		var j = Math.floor(pos.x / opt.cellSize),
				i = Math.floor(pos.y / opt.cellSize);

		var num = currentState[i][j];

		console.log("cell: ", i, j, "id: ", num, "color: ", colors[num]);
	}

	function randomizeState() {
		for (var k = 1; k <= opt.nucleidsCount; k++) {
			var i = Math.floor(Math.random() * opt.rows);
			var j = Math.floor(Math.random() * opt.cols);

			currentState[i][j] = k;
			colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);

            // czasem losowal sie bialy
            if (colors[k] == '#ffffff') {
                colors[k] = "#111111";
            }
						// czasem sie jakies dziwne losowaly
						while (colors[k].length != 7) {
							colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
						}
		}
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
			var j = Math.floor(pos.x / opt.cellSize),
				i = Math.floor(pos.y / opt.cellSize);

			opt.nucleidsCount++;
			colors[opt.nucleidsCount + 1] = '#'+Math.floor(Math.random()*16777215).toString(16);
			currentState[i][j] = opt.nucleidsCount;
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
		dom.canvas.addEventListener('mousemove', dumpCell, false);

		drawField();
	}

	// todo REFACTOR
	function drawField () { //console.log('draw');
		var ctx = dom.ctx;
		clearCanvas();
		processEachCell(function (i, j) {
			var c = currentState[i][j];
				ctx.fillStyle = colors[c];
				ctx.fillRect(j * opt.cellSize, i * opt.cellSize, opt.cellSize +1, opt.cellSize +1);
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

		var emptyCells = 0;

		// quick and dirty ughhh
		var x = (opt.periodic ? 0 : 1)

		for (var i = x; i < opt.rows -x; i++) {
			for (var j = x; j < opt.cols -x; j++) {
				var c = currentState[i][j];
				if (c == 0) {
					emptyCells++;
					m = opt['neighbourhood'](i, j);

					nextState[i][j] = m;
				} else {
					nextState[i][j] = c;
				}
				//console.log(m);
			}
		}
		currentState = nextState;
		updateGenerationInfo();
		opt.emptyCells = emptyCells;
	}

	function updateGenerationInfo() {
		opt.generation++;
		dom.gencnt.textContent = opt.generation;
	}

	function neumann(i, j, periodic) {
        var list = new Array(opt.nucleidsCount + 1).fill(0);

		periodic = periodic || opt.periodic;

		var k = i - 1,
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


		list[currentState[k][j]]++;

		list[currentState[i][l]]++;
		list[currentState[i][n]]++;
		list[currentState[i][j]]++;

		list[currentState[m][j]]++;

		var max1 = Math.max.apply(null, list);
		var idx = list.indexOf(max1);

		if (idx == 0) {
			if (max1 == 5) {
				return 0;
			} else {
				list.shift();
				return list.indexOf(Math.max.apply(null, list)) + 1;
			}
		} else {
			return idx;
		}
  }

	function moore(i, j, periodic) {
		var list = new Array(opt.nucleidsCount + 1).fill(0);

		periodic = periodic || opt.periodic;

		var k = i - 1,
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

		list[currentState[k][l]]++;
		list[currentState[k][j]]++;
		list[currentState[k][n]]++;

		list[currentState[i][l]]++;
		list[currentState[i][n]]++;
		list[currentState[i][j]]++;

		list[currentState[m][l]]++;
		list[currentState[m][j]]++;
		list[currentState[m][n]]++;

		var max1 = Math.max.apply(null, list);
		var idx = list.indexOf(max1);

		if (idx == 0) {
			if (max1 == 9) {
				return 0;
			} else {
				list.shift();
				return list.indexOf(Math.max.apply(null, list)) + 1;
			}
		} else {
			return idx;
		}
	}

	function loop() {
		recalculate();
		drawField();

		opt.timer = setTimeout(loop, opt.speed);

		if (opt.emptyCells == 0) {
			clearTimeout(opt.timer);
		}

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

		document.getElementById('moore').addEventListener('click', function () {
			opt.neighbourhood = moore;
		}, false);

		document.getElementById('neumann').addEventListener('click', function () {
			opt.neighbourhood = neumann;
		}, false);

		document.getElementById('lhex').addEventListener('click', function () {
			opt.neighbourhood = lhex;
		}, false);

		document.getElementById('rhex').addEventListener('click', function () {
			opt.neighbourhood = rhex;
		}, false);

		document.getElementById('lpent').addEventListener('click', function () {
			opt.neighbourhood = lpent;
		}, false);

		document.getElementById('rpent').addEventListener('click', function () {
			opt.neighbourhood = rpent;
		}, false);

		// document.getElementById('beequeen').addEventListener('click', putBeeQueen, false);
		// document.getElementById('glider').addEventListener('click', putGlider, false);
		// document.getElementById('frog').addEventListener('click', putFrog, false);



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

	function lhex(i, j, periodic) {
		var list = new Array(opt.nucleidsCount + 1).fill(0);

		periodic = periodic || opt.periodic;

		var k = i - 1,
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


		list[currentState[k][l]]++;
		list[currentState[k][j]]++;
		//list[currentState[k][n]]++;

		list[currentState[i][l]]++;
		list[currentState[i][n]]++;
		list[currentState[i][j]]++;

		//list[currentState[m][l]]++;
		list[currentState[m][j]]++;
		list[currentState[m][n]]++;

		var max1 = Math.max.apply(null, list);
		var idx = list.indexOf(max1);

		if (idx == 0) {
			if (max1 == 7) {
				return 0;
			} else {
				list.shift();
				return list.indexOf(Math.max.apply(null, list)) + 1;
			}
		} else {
			return idx;
		}
	}

	function rhex(i, j, periodic) {
		var list = new Array(opt.nucleidsCount + 1).fill(0);

		periodic = periodic || opt.periodic;

		var k = i - 1,
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


		//list[currentState[k][l]]++;
		list[currentState[k][j]]++;
		list[currentState[k][n]]++;

		list[currentState[i][l]]++;
		list[currentState[i][n]]++;
		list[currentState[i][j]]++;

		list[currentState[m][l]]++;
		list[currentState[m][j]]++;
		//list[currentState[m][n]]++;

		var max1 = Math.max.apply(null, list);
		var idx = list.indexOf(max1);

		if (idx == 0) {
			if (max1 == 7) {
				return 0;
			} else {
				list.shift();
				return list.indexOf(Math.max.apply(null, list)) + 1;
			}
		} else {
			return idx;
		}
	}

	function lpent(i, j, periodic) {
		var list = new Array(opt.nucleidsCount + 1).fill(0);

		periodic = periodic || opt.periodic;

		var k = i - 1,
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


		//list[currentState[k][l]]++;
		list[currentState[k][j]]++;
		list[currentState[k][n]]++;

		//list[currentState[i][l]]++;
		list[currentState[i][n]]++;
		list[currentState[i][j]]++;

		//list[currentState[m][l]]++;
		list[currentState[m][j]]++;
		list[currentState[m][n]]++;

		var max1 = Math.max.apply(null, list);
		var idx = list.indexOf(max1);

		if (idx == 0) {
			if (max1 == 6) {
				return 0;
			} else {
				list.shift();
				return list.indexOf(Math.max.apply(null, list)) + 1;
			}
		} else {
			return idx;
		}
	}

	function rpent(i, j, periodic) {
		var list = new Array(opt.nucleidsCount + 1).fill(0);

		periodic = periodic || opt.periodic;

		var k = i - 1,
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


		list[currentState[k][l]]++;
		list[currentState[k][j]]++;
		//list[currentState[k][n]]++;

		list[currentState[i][l]]++;
		//list[currentState[i][n]]++;
		list[currentState[i][j]]++;

		list[currentState[m][l]]++;
		list[currentState[m][j]]++;
		//list[currentState[m][n]]++;

		var max1 = Math.max.apply(null, list);
		var idx = list.indexOf(max1);

		if (idx == 0) {
			if (max1 == 6) {
				return 0;
			} else {
				list.shift();
				return list.indexOf(Math.max.apply(null, list)) + 1;
			}
		} else {
			return idx;
		}
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
