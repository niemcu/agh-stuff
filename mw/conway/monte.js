// autor: ympeg
// konwencja: wszystko camelcase
var MonteCarlo = (function() {

	var opt = {
		cellSize: 10,
		grid: false,
		boundary: 0,
		fillColor: '#D3AC75',
		rows: 200,
		cols: 250,
		periodic: true,
		speed: 80,
		timer: null,
		generation: 0,
		imageMode: false,
		nucleidsCount: 0,
		emptyCells: 0,
		distRadius: 10,
		showRadius: false,
		running: false,
	};

	//opt.idCount = opt.rows * opt.cols;

	var colors = [];
	colors[0] = '#FFFFFF';

	var dom = {};

	var state = new Array(opt.rows);

	var allIndexes = [];

	function setup() {
		for (var i = 0; i < opt.rows; i++) {
			state[i] = new Array(opt.cols).fill(0);
		}

		//build array to shuffle
		var q = 0;
		processEachCell(function (i, j) {
			allIndexes[q] = {};
			allIndexes[q].i = i;
			allIndexes[q].j = j;
			q++;
		});
	}

	function dumpCell(event) {
		var pos = getMousePos(event);

		var j = Math.floor(pos.x / opt.cellSize),
				i = Math.floor(pos.y / opt.cellSize);

		var num = state[i][j];

		console.log("cell: ", i, j, "id: ", num, "color: ", colors[num]);
	}

	var colors = [];

	function randomizeState() {
    var k = 0;
		processEachCell(function (i, j) {
      //var k = randomInt(0,50);
	    state[i][j] = k;
			colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
			// czasem losowal sie bialy
//            if (colors[k] == '#ffffff') {
//                colors[k] = "#111111";
//            }
			// czasem sie jakies dziwne losowaly
			while (colors[k].length != 7) {
				colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
			}
			k++;
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
		console.log('co jest');
		randomizeState();

		//dom.canvas.addEventListener('click', handleClick, false);
		dom.canvas.addEventListener('mousemove', dumpCell, false);

		drawField();
	}

	// todo REFACTOR
	function drawField () { //console.log('draw');
		var ctx = dom.ctx;
		clearCanvas();
		processEachCell(function (i, j) {
			var c = state[i][j];
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

	function reverseKronecker(i, j) {
		//console.log('revers dostalem', i, j);
		if (i == j) return 0;
		else return 1;
	}

	function randomInt(min, max) {
		return Math.floor(Math.random()*(max-min+1)+min);
	}

	function getEnergy(i, j, value) {
		value = value || state[i][j];

    var list = {};

    list[value] = 1;

    var k = i - 1,
			l = j - 1,
			m = i + 1,
			n = j + 1;

		if (opt.periodic) {
			if (k < 0)
				k = opt.rows - 1;
			if (l < 0)
				l = opt.cols - 1;
			if (m >= opt.rows)
				m = 0;
			if (n >= opt.cols)
				n = 0;
		}

    // dobry potwór XD chwala edycji wielu linii na raz
		if(list[state[k][l]] == null) list[state[k][l]] = 1;
		else list[state[k][l]]++;
		if(list[state[k][j]] == null) list[state[k][j]] = 1;
		else list[state[k][j]]++;
		if(list[state[k][n]] == null) list[state[k][n]] = 1;
		else list[state[k][n]]++;
		if(list[state[i][l]] == null) list[state[i][l]] = 1;
		else list[state[i][l]]++;
		if(list[state[i][n]] == null) list[state[i][n]] = 1;
		else list[state[i][n]]++;
		if(list[state[m][l]] == null) list[state[m][l]] = 1;
		else list[state[m][l]]++;
		if(list[state[m][j]] == null) list[state[m][j]] = 1;
		else list[state[m][j]]++;
		if(list[state[m][n]] == null) list[state[m][n]] = 1;
		else list[state[m][n]]++;

		var energy = 0;

		energy += reverseKronecker(value, state[k][l]);
		energy += reverseKronecker(value, state[k][j]);
		energy += reverseKronecker(value, state[k][n]);
		energy += reverseKronecker(value, state[i][l]);
		energy += reverseKronecker(value, state[i][n]);
		energy += reverseKronecker(value, state[m][l]);
		energy += reverseKronecker(value, state[m][j]);
		energy += reverseKronecker(value, state[m][n]);

    var neighbors = [];

    Object.keys(list).forEach(function (el) {
      var i = parseInt(el);
      if (i != value) {
        neighbors.push(i);
      }
    });

		return {
			energy: energy,
			neighbors: neighbors
		}
	}

	function shuffle(array) {
		let counter = array.length;

		// While there are elements in the array
		while (counter > 0) {
			// Pick a random index
			let index = Math.floor(Math.random() * counter);

			// Decrease counter by 1
			counter--;

			// And swap the last element with it
			let temp = array[counter];
			array[counter] = array[index];
			array[index] = temp;
		}

		return array;
	}
	// cycle
 	function recalculate() { //console.log('recalc');
		var end = allIndexes.length;
		var indices = shuffle(allIndexes);
		for (var p = 0; p < end; p++) {
			var cell = indices[p];

			var e1 = getEnergy(cell.i, cell.j);
			// jak 0 to jest w srodku, zostawic ja
			if (e1.energy == 0) {

			} else {
				var x = randomInt(0, e1.neighbors.length-1);
				var e2 = getEnergy(cell.i, cell.j, e1.neighbors[x]);

        if (e2.energy <= e1.energy) {
					state[cell.i][cell.j] = e1.neighbors[x];
          change = true;
				}
			}
		}
	}

	function updateGenerationInfo() {
		opt.generation++;
		dom.gencnt.textContent = opt.generation;
	}

	function loop() {
    opt.timer = setTimeout(loop, opt.speed);

		recalculate();
		drawField();


		// if (opt.emptyCells == 0) {
		// 	clearTimeout(opt.timer);
		// }

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

		dom.nucleids = document.getElementById('nucleids');
		dom.nucleids.value = opt.nucleidsCount;

		dom.radiusval = document.getElementById('radiusval');
		dom.radiusval.value = opt.distRadius;

		dom.startBtn = document.getElementById('start');
		dom.stopBtn = document.getElementById('stop');

		dom.periodicBtn   = document.getElementById('periodic');
		dom.unperiodicBtn = document.getElementById('unperiodic');

		document.getElementById('cells-plus').addEventListener('click', function () { dom.cellsAmount.value++; handleCellsChange(); }, false);
		document.getElementById('cells-minus').addEventListener('click', function () { dom.cellsAmount.value--; handleCellsChange(); }, false);

		document.getElementById('rule-plus').addEventListener('click', function () { dom.rule.value++; handleRuleChange(); }, false);
		document.getElementById('rule-minus').addEventListener('click', function () { dom.rule.value--; handleRuleChange(); }, false);

		document.getElementById('iter-plus').addEventListener('click', function () { dom.iterations.value++; handleIterChange(); }, false);
		document.getElementById('iter-minus').addEventListener('click', function () { dom.iterations.value--; handleIterChange(); }, false);

		document.getElementById('nucleids-plus').addEventListener('click', function () { dom.nucleids.value++; opt.nucleidsCount++; handleNucleidsChange(); }, false);
		//document.getElementById('nucleids-minus').addEventListener('click', function () { dom.nucleids.value--; handleNucleidsChange(); }, false);
		document.getElementById('radiusval-plus').addEventListener('click', function () { dom.radiusval.value++; opt.distRadius++; handleRadiusChange(); }, false);

		// document.getElementById('beequeen').addEventListener('click', putBeeQueen, false);
		// document.getElementById('glider').addEventListener('click', putGlider, false);
		// document.getElementById('frog').addEventListener('click', putFrog, false);

		dom.cellsAmount.addEventListener('change', handleCellsChange, false);
		dom.rule.addEventListener('change', handleRuleChange, false);
		dom.iterations.addEventListener('change', handleIterChange, false);

		// dom.imagelink.addEventListener('change', handleImageLinkChange, false);

		dom.startBtn.addEventListener('click', function () {
			dom.startBtn.className = 'active-bc';
			dom.stopBtn.className = '';
			opt.running = true;
			loop();
		}, false);

		dom.stopBtn.addEventListener('click', function (e) {
			dom.startBtn.className = '';
			dom.stopBtn.className = 'active-bc';
			opt.running = false;
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

		function handleRadiusChange() {
			opt.distRadius = parseInt(dom.radiusval.value);
			clear();
			run();
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

	function handleNucleidsChange(clearAndRun = true) {
		dom.nucleids.value = opt.nucleidsCount;
		opt.nucleidsCount = parseInt(dom.nucleids.value);

		if (clearAndRun) {
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

		// var picker = document.getElementById('color-picker');
		//
		// var buttons = picker.getElementsByTagName('li');
		//
		// [].forEach.call(buttons, function (el, index) {
		// 	el.addEventListener('click', function (event) {
		// 		resetClasses();
		// 		this.className += ' active';
		// 		opt.fillColor = colorPairs[this.id];
		// 		drawField();
		// 	}, false);
		// });
		//
		// function resetClasses() {
		// 	[].forEach.call(buttons, function (el) {
		// 		el.className = '';
		// 	});
		// }
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
})();
