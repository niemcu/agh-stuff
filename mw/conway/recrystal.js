// autor: ympeg
// konwencja: wszystko camelcase

// TODO OGOLNIE JEST NIEZLE TYLKO CRIT RO NIE WIADOMO JAKIE NO I
// JESZCZE BY TRZEBA BYLO DODAWAC TEZ DYSLOKACJI DO TYCH ZREKRYSTALIZOWANYCH
// ALE PO PROSTU JUZ NIE REKRYSTALIZUJA

var Recrystal = (function () {

	var opt = {};

	opt.timeStep = 0;
	opt.critRo = 400888877;

	opt.numcells = 0;

	opt.cellSize = 10;
	opt.speed = 80;

	opt.rows = 80;
	opt.cols = 160;

	opt.periodic = true;

	opt.reccnt = 1;

	opt.previousRo = 0;

	var dom = {};

	var colors = [];

	var state = new Array(opt.rows);

	var getRo = function(time) {
		var A = 8.671097e+13,
		B = 9.41e+00;

		time = time || opt.timeStep;

		var ro = A/B + (1-A/B)* Math.exp(-B*time);

		var out = ro - opt.previousRo;
		opt.previousRo = ro;
		return out;
	};

	function processEachCell(callback) {
		for (var i = 0; i < opt.rows; i++) {
			for (var j = 0; j < opt.cols; j++) {
				// if typeof
				callback(i, j);
			}
		}
	}

	var init = function (microstruct, nucleids, colorsArr) {
		for (var i = 0; i < opt.rows; i++) {
			state[i] = new Array(opt.cols);
		}
		var canvas = document.getElementsByTagName('canvas')[0];
		dom.canvas = canvas;
		dom.ctx = canvas.getContext('2d');
		opt.cellSize = dom.canvas.width / opt.cols;
		colors = colorsArr;

		opt.nucleidsCount = nucleids;

		processEachCell(function (i, j) {
			state[i][j] = {
				id: microstruct[i][j],
				ro: 0,
				rec: false
			};
		});

		//console.log(state);

		step();

		//
		// for (; opt.timeStep < 0.2; opt.timeStep += 0.001) {
		// 	console.log('ro ' + Math.round(opt.timeStep, 3), getCurrentRo().toExponential(2));
		// }
	};

	var step = function() {
		var numcells = 0;
		var reccells = 0;
		var edgycnt = 0;
		var notedgycnt = 0;
		opt.timer = setTimeout(step, opt.speed);
		//console.log('krok ' + opt.timeStep);
		var addRo;
		// oblicz ro srednie
		var avgRo = getRo() / (opt.rows*opt.cols);

		// dorzuc i sprawdz
		processEachCell(function (i, j) {
			numcells++;
			if (!state[i][j].rec) {
				var neigh = checkNeighboursRecrystalized(i, j);
				// jesli wyjebalo ro krytyczne - rekrystalizujmy
				if (neigh) {
					state[i][j].rec = true;
					state[i][j].id = neigh;
					state[i][j].recNow = true;
					state[i][j].ro = 0;
					//colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
				} else if (state[i][j].ro > opt.critRo) {
					//console.log('mamy rekrystaliz');
					opt.nucleidsCount++;
					var k = opt.nucleidsCount;
					//if (k > 200) clearTimeout(opt.timer);
					//alert('no w koncu! Komorka ' + i + ' ' + j + ' rekrystalizuje!');
					state[i][j].rec = true;
					state[i][j].id = k;
					state[i][j].recNow = true;
					state[i][j].ro = 0;
					//colors[k] = '#000000';
					//console.log(k);
					colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
					// // czasem losowal sie bialy
					// if (colors[k] == '#ffffff') {
					// 	colors[k] = "#111111";
					// }
					// // czasem sie jakies dziwne losowaly
					// while (colors[k].length != 7) {
					// 	colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
					// }

					// sprawdzanie czy sasiad zrekrystalizowal w poprzednim kroku czasowym
				} else {
					// dorzucmy mu dyslokacji
					if (isOnEdge(i, j)) {
						addRo = avgRo * (Math.floor(Math.random() * (180-120) + 120)/100);
						//console.log('kom na granicy ro =' + state[i][j].ro);
						edgycnt++;
					} else {
						notedgycnt++;
						addRo = avgRo * (Math.floor(Math.random() * 10)/100);
					}
					state[i][j].ro += addRo;
				}
			} else {
				reccells++;

				if (state[i][j].recPreviously) {
						state[i][j].recPreviously = false;
				}

				if (state[i][j].recNow) {
					state[i][j].recNow = false;
					state[i][j].recPreviously = true;
				}
			}

		});
		//console.log(edgycnt);
		opt.timeStep += 0.001;

		// sumuj dyslokacje
		var roSum = 0;
		processEachCell(function (i, j) {
			roSum += state[i][j].ro;
		});
		console.log(roSum);

		drawField();


		//console.log(edgycnt, notedgycnt);
		if (numcells == reccells) {
			clearTimeout(opt.timer);
			console.log('koniec zabawy');
			console.log(state);
			console.log(colors);
		}
	};

	var checkNeighboursRecrystalized = function(i, j, periodic) {
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

		if(state[k][l].recPreviously) return state[k][l].id;
		if(state[k][j].recPreviously) return state[k][j].id;
		if(state[k][n].recPreviously) return state[k][n].id;

		if(state[i][l].recPreviously) return state[i][l].id;
		if(state[i][n].recPreviously) return state[i][n].id;
		if(state[i][j].recPreviously) return state[i][j].id;

		if(state[m][l].recPreviously) return state[m][l].id;
		if(state[m][j].recPreviously) return state[m][j].id;
		if(state[m][n].recPreviously) return state[m][n].id;

		return false;
	};

	var drawField = function () { //console.log('draw');
	var ctx = dom.ctx;
	clearCanvas();
	processEachCell(function (i, j) {
		var c = state[i][j];
		if (c.id == 0) {
		} else {
			ctx.fillStyle = colors[c.id];
			ctx.fillRect(j * opt.cellSize, i * opt.cellSize, opt.cellSize +1, opt.cellSize +1);
			if (c.rec) {
			}
		}
	});

	//if (opt.grid) applyGrid();
}

var clearCanvas = function () {
	var ctx = dom.canvas.getContext('2d');
	ctx.clearRect(0, 0, dom.canvas.width, dom.canvas.height);
}

var isOnEdge = function (i, j) {
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

	var curr = state[i][j].id;

	if (state[k][l].id != curr) return true;
	if (state[k][j].id != curr) return true;
	if (state[k][n].id != curr) return true;
	if (state[i][l].id != curr) return true;
	if (state[i][n].id != curr) return true;
	if (state[m][l].id != curr) return true;
	if (state[m][j].id != curr) return true;
	if (state[m][n].id != curr) return true;

	return false;
}

var privFun = function () {
	console.log('jestem w hcuj prywatny');
};

var opti = { huj: 54 };

var recrystalize = function () {

};

return {
	init: init,
	recrystalize: recrystalize
};
})();









