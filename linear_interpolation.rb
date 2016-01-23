def linear_interpol(x, x0, y0, x1, y1)
	return y0 + ((y1 - y0) / (x1 - x0)) * (x - x0)
end

if ARGV.size >= 5
	 x = ARGV.shift
	x0 = ARGV.shift
	y0 = ARGV.shift
	x1 = ARGV.shift
	y1 = ARGV.shift
else 
	puts "Give me x: ";  x = gets
	puts "Give me x0:"; x0 = gets
	puts "Give me y0:"; y0 = gets
	puts "Give me x1:"; x1 = gets
	puts "Give me y1:"; y1 = gets
end

puts  linear_interpol(x.to_f, 
		     x0.to_f,
		     y0.to_f, 
		     x1.to_f, 
		     y1.to_f)

