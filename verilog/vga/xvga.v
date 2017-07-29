module xvga(clk,hcount,vcount,hsync,vsync,rgb);
	input clk;  // 64.8Mhz
	output [10:0] hcount;
	output [9:0] vcount;
	output hsync,vsync;
	output [2:0] rgb;

	reg hsync ,vsync,hblank,vblank,blank;
	reg [10:0] hcount;
	reg [9:0] vcount;

	wire hsyncon,hsyncoff,hreset,hblankon;
	wire vsyncon,vsyncoff,vreset,vblankon;

	wire next_hb=hreset?0:hblankon?1:hblank;
	wire next_vb=vreset?0:vblankon?1:vblank;

	always@(posedge clk)
	begin
		hcount<=hreset?0:hcount+1;
		hblank<=next_hb;
		hsync<=hsyncon?0:hsyncoff?1:hsync;

		vcount<=hreset?(vreset?0:vcount+1):vcount;
		vblank<=next_vb;
		vsync<=vsyncon?0:vsyncoff?1:vsync; //active low 
	end


	assign hblankon=()
