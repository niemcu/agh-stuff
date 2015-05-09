require 'prawn'
require 'prawn/table'
require 'gruff'

class OptSprawko
  include Prawn::View
  def initialize
    @all_data = []
    @labels = []
    @test_packet = []
    @graph_data = []
  end

  def parse_file(filename)
    File.open(filename).each do |line|
      if line =~ /^[Kh]/
        @labels.push(line.chomp)
        if not @test_packet.empty?
          @all_data.push(@test_packet)
          @graph_data.push(compute_avg_packet_values(@test_packet))
          @test_packet = []
        end
      elsif line =~ /^T/
        line_data = arrayify_ab_output_line(line)
        @test_packet.push(line_data)
      else
        next
      end
    end
    # czyszczenie
    if not @test_packet.empty? # jak to zrobic ladniej krwa
      @all_data.push(@test_packet)
      @graph_data.push(compute_avg_packet_values(@test_packet))
    end
    @labels.reverse!
    @all_data.reverse!
  end
  # ta metode by mozna bylo lepiej przemyslec
  def compute_avg_packet_values(test_packet)
    out = [0, 0, 0, 0]
    (0..3).each_with_index do |i|
      (0..2).each_with_index do |j|
        out[i] += test_packet[j][i].to_f
      end
      out[i] = (out[i] / 3).round(3)
    end
    out
  end

  def generate_doc
    insert_table_headers
    column_box([0, cursor], :columns => 2, :width => bounds.width) do
      while not @labels.empty?
        text @labels.pop
        move_down 5
        t = make_table (@all_data.pop)
        t.row(0).font_style = :bold
        t.draw
        move_down 28 # odstep wyznaczony doswiadczalnie, hehe
      end
    end
    generate_graphs
    (0..3).each do |i|
      start_new_page(:layout => :landscape)
      image i.to_s + '.png'
    end
  end

  def generate_graphs
    titles = ['Time taken for tests [seconds]',
              'Requests per second',
              'Time per request [ms] (mean)',
              'Time per request [ms] (mean, across all)']
    (0..3).each_with_index do |var|
      dataset = @graph_data.collect { |arr| arr[var] }
      g = Gruff::Line.new(700)
      g.title = titles[var]

      g.theme = {
          :colors => %w(black brown blue red green),
          :marker_color => 'black',
          :font_color => 'black',
          :background_colors => 'white'
      }
      g.labels = { 0 => 'ab -n 1000', 1 => 'ab -n 5000', 2 => 'ab -n 10000', 3 => 'ab -n 100000' }
      g.data(:niezmieniane, dataset[0..3])
      g.data(:MC10, dataset[4..7])
      g.data(:MC50, dataset[8..11])
      g.data(:MC100, dataset[12..15])
      g.data(:MC1000, dataset[16..19])
      g.write(var.to_s + '.png')
    end
  end

  def insert_table_headers
    header = %w{ TTFT RPS TPR TPRA }
    @all_data.each do |tab|
      tab.unshift(header)
    end
  end

  def arrayify_ab_output_line(line)
    data = line.split
    [ data[4], data[9], data[15], data[21] ]
  end
end

gen = OptSprawko.new
gen.parse_file 'output_clean'
gen.generate_doc
gen.save_as('tabele.pdf')