var Grains = (function(Recrystal) {

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
		nucleidsCount: 120,
		emptyCells: 0,
		neighbourhood: moore,
		distRadius: 10,
		showRadius: false,
		running: false,
		fullrandom: false,
		randompent: false
	};

	var colors = [];
	colors[0] = '#FFFFFF';

	var dom = {};

	var currentState = new Array(opt.rows);

	function setup() {
		for (var i = 0; i < opt.rows; i++) {
			currentState[i] = new Array(opt.cols).fill(0);
		}
	}

	function dumpCell(event) {
		var pos = getMousePos(event);

		var j = Math.floor(pos.x / opt.cellSize),
		i = Math.floor(pos.y / opt.cellSize);

		var num = currentState[i][j];

		//console.log("cell: ", i, j, "id: ", num, "color: ", colors[num]);
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

			colors[opt.nucleidsCount + 1] = '#'+Math.floor(Math.random()*16777215).toString(16);
			opt.nucleidsCount++;
			currentState[i][j] = opt.nucleidsCount;


			handleNucleidsChange(false);

			if (opt.nucleidsCount == 0) {

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
		dom.canvas.addEventListener('mousemove', dumpCell, false);

		drawField();
	}

	// todo REFACTOR
	function drawField () { //console.log('draw');
	var ctx = dom.ctx;
	clearCanvas();
	processEachCell(function (i, j) {
		var c = currentState[i][j];
		if (c != 0 && opt.showRadius && !opt.running) {
			ctx.beginPath();
			ctx.arc(j*opt.cellSize + opt.cellSize/2, i*opt.cellSize + opt.cellSize/2, opt.distRadius, 0, 2 * Math.PI, false);
			ctx.fillStyle = 'yellow';
			ctx.fill();
			ctx.fillStyle = colors[c];
		}
		if (c == 0) {

		} else {
			ctx.fillStyle = colors[c];
			ctx.fillRect(j * opt.cellSize, i * opt.cellSize, opt.cellSize +1, opt.cellSize +1);
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

var emptyCells = 0;

// quick and dirty ughhh
var x = (opt.periodic ? 0 : 1)

for (var i = x; i < opt.rows -x; i++) {
	for (var j = x; j < opt.cols -x; j++) {
		var c = currentState[i][j];
		if (c == 0) {
			emptyCells++;
			var fun = opt['neighbourhood'];
			if (opt.fullrandom) {
				fun = randomNeighbourhood();
			}
			if (opt.randompent) {
				fun = randomPent();
			}
			m = fun(i, j);

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
		console.log('dosyc');
		alert('Przystepujemy do rekrystalizacji!');
		Recrystal.init(currentState, opt.nucleidsCount, colors);
	}

}

function randomNeighbourhood() {
	var arr = [moore, neumann, rpent, lpent, rhex, lhex];
	var n = Math.floor((Math.random() * arr.length));
	return arr[n];
}

function randomPent() {
	var arr = [rpent, lpent];
	var n = Math.floor((Math.random() * arr.length));
	return arr[n];
}

function distributeEvenly() {
	var n = opt.nucleidsCount;
	var i = opt.rows;
	var j = opt.cols;

	var difColumn = Math.round(j / (Math.sqrt(n)+1));
	var difRow = Math.round(i / (Math.sqrt(n)+1));
	var k = 1;
	var ccol = difColumn;
	var crow = difRow;

	for (; difRow < opt.rows; difRow += crow) {
		if (k == n+1) break;
		var difColumn = Math.round(j / (Math.sqrt(n)+1));

		for (; difColumn < opt.cols; difColumn += ccol) {
			currentState[difRow][difColumn] = k;
			colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
			//czasem losowal sie bialy
			if (colors[k] == '#ffffff') {
				colors[k] = "#111111";
			}
			// czasem sie jakies dziwne losowaly
			while (colors[k].length != 7) {
				colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
			}
			k++;
			if (k == n+1) break;
		}
	}

	// for (var k = 1; k <= n; k++) {
	//
	// 	console.log(difRow, difColumn);
	// 	currentState[difRow][difColumn] = k;
	// 	colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
	//
	// 				// czasem losowal sie bialy
	// 				if (colors[k] == '#ffffff') {
	// 						colors[k] = "#111111";
	// 				}
	// 				// czasem sie jakies dziwne losowaly
	// 				while (colors[k].length != 7) {
	// 					colors[k] = '#'+Math.floor(Math.random()*16777215).toString(16);
	// 				}
	// 	difRow += difRow;
	// 	difColumn += difColumn	;
	// }
}

function distributeRadius() {
	var r = opt.distRadius;

	if (r == 0)
	return;

	var stack = [];

	while(stack.length != opt.nucleidsCount) {
		var i = Math.floor(Math.random() * opt.rows);
		var j = Math.floor(Math.random() * opt.cols);
		// przelec przez wszystkie juz zrobione punkty i zobacz czy nie siadles w radius ich
		var good = true;
		for (var p = 0; p < stack.length; p++) {
			var pt = stack[p];
			if (Math.sqrt((i-pt.x)*(i-pt.x) + (j-pt.y)*(j-pt.y)) < opt.distRadius) {
				good = false;
				break;
			}
		}
		if (good) {
			stack.push({x: i, y: j});
		}
	}

	for (var k = 1; k <= opt.nucleidsCount; k++) {
		var point = stack[k-1];
		currentState[point.x][point.y] = k;
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

// why dis must be so ugly yhhhhh
function setupPanel() {
	dom.cellsAmount = document.getElementById('cells-amount');
	dom.cellsAmount.value = opt.cols;

	dom.rule = document.getElementById('rule');
	dom.rule.value = opt.speed;

	dom.gencnt = document.getElementById('generation-counter');

	dom.manual = document.getElementById('manual');
	dom.frandom = document.getElementById('frandom');
	dom.evenly = document.getElementById('evenly');
	dom.radius = document.getElementById('radius');

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

	dom.grid = document.getElementById('grid');

	document.getElementById('cells-plus').addEventListener('click', function () { dom.cellsAmount.value++; handleCellsChange(); }, false);
	document.getElementById('cells-minus').addEventListener('click', function () { dom.cellsAmount.value--; handleCellsChange(); }, false);

	document.getElementById('rule-plus').addEventListener('click', function () { dom.rule.value++; handleRuleChange(); }, false);
	document.getElementById('rule-minus').addEventListener('click', function () { dom.rule.value--; handleRuleChange(); }, false);

	document.getElementById('iter-plus').addEventListener('click', function () { dom.iterations.value++; handleIterChange(); }, false);
	document.getElementById('iter-minus').addEventListener('click', function () { dom.iterations.value--; handleIterChange(); }, false);

	document.getElementById('nucleids-plus').addEventListener('click', function () { dom.nucleids.value++; opt.nucleidsCount++; handleNucleidsChange(); }, false);
	//document.getElementById('nucleids-minus').addEventListener('click', function () { dom.nucleids.value--; handleNucleidsChange(); }, false);
	document.getElementById('radiusval-plus').addEventListener('click', function () { dom.radiusval.value++; opt.distRadius++; handleRadiusChange(); }, false);

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

	dom.manual.addEventListener('click', function () {
		clear();
		run();
		opt.nucleidsCount = 0;
	});

	dom.frandom.addEventListener('click', function () {
		clear();
		run();
		var n = parseInt(dom.nucleids.value);
		if (n != 0) {
			opt.nucleidsCount = n;
			randomizeState();
			drawField();
		} else {
			console.log('nucleids cannot be 0!');
		}
	});

	dom.evenly.addEventListener('click', function () {
		clear();
		run();
		var n = parseInt(dom.nucleids.value);
		if (n != 0) {
			opt.nucleidsCount = n;
			distributeEvenly();
			drawField();
		} else {
			console.log('nucleids cannot be 0!');
		}
	});

	dom.radius.addEventListener('click', function () {
		clear();
		run();
		var n = parseInt(dom.nucleids.value);
		if (n != 0) {
			opt.nucleidsCount = n;
			distributeRadius();
			drawField();
		} else {
			console.log('nucleids cannot be 0!');
		}
	});

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
})(Recrystal);
