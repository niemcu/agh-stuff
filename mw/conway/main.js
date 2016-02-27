// game of life conway
// autor: ympeg
// konwencja: zmienne z underscore, funkcje camelcasem;

var Grid = (function () {
})();

var applyGrid = function (cvs, size) {
	var ctx = cvs.getContext('2d'),
			  w = cvs.width,
				h = cvs.height,
				i = 0;

	for (; i < w; i += size) {
		ctx.beginPath();
		ctx.moveTo(i, 0);
		ctx.lineTo(i, h);
		ctx.stroke();

		ctx.beginPath();
		ctx.moveTo(0, i);
		ctx.lineTo(w, i);
		ctx.stroke();
	}
};

document.addEventListener('DOMContentLoaded', function() {
	var cvs = document.getElementById('game-canvas'),
			ctx = cvs.getContext('2d'),
			cell_size = 20,
			field = new Array(cell_size),
			width = cvs.width / cell_size,
			height = cvs.height / cell_size;

	// init field
	for (var i = 0; i < width; i++) {
		field[i] = new Array(cell_size);
		for (var j = 0; j < height; j++) {
			field[i][j] = Math.round(Math.random());
		}
	}

	for (var i = 0; i < width; i++) {
		for (var j = 0; j < height; j++) {
			if (field[i][j] != 0) {
				ctx.fillRect(i * cell_size, j * cell_size, cell_size, cell_size);
			}
		}
	}

	console.log("elo");
	console.log(cvs.width, cvs.height);
	
	applyGrid(cvs, cell_size);

	// 6 kolumna, 8 wiersz;
});
