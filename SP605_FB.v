`timescale 	100ps / 100ps
//-------------------------------------
// SP605_FB.v
//-------------------------------------
// History of Changes:
//	10-09-2009 Created based on SP601_FB.v
//-------------------------------------
//-------------------------------------
module SP605_FB 
#(
   parameter C3_P0_MASK_SIZE           = 4,
   parameter C3_P0_DATA_PORT_SIZE      = 32,
   parameter C3_P1_MASK_SIZE           = 4,
   parameter C3_P1_DATA_PORT_SIZE      = 32,
   parameter C3_CLK_PERIOD           = 2500,       
   parameter C3_CALIB_SOFT_IP        = "FALSE",       
   parameter C3_SIMULATION           = "FALSE",       
   parameter C3_RST_ACT_LOW          = 0,       
   parameter C3_MEM_ADDR_ORDER       = "ROW_BANK_COLUMN",       
   parameter C3_NUM_DQ_PINS          = 16,       
   parameter C3_MEM_ADDR_WIDTH       = 13,       
   parameter C3_MEM_BANKADDR_WIDTH   = 3,       
   parameter C3_MC_CALIB_BYPASS      = "NO"           
)
(

// Interface to DDR3 memory
output	wire			DDR3_CLK_P,
output	wire			DDR3_CLK_N,
output	wire			DDR3_CKE,
output	wire	[14:0]	DDR3_A,
output	wire	[2:0]	DDR3_BA,
output	wire			DDR3_RAS_B,
output	wire			DDR3_CAS_B,
output	wire			DDR3_WE_B,
output	wire			DDR3_RESET_N,
output	wire			DDR3_UDM,
output	wire			DDR3_LDM,
inout	tri 			DDR3_UDQS_P,
inout	tri 			DDR3_UDQS_N,
inout	tri 			DDR3_LDQS_P,
inout	tri 			DDR3_LDQS_N,
inout	tri 	[15:0]	DDR3_DQ,
output	wire 			DDR3_ODT,
inout					FPGA_ONCHIP_TERM1, // ZIO - open?
inout		 			FPGA_ONCHIP_TERM2, // RZQ - soft calibration R

// Master Clock for memory controller block 
input	wire			MCBCLK_2X_0,   // CLKOUT0 from PLL @ 800 MHz
input	wire			MCBCLK_2X_180, // CLKOUT1 from PLL @ 800 MHz, 180 degree phase
input	wire			MCBCLK_PLL_LOCK, // from PLL 
output	wire			MCB_PLL_LOCK, // from MCB_PLL 
input	wire			CALIB_CLK, // GCLK.  MIN = 50MHz, MAX = 100MHz.

// system Reset - resets MCB (MCB also reset by !PLL_LOCK)
input	wire			RST, 
input	wire			RSTCLK,

// Ethernet Write Port - writes original image
input	wire			ETH_WR_CLK,        // expect to be CLK125, but whatever
input	wire	[15:0]	ETH_WR_START_X,    // start X
input	wire	[15:0]	ETH_WR_LINE,       // Line to write to
input	wire	[15:0]	ETH_WR_NUM_PIXELS, // number of pixels to write
input	wire			ETH_WR_GO,         // strobe before set of writes - latches parameters
output	reg 			ETH_WR_DONE = 0,   // Strobes when done
output	reg 			ETH_WR_BUSY = 0,   // Asserted from GO to DONE
input	wire	[23:0]	ETH_WR_PIX,
input	wire			ETH_WR_STB,        // Start strobing in pixels after GO

// Ethernet Read Port - Reads processed image
input	wire			ETH_RD_CLK,        // expect to be CLK125, but whatever
input	wire	[15:0]	ETH_RD_START_X,    // start X
input	wire	[15:0]	ETH_RD_LINE,       // Line to read from
input	wire	[15:0]	ETH_RD_NUM_PIXELS, // number of pixels to read
input	wire			ETH_RD_FRAME_SEL,  // 0 for original, 1 for processed
input	wire			ETH_RD_GO,         // strobe before set of reads - latches parameters, starts
output	reg 			ETH_RD_DONE = 0,   // Strobes when done
output	reg 			ETH_RD_BUSY = 0,   // Asserted from GO to DONE
output	reg 			ETH_RD_READY = 0,  // indicates that read FIFO has pixels ready for RD_STB
output	reg 	[23:0]	ETH_RD_PIX,
input	wire			ETH_RD_STB,        // Strobe to pull RD_PIX from FIFO

// Processing Read Port - Reads original image
input	wire			PROC_RD_CLK,       // Processing clock (200MHz?)
input	wire	[15:0]	PROC_RD_WIDTH,     // in pixels (max 4k)
input	wire	[15:0]	PROC_RD_HEIGHT,    // in pixels (max 4k)
input	wire			PROC_RD_GO,        // Strobe to start frame read - latches parameters, starts
output	reg 			PROC_RD_DONE = 0,  // Strobes when done
output	reg 			PROC_RD_BUSY = 0,  // Asserted from GO to DONE
output	wire			PROC_RD_READY,     // indicates that read FIFO has pixels ready for RD_STB
output	wire	[23:0]	PROC_RD_PIX,
input	wire			PROC_RD_STB,       // Strobe to pull RD_PIX from FIFO

// Processing Write Port - writes processed image
input	wire			PROC_WR_CLK,       // Processing clock (200MHz?)
input	wire	[15:0]	PROC_WR_WIDTH,     // in pixels (max 4k)
input	wire	[15:0]	PROC_WR_HEIGHT,    // in pixels (max 4k)
input	wire			PROC_WR_GO,        // strobe before start of frame - latches parameters
output	reg 			PROC_WR_DONE = 0,  // Strobes when done
output	reg 			PROC_WR_BUSY = 0,  // Asserted from GO to DONE
input	wire	[23:0]	PROC_WR_PIX,
input	wire			PROC_WR_STB,        // Start strobing in pixels after GO


// DVI Read Port - Reads original image
input	wire			DVI_RD_CLK,       // DVI output clock (100-150MHz?)
input	wire			DVI_RD_FRAME_SEL,  // 0 for original, 1 for processed
input	wire	[15:0]	DVI_RD_WIDTH,     // in pixels (max 4k)
input	wire	[15:0]	DVI_RD_HEIGHT,    // in pixels (max 4k)
input	wire			DVI_RD_GO,        // Strobe to start frame read - latches parameters, starts
output	reg 			DVI_RD_DONE = 0,  // Strobes when done
output	reg 			DVI_RD_BUSY = 0,  // Asserted from GO to DONE
output	wire			DVI_RD_READY,     // indicates that read FIFO has pixels ready for RD_STB
output	wire	[23:0]	DVI_RD_PIX,
input	wire			DVI_RD_STB       // Strobe to pull RD_PIX from FIFO


);

//------- Ethernet Write Port - writes original image -------
reg 	[8:0]	eth_wr_cntdn = 0;
reg 	[3:0]	eth_wr_fifo_cnt = 0;
reg 	[11:0]	eth_wr_y, eth_wr_x;
// write data path
wire	[31:0]	eth_wr_data = {8'h00, ETH_WR_PIX[23:0]};
wire			eth_wr_en = ETH_WR_BUSY && ETH_WR_STB; // ignore if not BUSY
wire	[3:0]	eth_wr_mask = 4'b0000; // write all bytes always
// generate command 
reg 	[29:0]	eth_wr_cmd_addr;
reg 	[5:0]	eth_wr_cmd_bl; 
reg 			eth_wr_cmd_en = 0;
wire	[2:0]	eth_wr_cmd_instr = 3'b000; // write, no AP

always @(posedge ETH_WR_CLK)
	if (!ETH_WR_BUSY && ETH_WR_GO) // ignore GO if busy (protection)
		begin
		eth_wr_fifo_cnt <= 0; // should aready be 0!!! 
		eth_wr_cntdn    <= ETH_WR_NUM_PIXELS;
		eth_wr_y        <= ETH_WR_LINE;
		eth_wr_x        <= ETH_WR_START_X;
		ETH_WR_BUSY     <= 1;
		ETH_WR_DONE     <= 0;
		eth_wr_cmd_en   <= 0;
		end
	else if (ETH_WR_BUSY && ETH_WR_STB) // pixel written to write datapath, ignore if not busy
		begin
		eth_wr_cntdn <= eth_wr_cntdn -1;
		if ((eth_wr_cntdn == 1)||(eth_wr_fifo_cnt == 4'd15)) // last pixel or set of 16
			begin
			eth_wr_cmd_addr <= {eth_wr_x[11:10], eth_wr_y[11:0], 1'b0, eth_wr_x[9:0], 2'b00};
			eth_wr_cmd_bl   <= eth_wr_fifo_cnt; // actual burst length is bl +1
			eth_wr_cmd_en   <= 1; // issue the command
			eth_wr_fifo_cnt <= 0; // reset the count
			eth_wr_x        <= eth_wr_x + 16; // lower 4 bits are constant!
			ETH_WR_BUSY     <= (eth_wr_cntdn != 1); // clear if done
			ETH_WR_DONE     <= (eth_wr_cntdn == 1); // set if done
			end
		else // not time for a burst
			begin
			eth_wr_cmd_en   <= 0;
			eth_wr_fifo_cnt <= eth_wr_fifo_cnt +1; // count how many are in the FIFO
			end
		end
	else
		begin
		eth_wr_cmd_en   <= 0;
		ETH_WR_DONE     <= 0;
		end


//wire	[35:0]	ctrl0, ctrl1;
//icon_tst icontst (
//    .CONTROL0(ctrl0), // INOUT BUS [35:0]
//    .CONTROL1(ctrl1)  // INOUT BUS [35:0]
//);
//
////ILA port
//wire	eth_wr_cmd_empty, eth_wr_cmd_full, eth_wr_full, eth_wr_underrun, eth_wr_error;
//wire	[6:0]	eth_wr_count;
//tst_ila tstilawr (
//    .CONTROL(ctrl0), // INOUT BUS [35:0]
//    .CLK(ETH_WR_CLK), // IN
//    .TRIG0({eth_wr_cmd_en, eth_wr_cmd_instr[2:0], eth_wr_cmd_full, eth_wr_cmd_empty, eth_wr_cmd_bl[5:0]}), // IN BUS [31:0]
//    .TRIG1(eth_wr_cmd_addr[29:0]), // IN BUS [31:0]
//    .TRIG2(eth_wr_data[31:0]), // IN BUS [31:0]
//    .TRIG3({eth_wr_en, eth_wr_mask[3:0], eth_wr_underrun, eth_wr_error, eth_wr_full, eth_wr_empty, eth_wr_count[6:0]}) // IN BUS [31:0]
//);

//------- Ethernet Read Port - Reads processed image -------
reg 	[8:0]	eth_rd_cntdn = 0; // pixels left in line, -1
reg 	[6:0]	eth_rd_fifo_cnt = 0;
reg 	[11:0]	eth_rd_y, eth_rd_x;
// read data path
wire			eth_rd_empty; // FIFO empty flag
wire			eth_rd_en = !eth_rd_empty; // keep FIFO empty
wire	[31:0]	eth_rd_data; // FIFO rd data
// pipeline delay read for timing
reg 			eth_rd_en_prev = 0; 
reg 	[23:0]	eth_rd_dat_prev;
always @(posedge ETH_RD_CLK) 
	begin
	eth_rd_en_prev <= eth_rd_en;
	eth_rd_dat_prev <= eth_rd_data[23:0];
	end
// generate command 
reg 	[29:0]	eth_rd_cmd_addr;
reg 	[5:0]	eth_rd_cmd_bl; 
reg 			eth_rd_cmd_en = 0;
wire	[2:0]	eth_rd_cmd_instr = 3'b001; // read, no AP

always @(posedge ETH_RD_CLK)
	if (!ETH_RD_BUSY) // in between frames
		begin
		if(ETH_RD_GO) 
			begin
			eth_rd_cntdn    <= ETH_RD_NUM_PIXELS -1;
			eth_rd_y        <= ETH_RD_LINE;
			eth_rd_x        <= ETH_RD_START_X;
			ETH_RD_BUSY     <= 1;
			end
		eth_rd_fifo_cnt <= 0; // FIFO should aready be before GO!!
		eth_rd_cmd_en   <= 0;
		ETH_RD_DONE     <= 0;
		end
	else // reading a packet
		begin
		if ((eth_rd_fifo_cnt[6:5] == 2'b00) && !eth_rd_cmd_en) // Request more: FIFO less than half full (pending)
			begin // !eth_rd_cmd_en prevents two cmds in a row, allows fifo_cnt latency
			if (eth_rd_cntdn[8:4] == 0) // <= 16: this will be the last read request 
				begin
				eth_rd_cmd_bl   <= eth_rd_cntdn[3:0]; // actual burst length is bl +1
				ETH_RD_BUSY     <= 0; // clear if done
				ETH_RD_DONE     <= 1; // set if done
				end
			else // request 16 more from this line
				begin
				eth_rd_cmd_bl   <= 6'd15; // 16: actual burst length is bl +1
				end
			eth_rd_x        <= eth_rd_x + 16; // lower 4 bits are always 0!
			eth_rd_cntdn    <=  eth_rd_cntdn -16; // last 4 bits constant!
			eth_rd_cmd_addr <= {eth_rd_x[11:10], eth_rd_y[11:0], ETH_RD_FRAME_SEL, eth_rd_x[9:0], 2'b00};
			eth_rd_cmd_en   <= 1; // issue the command
			end
		else // not time to request more
			begin
			eth_rd_cmd_en   <= 0;
			end
		// count how many are in the FIFO or pending
//		eth_rd_fifo_cnt <= 
//			!eth_rd_en &&  eth_rd_cmd_en ? eth_rd_fifo_cnt +16 : // just burst
//			 eth_rd_en &&  eth_rd_cmd_en ? eth_rd_fifo_cnt +15 : // burst minus read
//			 eth_rd_en && !eth_rd_cmd_en ? eth_rd_fifo_cnt -1  : // just read
//			                               eth_rd_fifo_cnt +0  ; // nothing
		eth_rd_fifo_cnt <= 
			!eth_rd_en_prev &&  eth_rd_cmd_en ? eth_rd_fifo_cnt +16 : // just burst
			 eth_rd_en_prev &&  eth_rd_cmd_en ? eth_rd_fifo_cnt +15 : // burst minus read
			 eth_rd_en_prev && !eth_rd_cmd_en ? eth_rd_fifo_cnt -1  : // just read
			                                    eth_rd_fifo_cnt +0  ; // nothing
		end
// FIFO to buffer up read pixels - spearate from async FIFO - bigger
(* RAM_STYLE="AUTO" *)
reg [23:0] eth_rd_fifo_ram [511:0];
reg 	[8:0]	eth_rd_fifo_wr_adr=0, eth_rd_fifo_rd_adr=0;
always @(posedge ETH_RD_CLK)
	begin
//	eth_rd_fifo_wr_adr <= ETH_RD_GO ? 0 : eth_rd_en       ? eth_rd_fifo_wr_adr+1 : eth_rd_fifo_wr_adr;
	eth_rd_fifo_wr_adr <= ETH_RD_GO ? 0 : eth_rd_en_prev  ? eth_rd_fifo_wr_adr+1 : eth_rd_fifo_wr_adr;
	eth_rd_fifo_rd_adr <= ETH_RD_GO ? 0 : ETH_RD_STB      ? eth_rd_fifo_rd_adr+1 : eth_rd_fifo_rd_adr;
//	if (eth_rd_en)      eth_rd_fifo_ram[eth_rd_fifo_wr_adr] <= eth_rd_data[23:0]; 
	if (eth_rd_en_prev) eth_rd_fifo_ram[eth_rd_fifo_wr_adr] <= eth_rd_dat_prev[23:0]; 
	ETH_RD_PIX  <= eth_rd_fifo_ram[eth_rd_fifo_rd_adr];
	ETH_RD_READY <= (eth_rd_fifo_wr_adr == (ETH_RD_NUM_PIXELS -1)) && eth_rd_en_prev; // full packet has come in
	end


//------- Processing Read Port - Reads original image -------
reg 	[11:0]	proc_rd_cntdn = 0; // pixels left in line, -1
reg 	[5:0]	proc_rd_fifo_add = 0; // number to add due to burst
reg 	[6:0]	proc_rd_fifo_cnt = 0;
reg 	[11:0]	proc_rd_y, proc_rd_x;
reg 	[11:0]	proc_rd_last_x, proc_rd_last_y;
// read data path
wire			proc_rd_en = PROC_RD_STB; // FIFO rd strobe
wire	[31:0]	proc_rd_data; // FIFO rd data
assign PROC_RD_PIX = proc_rd_data[23:0];
wire	[6:0]	proc_rd_count; // used to start READY
reg 			proc_rd_ready_ok = 0;
always @(posedge PROC_RD_CLK) proc_rd_ready_ok <= PROC_RD_GO ? 0 : proc_rd_count[5] ? 1 : proc_rd_ready_ok;
wire			proc_rd_empty; // FIFO empty flag
assign PROC_RD_READY = !proc_rd_empty && proc_rd_ready_ok; // FIFO not empty, and enough buffered up to start
// generate command 
reg 	[29:0]	proc_rd_cmd_addr;
reg 	[5:0]	proc_rd_cmd_bl; 
reg 			proc_rd_cmd_en = 0;
wire	[2:0]	proc_rd_cmd_instr = 3'b001; // read, no AP

always @(posedge PROC_RD_CLK)
	if (!PROC_RD_BUSY) // in between frames
		begin
		if(PROC_RD_GO) 
			begin
			proc_rd_fifo_cnt <= 0; // should aready be 0!!! 
			proc_rd_last_x   <= PROC_RD_WIDTH -1;
			proc_rd_last_y   <= PROC_RD_HEIGHT -1;
			proc_rd_cntdn    <= PROC_RD_WIDTH -1;
			PROC_RD_BUSY     <= 1;
			end
		proc_rd_y        <= 0; // start at (0,0)
		proc_rd_x        <= 0;
		proc_rd_cmd_en   <= 0;
		PROC_RD_DONE     <= 0;
		proc_rd_fifo_add <= 0;
		end
	else // reading a frame
		begin
		if ((proc_rd_fifo_cnt[6:5] == 2'b00) && !proc_rd_cmd_en) // Request more: FIFO less than half full (pending)
			begin // !proc_rd_cmd_en prevents two cmds in a row, allows fifo_cnt latency
			if (proc_rd_cntdn[11:4] == 0) // <= 16: this will be the last read request of this line
				begin
				proc_rd_cntdn    <= proc_rd_last_x; // reset countdown
				proc_rd_cmd_bl   <= proc_rd_cntdn[3:0]; // actual burst length is bl +1
				proc_rd_fifo_add <= proc_rd_cntdn[3:0] +1; // add to counter
				proc_rd_x        <= 0;
				proc_rd_y        <= proc_rd_y +1; // next line
				PROC_RD_BUSY     <= (proc_rd_y != proc_rd_last_y); // clear if done
				PROC_RD_DONE     <= (proc_rd_y == proc_rd_last_y); // set if done
				end
			else // request 16 more from this line
				begin
				proc_rd_cntdn    <=  proc_rd_cntdn -16; // last 4 bits constant!
				proc_rd_cmd_bl   <= 6'd15; // 16: actual burst length is bl +1
				proc_rd_fifo_add <= 5'd16; // add 16 to counter
				proc_rd_x        <= proc_rd_x + 16; // lower 4 bits are always 0!
				end
			proc_rd_cmd_addr <= {proc_rd_x[11:10], proc_rd_y[11:0], 1'b0, proc_rd_x[9:0], 2'b00};
			proc_rd_cmd_en   <= 1; // issue the command
			end
		else // not time to request more
			begin
			proc_rd_cmd_en   <= 0;
			proc_rd_fifo_add <= 0;
			proc_rd_cmd_bl   <= -1; // to allow simple adder for proc_rd_fifo_cnt
			end
		// count how many are in the FIFO or pending
		//proc_rd_fifo_cnt <= PROC_RD_STB ? proc_rd_fifo_cnt + proc_rd_fifo_add -1 : 
		//                                  proc_rd_fifo_cnt + proc_rd_fifo_add; 
		proc_rd_fifo_cnt <= PROC_RD_STB ? proc_rd_fifo_cnt + proc_rd_cmd_bl :
		                                  proc_rd_fifo_cnt + proc_rd_fifo_add; // mux is faster than adder?
		end


//------- Processing Write Port - writes processed image -------
reg 	[11:0]	proc_wr_cntdn = 0;
reg 	[3:0]	proc_wr_fifo_cnt = 0;
reg 	[11:0]	proc_wr_y, proc_wr_x;
reg 	[11:0]	proc_wr_last_x, proc_wr_last_y;
// write data path
wire	[31:0]	proc_wr_data = {8'h00, PROC_WR_PIX[23:0]};
wire			proc_wr_en = PROC_WR_BUSY && PROC_WR_STB; // ignore if not BUSY
wire	[3:0]	proc_wr_mask = 4'b0000; // write all bytes always
// generate command 
reg 	[29:0]	proc_wr_cmd_addr;
reg 	[5:0]	proc_wr_cmd_bl; 
reg 			proc_wr_cmd_en = 0;
wire	[2:0]	proc_wr_cmd_instr = 3'b000; // write, no AP

always @(posedge PROC_WR_CLK)
	if (!PROC_WR_BUSY && PROC_WR_GO) // ignore GO if busy (protection)
		begin
		proc_wr_fifo_cnt <= 0; // should aready be 0!!! 
		proc_wr_last_x   <= PROC_WR_WIDTH -1;
		proc_wr_last_y   <= PROC_WR_HEIGHT -1;
		proc_wr_cntdn    <= PROC_WR_WIDTH -1;
		proc_wr_y        <= 0; // start at (0,0)
		proc_wr_x        <= 0;
		PROC_WR_BUSY     <= 1;
		PROC_WR_DONE     <= 0;
		proc_wr_cmd_en   <= 0;
		end
	else if (PROC_WR_BUSY && PROC_WR_STB) // pixel written to write datapath, ignore if not busy
		begin
		if (proc_wr_cntdn == 0) // end of line, burst remainder
			begin
			proc_wr_cntdn    <= proc_wr_last_x; // reset countdown
			proc_wr_cmd_addr <= {proc_wr_x[11:10], proc_wr_y[11:0], 1'b1, proc_wr_x[9:0], 2'b00};
			proc_wr_cmd_bl   <= proc_wr_fifo_cnt; // actual burst length is bl +1
			proc_wr_cmd_en   <= 1; // issue the command
			proc_wr_fifo_cnt <= 0; // reset the count
			proc_wr_y        <= proc_wr_y +1;
			proc_wr_x        <= 0;
			PROC_WR_BUSY     <= (proc_wr_y != proc_wr_last_y); // clear if done
			PROC_WR_DONE     <= (proc_wr_y == proc_wr_last_y); // set if done
			end
		else if (proc_wr_fifo_cnt == 4'd15)// middle of line, enough for a burst of 16
			begin
			proc_wr_cntdn    <=  proc_wr_cntdn -1; 
			proc_wr_cmd_addr <= {proc_wr_x[11:10], proc_wr_y[11:0], 1'b1, proc_wr_x[9:0], 2'b00};
			proc_wr_cmd_bl   <= proc_wr_fifo_cnt; // actual burst length is bl +1
			proc_wr_cmd_en   <= 1; // issue the command
			proc_wr_fifo_cnt <= 0; // reset the count
			proc_wr_x        <= proc_wr_x + 16; // lower 4 bits are always 0!
			end
		else // not time for a burst
			begin
			proc_wr_cntdn    <=  proc_wr_cntdn -1; 
			proc_wr_cmd_en   <= 0;
			proc_wr_fifo_cnt <= proc_wr_fifo_cnt +1; // count how many are in the FIFO
			end
		end
	else
		begin
		proc_wr_cmd_en   <= 0;
		PROC_WR_DONE     <= 0;
		end

 
 //------- DVI Read Port - Reads original image -------
wire	[31:0]	dvi_rd_data; // FIFO rd data
wire			dvi_rd_empty; // FIFO empty flag
wire			full;
wire			dvi_rd_en = !dvi_rd_empty&&!full; // FIFO rd strobe (for MCB fifo)
wire			dvi_fifo_empty;
assign 			DVI_RD_READY = !dvi_fifo_empty; // FIFO not empty, and enough buffered up to start
dvi_pix_fifo dvi_fifo (
	.clk(DVI_RD_CLK),
	.rst(DVI_RD_GO),
	.din(dvi_rd_data[23:0]), // Bus [23 : 0] 
	.wr_en(dvi_rd_en),
	.rd_en(DVI_RD_STB),
	.dout(DVI_RD_PIX), // Bus [23 : 0] 
	.full(full),
	.empty(dvi_fifo_empty),
	.data_count()); // Bus [9 : 0]  

 
reg 	[11:0]	dvi_rd_cntdn = 0; // pixels left in line, -1
reg 	[5:0]	dvi_rd_fifo_add = 0; // number to add due to burst
reg 	[6:0]	dvi_rd_fifo_cnt = 0;
reg 	[11:0]	dvi_rd_y, dvi_rd_x;
reg 	[11:0]	dvi_rd_last_x, dvi_rd_last_y;
// read data path
wire	[6:0]	dvi_rd_count; // used to start READY
wire 	      	calib_done;

// generate command 
reg 	[29:0]	dvi_rd_cmd_addr;
reg 	[8:0]	dvi_rd_cmd_bl; 
reg 			dvi_rd_cmd_en = 0;
wire	[2:0]	dvi_rd_cmd_instr = 3'b001; // read, no AP
always @(posedge DVI_RD_CLK)
	if (!DVI_RD_BUSY||!calib_done) // in between frames
		begin
		if(DVI_RD_GO&&calib_done) 
			begin
			dvi_rd_fifo_cnt <= 0; // should aready be 0!!! 
			dvi_rd_last_x   <= DVI_RD_WIDTH -1;
			dvi_rd_last_y   <= DVI_RD_HEIGHT -1;
			dvi_rd_cntdn    <= DVI_RD_WIDTH -1;
			DVI_RD_BUSY     <= 1;
			end
		dvi_rd_y        <= 0; // start at (0,0)
		dvi_rd_x        <= 0;
		dvi_rd_cmd_en   <= 0;
		DVI_RD_DONE     <= 0;
		dvi_rd_fifo_add <= 0;
		end
	else // reading a frame
		begin
		if ((dvi_rd_fifo_cnt[6:5] == 2'b00) && !dvi_rd_cmd_en) // Request more: FIFO less than half full (pending)
			begin // !dvi_rd_cmd_en prevents two cmds in a row, allows fifo_cnt latency
			if (dvi_rd_cntdn[11:4] == 0) // <= 16: this will be the last read request of this line
				begin
				dvi_rd_cntdn    <= dvi_rd_last_x; // reset countdown
				dvi_rd_cmd_bl   <= dvi_rd_cntdn[3:0]; // actual burst length is bl +1
				dvi_rd_fifo_add <= dvi_rd_cntdn[3:0] +1; // add to counter
				dvi_rd_x        <= 0;
				dvi_rd_y        <= dvi_rd_y +1; // next line
				DVI_RD_BUSY     <= (dvi_rd_y != dvi_rd_last_y); // clear if done
				DVI_RD_DONE     <= (dvi_rd_y == dvi_rd_last_y); // set if done
				end
			else // request 16 more from this line
				begin
				dvi_rd_cntdn    <=  dvi_rd_cntdn -16; // last 4 bits constant!
				dvi_rd_cmd_bl   <= 9'd15; // 16: actual burst length is bl +1
				dvi_rd_fifo_add <= 6'd16; // add 16 to counter
				dvi_rd_x        <= dvi_rd_x + 16; // lower 4 bits are always 0!
				end
			dvi_rd_cmd_addr <= {dvi_rd_x[11:10], dvi_rd_y[11:0], DVI_RD_FRAME_SEL, dvi_rd_x[9:0], 2'b00};
			dvi_rd_cmd_en   <= 1; // issue the command
			end
		else // not time to request more
			begin
			dvi_rd_cmd_en   <= 0;
			dvi_rd_fifo_add <= 0;
			dvi_rd_cmd_bl   <= -1; // to allow simple adder for dvi_rd_fifo_cnt
			end
		// count how many are in the FIFO or pending
		//dvi_rd_fifo_cnt <= DVI_RD_STB ? dvi_rd_fifo_cnt + dvi_rd_fifo_add -1 : 
		//                                  dvi_rd_fifo_cnt + dvi_rd_fifo_add; 
		dvi_rd_fifo_cnt <= dvi_rd_en ? dvi_rd_fifo_cnt + dvi_rd_cmd_bl :
		                                  dvi_rd_fifo_cnt + dvi_rd_fifo_add; // mux is faster than adder?
		end
 

 
wire	rd_error; 
/*
 wire	[35:0]	control0;
cs_icon tp_icon (
    .CONTROL0(control0) // INOUT BUS [35:0]
);

cs_ila tp_ila (
    .CONTROL(control0), // INOUT BUS [35:0]
    .CLK(DVI_RD_CLK), // IN
    .TRIG0({dvi_rd_cmd_bl,dvi_rd_x,DVI_RD_STB,DVI_RD_READY,DVI_RD_GO,DVI_RD_DONE,DVI_RD_BUSY,dvi_rd_count,dvi_rd_cmd_en,calib_done}) // IN BUS [31:0]
);
*/ 
 
 
   //----------------------------------------------------------------------------------------
   //
   //          Clocking
   //
   //----------------------------------------------------------------------------------------

// PLL is outside this module (as it may be shared with other logic)
   
// Drive PLL clocks to MCB3 glock nets
   wire 	      mcb3_sysclk_2x;      
   wire 	      mcb3_sysclk_2x_180;
   wire 	      mcb3_pll_ce_0;
   wire 	      mcb3_pll_ce_90;

   BUFPLL_MCB BUFPLL_MCB3
     ( .IOCLK0         (mcb3_sysclk_2x),
       .IOCLK1         (mcb3_sysclk_2x_180),
       .GCLK           (CALIB_CLK),		// gclk
       .LOCK           (MCB_PLL_LOCK),		// pll_lock
       .LOCKED         (MCBCLK_PLL_LOCK),	// pll_locked
       .SERDESSTROBE0  (mcb3_pll_ce_0),
       .SERDESSTROBE1  (mcb3_pll_ce_90),
       .PLLIN0         (MCBCLK_2X_0),
       .PLLIN1         (MCBCLK_2X_180)
       );

  //***************************************************************************
  // Reset synchronization
  // NOTES:
  //   1. shut down the whole operation if the PLL hasn't yet locked (and
  //      by inference, this means that external SYS_RST_IN has been asserted -
  //      PLL deasserts LOCKED as soon as SYS_RST_IN asserted)
  //   2. asynchronously assert reset. This was we can assert reset even if
  //      there is no clock (needed for things like 3-stating output buffers).
  //      reset deassertion is synchronous.
  //***************************************************************************


  // # of clock cycles to delay deassertion of reset. Needs to be a fairly
  // high number not so much for metastability protection, but to give time
  // for reset (i.e. stable clock cycles) to propagate through all state
  // machines and to all control signals (i.e. not all control signals have
  // resets, instead they rely on base state logic being reset, and the effect
  // of that reset propagating through the logic). Need this because we may not
  // be getting stable clock cycles while reset asserted (i.e. since reset
  // depends on PLL/DCM lock status)
  localparam RST_SYNC_NUM = 25;

   wire    rst_tmp = RST | ~MCBCLK_PLL_LOCK;  // Reset signal (RST) gated by PLL lock
   reg [RST_SYNC_NUM-1:0] rst0_sync_r    /* synthesis syn_maxfan = 10 */;  // Reset pipeline used to reduce timing paths
   // synthesis attribute max_fanout of rst0_sync_r is 10
   always @(posedge RSTCLK or posedge rst_tmp) // FIXME is CALIB_CLK good choice?  We have various clocks...
     if (rst_tmp)
       rst0_sync_r <= {RST_SYNC_NUM{1'b1}};
     else
       // logical left shift by one (pads with 0)
       rst0_sync_r <= rst0_sync_r << 1;

   wire 		  mcb_rst    = rst0_sync_r[RST_SYNC_NUM-1];



   //----------------------------------------------------------------------------------------
   //
   //  MCB Instantiation
   //   
   //----------------------------------------------------------------------------------------   

   localparam C3_ARB_NUM_TIME_SLOTS   = 12;       
//   localparam C3_ARB_TIME_SLOT_0      = 18'o012345;       
//   localparam C3_ARB_TIME_SLOT_1      = 18'o123450;       
//   localparam C3_ARB_TIME_SLOT_2      = 18'o234501;       
//   localparam C3_ARB_TIME_SLOT_3      = 18'o345012;       
//   localparam C3_ARB_TIME_SLOT_4      = 18'o450123;       
//   localparam C3_ARB_TIME_SLOT_5      = 18'o501234;       
//   localparam C3_ARB_TIME_SLOT_6      = 18'o012345;       
//   localparam C3_ARB_TIME_SLOT_7      = 18'o123450;       
//   localparam C3_ARB_TIME_SLOT_8      = 18'o234501;       
//   localparam C3_ARB_TIME_SLOT_9      = 18'o345012;       
//   localparam C3_ARB_TIME_SLOT_10     = 18'o450123;       
//   localparam C3_ARB_TIME_SLOT_11     = 18'o501234;       

   localparam C3_ARB_TIME_SLOT_0      = 18'o541023;       
   localparam C3_ARB_TIME_SLOT_1      = 18'o451023;       
   localparam C3_ARB_TIME_SLOT_2      = 18'o541023;       
   localparam C3_ARB_TIME_SLOT_3      = 18'o451023;       
   localparam C3_ARB_TIME_SLOT_4      = 18'o541023;       
   localparam C3_ARB_TIME_SLOT_5      = 18'o451023;       
   localparam C3_ARB_TIME_SLOT_6      = 18'o541023;       
   localparam C3_ARB_TIME_SLOT_7      = 18'o451023;       
   localparam C3_ARB_TIME_SLOT_8      = 18'o541023;       
   localparam C3_ARB_TIME_SLOT_9      = 18'o451023;       
   localparam C3_ARB_TIME_SLOT_10     = 18'o541023;       
   localparam C3_ARB_TIME_SLOT_11     = 18'o451023;

   localparam C3_MEM_TRAS             = 37500;       
   localparam C3_MEM_TRCD             = 13130;       
   localparam C3_MEM_TREFI            = 7800000;       
   localparam C3_MEM_TRFC             = 110000;       
   localparam C3_MEM_TRP              = 13130;       
   localparam C3_MEM_TWR              = 15000;       
   localparam C3_MEM_TRTP             = 7500;       
   localparam C3_MEM_TWTR             = 7500;       
   localparam C3_MEM_TYPE             = "DDR3";       
   localparam C3_MEM_DENSITY          = "1Gb";       
   localparam C3_MEM_BURST_LEN        = 8;       
   localparam C3_MEM_CAS_LATENCY      = 6;       
   localparam C3_MEM_NUM_COL_BITS     = 10;       
   localparam C3_MEM_DDR1_2_ODS       = "FULL";       
   localparam C3_MEM_DDR2_RTT         = "50OHMS";       
   localparam C3_MEM_DDR2_DIFF_DQS_EN  = "YES";       
   localparam C3_MEM_DDR2_3_PA_SR     = "FULL";       
   localparam C3_MEM_DDR2_3_HIGH_TEMP_SR  = "NORMAL";       
   localparam C3_MEM_DDR3_CAS_LATENCY  = 6;       
   localparam C3_MEM_DDR3_ODS         = "DIV6";       
   localparam C3_MEM_DDR3_RTT         = "DIV4";       
   localparam C3_MEM_DDR3_CAS_WR_LATENCY  = 5;       
   localparam C3_MEM_DDR3_AUTO_SR     = "ENABLED";       
   localparam C3_MEM_DDR3_DYN_WRT_ODT  = "OFF";       
   localparam C3_MEM_MOBILE_PA_SR     = "FULL";       
   localparam C3_MEM_MDDR_ODS         = "FULL";       
   localparam C3_MC_CALIBRATION_RA    = 16'h0000;       
   localparam C3_MC_CALIBRATION_BA    = 4'h0;       
   localparam C3_MC_CALIBRATION_CA    = 12'h000;       
   localparam C3_MC_CALIBRATION_MODE  = "NOCALIBRATION";       
   localparam C3_MC_CALIBRATION_DELAY  = "HALF";       
   localparam C3_SKIP_IN_TERM_CAL     = 1;       
   localparam C3_SKIP_DYNAMIC_CAL     = 0;       
   localparam C3_LDQSP_TAP_DELAY_VAL  = 0;       
   localparam C3_LDQSN_TAP_DELAY_VAL  = 0;       
   localparam C3_UDQSP_TAP_DELAY_VAL  = 0;       
   localparam C3_UDQSN_TAP_DELAY_VAL  = 0;       
   localparam C3_DQ0_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ1_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ2_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ3_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ4_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ5_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ6_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ7_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ8_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ9_TAP_DELAY_VAL    = 0;       
   localparam C3_DQ10_TAP_DELAY_VAL   = 0;       
   localparam C3_DQ11_TAP_DELAY_VAL   = 0;       
   localparam C3_DQ12_TAP_DELAY_VAL   = 0;       
   localparam C3_DQ13_TAP_DELAY_VAL   = 0;       
   localparam C3_DQ14_TAP_DELAY_VAL   = 0;       
   localparam C3_DQ15_TAP_DELAY_VAL   = 0;       
   localparam C3_p0_BEGIN_ADDRESS                   = (C3_SIMULATION == "FALSE") ? 32'h01000000:32'h00000100;
   localparam C3_p0_DATA_MODE                       = 4'b0010;
   localparam C3_p0_END_ADDRESS                     = (C3_SIMULATION == "FALSE") ? 32'h02ffffff:32'h000002ff;
   localparam C3_p0_PRBS_EADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'hfc000000:32'hfffffc00;
   localparam C3_p0_PRBS_SADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'h01000000:32'h00000100;
   localparam C3_p1_BEGIN_ADDRESS                   = (C3_SIMULATION == "FALSE") ? 32'h03000000:32'h00000300;
   localparam C3_p1_DATA_MODE                       = 4'b0010;
   localparam C3_p1_END_ADDRESS                     = (C3_SIMULATION == "FALSE") ? 32'h04ffffff:32'h000004ff;
   localparam C3_p1_PRBS_EADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'hf8000000:32'hfffff800;
   localparam C3_p1_PRBS_SADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'h03000000:32'h00000300;
   localparam C3_p2_BEGIN_ADDRESS                   = (C3_SIMULATION == "FALSE") ? 32'h05000000:32'h00000500;
   localparam C3_p2_DATA_MODE                       = 4'b0010;
   localparam C3_p2_END_ADDRESS                     = (C3_SIMULATION == "FALSE") ? 32'h06ffffff:32'h000006ff;
   localparam C3_p2_PRBS_EADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'hf8000000:32'hfffff800;
   localparam C3_p2_PRBS_SADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'h05000000:32'h00000500;
   localparam C3_p3_BEGIN_ADDRESS                   = (C3_SIMULATION == "FALSE") ? 32'h01000000:32'h00000100;
   localparam C3_p3_DATA_MODE                       = 4'b0010;
   localparam C3_p3_END_ADDRESS                     = (C3_SIMULATION == "FALSE") ? 32'h02ffffff:32'h000002ff;
   localparam C3_p3_PRBS_EADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'hfc000000:32'hfffffc00;
   localparam C3_p3_PRBS_SADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'h01000000:32'h00000100;
   localparam C3_p4_BEGIN_ADDRESS                   = (C3_SIMULATION == "FALSE") ? 32'h05000000:32'h00000500;
   localparam C3_p4_DATA_MODE                       = 4'b0010;
   localparam C3_p4_END_ADDRESS                     = (C3_SIMULATION == "FALSE") ? 32'h06ffffff:32'h000006ff;
   localparam C3_p4_PRBS_EADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'hf8000000:32'hfffff800;
   localparam C3_p4_PRBS_SADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'h05000000:32'h00000500;
   localparam C3_p5_BEGIN_ADDRESS                   = (C3_SIMULATION == "FALSE") ? 32'h03000000:32'h00000300;
   localparam C3_p5_DATA_MODE                       = 4'b0010;
   localparam C3_p5_END_ADDRESS                     = (C3_SIMULATION == "FALSE") ? 32'h04ffffff:32'h000004ff;
   localparam C3_p5_PRBS_EADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'hf8000000:32'hfffff800;
   localparam C3_p5_PRBS_SADDR_MASK_POS             = (C3_SIMULATION == "FALSE") ? 32'h03000000:32'h00000300;

   // Generate local MCB related parameters.
   localparam C_BUFPLL_0_LOCK_SRC         = "LOCK_TO_0";

   memc3_wrapper #
     (
//   .C_CLK_PERIOD                     (C3_CLK_PERIOD),
   .C_CALIB_SOFT_IP                  (C3_CALIB_SOFT_IP),
   .C_ARB_NUM_TIME_SLOTS             (C3_ARB_NUM_TIME_SLOTS),
   .C_ARB_TIME_SLOT_0                (C3_ARB_TIME_SLOT_0),
   .C_ARB_TIME_SLOT_1                (C3_ARB_TIME_SLOT_1),
   .C_ARB_TIME_SLOT_2                (C3_ARB_TIME_SLOT_2),
   .C_ARB_TIME_SLOT_3                (C3_ARB_TIME_SLOT_3),
   .C_ARB_TIME_SLOT_4                (C3_ARB_TIME_SLOT_4),
   .C_ARB_TIME_SLOT_5                (C3_ARB_TIME_SLOT_5),
   .C_ARB_TIME_SLOT_6                (C3_ARB_TIME_SLOT_6),
   .C_ARB_TIME_SLOT_7                (C3_ARB_TIME_SLOT_7),
   .C_ARB_TIME_SLOT_8                (C3_ARB_TIME_SLOT_8),
   .C_ARB_TIME_SLOT_9                (C3_ARB_TIME_SLOT_9),
   .C_ARB_TIME_SLOT_10               (C3_ARB_TIME_SLOT_10),
   .C_ARB_TIME_SLOT_11               (C3_ARB_TIME_SLOT_11),
   .C_MEM_TRAS                       (C3_MEM_TRAS),
   .C_MEM_TRCD                       (C3_MEM_TRCD),
   .C_MEM_TREFI                      (C3_MEM_TREFI),
   .C_MEM_TRFC                       (C3_MEM_TRFC),
   .C_MEM_TRP                        (C3_MEM_TRP),
   .C_MEM_TWR                        (C3_MEM_TWR),
   .C_MEM_TRTP                       (C3_MEM_TRTP),
   .C_MEM_TWTR                       (C3_MEM_TWTR),
   .C_MEM_ADDR_ORDER                 (C3_MEM_ADDR_ORDER),
   .C_NUM_DQ_PINS                    (C3_NUM_DQ_PINS),
   .C_MEM_TYPE                       (C3_MEM_TYPE),
   .C_MEM_DENSITY                    (C3_MEM_DENSITY),
   .C_MEM_BURST_LEN                  (C3_MEM_BURST_LEN),
   .C_MEM_CAS_LATENCY                (C3_MEM_CAS_LATENCY),
   .C_MEM_ADDR_WIDTH                 (C3_MEM_ADDR_WIDTH),
   .C_MEM_BANKADDR_WIDTH             (C3_MEM_BANKADDR_WIDTH),
   .C_MEM_NUM_COL_BITS               (C3_MEM_NUM_COL_BITS),
   .C_MEM_DDR1_2_ODS                 (C3_MEM_DDR1_2_ODS),
   .C_MEM_DDR2_RTT                   (C3_MEM_DDR2_RTT),
   .C_MEM_DDR2_DIFF_DQS_EN           (C3_MEM_DDR2_DIFF_DQS_EN),
   .C_MEM_DDR2_3_PA_SR               (C3_MEM_DDR2_3_PA_SR),
   .C_MEM_DDR2_3_HIGH_TEMP_SR        (C3_MEM_DDR2_3_HIGH_TEMP_SR),
   .C_MEM_DDR3_CAS_LATENCY           (C3_MEM_DDR3_CAS_LATENCY),
   .C_MEM_DDR3_ODS                   (C3_MEM_DDR3_ODS),
   .C_MEM_DDR3_RTT                   (C3_MEM_DDR3_RTT),
   .C_MEM_DDR3_CAS_WR_LATENCY        (C3_MEM_DDR3_CAS_WR_LATENCY),
   .C_MEM_DDR3_AUTO_SR               (C3_MEM_DDR3_AUTO_SR),
   .C_MEM_DDR3_DYN_WRT_ODT           (C3_MEM_DDR3_DYN_WRT_ODT),
   .C_MEM_MOBILE_PA_SR               (C3_MEM_MOBILE_PA_SR),
   .C_MEM_MDDR_ODS                   (C3_MEM_MDDR_ODS),
   .C_MC_CALIB_BYPASS                (C3_MC_CALIB_BYPASS),
//   .C_MC_CALIBRATION_RA              (C3_MC_CALIBRATION_RA),
//   .C_MC_CALIBRATION_BA              (C3_MC_CALIBRATION_BA),
//   .C_MC_CALIBRATION_CA              (C3_MC_CALIBRATION_CA),
   .C_MC_CALIBRATION_MODE            (C3_MC_CALIBRATION_MODE),
   .C_MC_CALIBRATION_DELAY           (C3_MC_CALIBRATION_DELAY),
   .C_SKIP_IN_TERM_CAL               (C3_SKIP_IN_TERM_CAL),
   .C_SKIP_DYNAMIC_CAL               (C3_SKIP_DYNAMIC_CAL),
   .C_LDQSP_TAP_DELAY_VAL            (C3_LDQSP_TAP_DELAY_VAL),
   .C_LDQSN_TAP_DELAY_VAL            (C3_LDQSN_TAP_DELAY_VAL),
   .C_UDQSP_TAP_DELAY_VAL            (C3_UDQSP_TAP_DELAY_VAL),
   .C_UDQSN_TAP_DELAY_VAL            (C3_UDQSN_TAP_DELAY_VAL),
   .C_DQ0_TAP_DELAY_VAL              (C3_DQ0_TAP_DELAY_VAL),
   .C_DQ1_TAP_DELAY_VAL              (C3_DQ1_TAP_DELAY_VAL),
   .C_DQ2_TAP_DELAY_VAL              (C3_DQ2_TAP_DELAY_VAL),
   .C_DQ3_TAP_DELAY_VAL              (C3_DQ3_TAP_DELAY_VAL),
   .C_DQ4_TAP_DELAY_VAL              (C3_DQ4_TAP_DELAY_VAL),
   .C_DQ5_TAP_DELAY_VAL              (C3_DQ5_TAP_DELAY_VAL),
   .C_DQ6_TAP_DELAY_VAL              (C3_DQ6_TAP_DELAY_VAL),
   .C_DQ7_TAP_DELAY_VAL              (C3_DQ7_TAP_DELAY_VAL),
   .C_DQ8_TAP_DELAY_VAL              (C3_DQ8_TAP_DELAY_VAL),
   .C_DQ9_TAP_DELAY_VAL              (C3_DQ9_TAP_DELAY_VAL),
   .C_DQ10_TAP_DELAY_VAL             (C3_DQ10_TAP_DELAY_VAL),
   .C_DQ11_TAP_DELAY_VAL             (C3_DQ11_TAP_DELAY_VAL),
   .C_DQ12_TAP_DELAY_VAL             (C3_DQ12_TAP_DELAY_VAL),
   .C_DQ13_TAP_DELAY_VAL             (C3_DQ13_TAP_DELAY_VAL),
   .C_DQ14_TAP_DELAY_VAL             (C3_DQ14_TAP_DELAY_VAL),
   .C_DQ15_TAP_DELAY_VAL             (C3_DQ15_TAP_DELAY_VAL)
      )

   memc_wrapper_inst
     (
	  .mcb3_dram_dq                        (DDR3_DQ),
      .mcb3_dram_a                         (DDR3_A),
      .mcb3_dram_ba                        (DDR3_BA),
      .mcb3_dram_ras_n                     (DDR3_RAS_B),
      .mcb3_dram_cas_n                     (DDR3_CAS_B),
      .mcb3_dram_we_n                      (DDR3_WE_B),
      .mcb3_dram_odt                       (DDR3_ODT),
	  .mcb3_dram_reset_n                   (DDR3_RESET_N),
      .mcb3_dram_cke                       (DDR3_CKE),
   	  .mcb3_dram_dm                        (DDR3_LDM),
      .mcb3_dram_udqs                      (DDR3_UDQS_P),
      .mcb3_dram_udqs_n                    (DDR3_UDQS_N),
   	  .mcb3_dram_udm                       (DDR3_UDM),  	  
      .calib_done                          (calib_done),
      .async_rst                           (mcb_rst),
      .sysclk_2x                           (mcb3_sysclk_2x),
      .sysclk_2x_180                       (mcb3_sysclk_2x_180),
      .pll_ce_0                            (mcb3_pll_ce_0),
      .pll_ce_90                           (mcb3_pll_ce_90),
      .pll_lock                            (MCB_PLL_LOCK),
      .mcb_drp_clk                         (CALIB_CLK),
//	  .status                         		(??),
      .mcb3_dram_dqs                       (DDR3_LDQS_P),
      .mcb3_dram_dqs_n                     (DDR3_LDQS_N),
      .mcb3_dram_ck                       (DDR3_CLK_P),
      .mcb3_dram_ck_n                     (DDR3_CLK_N),       	  			

//    .rzq                                 (FPGA_ONCHIP_TERM2),
//    .zio                                 (FPGA_ONCHIP_TERM1),


      //User Port0 Interface Signals (Ethernet Write)
      .p0_cmd_clk(ETH_WR_CLK),
      .p0_cmd_en(eth_wr_cmd_en),
      .p0_cmd_instr(eth_wr_cmd_instr),
      .p0_cmd_bl(eth_wr_cmd_bl),
      .p0_cmd_byte_addr(eth_wr_cmd_addr),
      //.p0_cmd_empty(),
      //.p0_cmd_full(),

      // Data Wr Port signals
      .p0_wr_clk(ETH_WR_CLK),
      .p0_wr_en(eth_wr_en),
      .p0_wr_mask(eth_wr_mask),
      .p0_wr_data(eth_wr_data),
      //.p0_wr_full(),
      .p0_wr_empty(eth_wr_empty),
      //.p0_wr_count(),
      //.p0_wr_underrun(),
      //.p0_wr_error(),

      //Data Rd Port signals
      .p0_rd_clk(1'b0),
      .p0_rd_en(1'b0),
      //.p0_rd_data(),
      //.p0_rd_full(),
      //.p0_rd_empty(),
      //.p0_rd_count(),
      //.p0_rd_overflow(),
      //.p0_rd_error(),



	  //User Port1 Interface Signals (Processing Write)
      .p1_cmd_clk(PROC_WR_CLK),
      .p1_cmd_en(proc_wr_cmd_en),
      .p1_cmd_instr(proc_wr_cmd_instr),
      .p1_cmd_bl(proc_wr_cmd_bl),
      .p1_cmd_byte_addr(proc_wr_cmd_addr),
      //.p1_cmd_empty(),
      //.p1_cmd_full(),

      // Data Wr Port signals
      .p1_wr_clk(PROC_WR_CLK),
      .p1_wr_en(proc_wr_en),
      .p1_wr_mask(proc_wr_mask),
      .p1_wr_data(proc_wr_data),
      //.p1_wr_full(),
      //.p1_wr_empty(),
      //.p1_wr_count(),
      //.p1_wr_underrun(),
      //.p1_wr_error(),

      //Data Rd Port signals
      .p1_rd_clk(1'b0),
      .p1_rd_en(1'b0),
      //.p1_rd_data(),
      //.p1_rd_full(),
      //.p1_rd_empty(),
      //.p1_rd_count(),
      //.p1_rd_overflow(),
      //.p1_rd_error(),


      //User Port2 Interface Signals (Ethernet Read)

      .p2_cmd_clk(ETH_RD_CLK),
      .p2_cmd_en(eth_rd_cmd_en),
      .p2_cmd_instr(eth_rd_cmd_instr),
      .p2_cmd_bl(eth_rd_cmd_bl),
      .p2_cmd_byte_addr(eth_rd_cmd_addr),
      //.p2_cmd_empty(),
      //.p2_cmd_full(),

      //Data Rd Port signals
      .p2_rd_clk(ETH_RD_CLK),
      .p2_rd_en(eth_rd_en),
      .p2_rd_data(eth_rd_data),
      //.p2_rd_full(),
      .p2_rd_empty(eth_rd_empty),
      .p2_rd_count(eth_rd_count),
      //.p2_rd_overflow(),
      //.p2_rd_error(),

      //User Port3 Interface Signals (11.3 doesn't support using this port for reads)

      .p3_cmd_clk(CALIB_CLK),
      .p3_cmd_en(1'b0),
      .p3_cmd_instr(3'b0),
      .p3_cmd_bl(6'b0),
      .p3_cmd_byte_addr(30'b0),
      //.p3_cmd_empty(),
      //.p3_cmd_full(),

      //Data Rd Port signals
      .p3_rd_clk(1'b0),
      .p3_rd_en(1'b0),
      //.p3_rd_data(),
      //.p3_rd_full(),
      //.p3_rd_empty(),
      //.p3_rd_count(),
      //.p3_rd_overflow(),
      //.p3_rd_error(),
      
      //User Port4 Interface Signals (DVI Read)

      .p4_cmd_clk(DVI_RD_CLK),
      .p4_cmd_en(dvi_rd_cmd_en),
      .p4_cmd_instr(dvi_rd_cmd_instr),
      .p4_cmd_bl(dvi_rd_cmd_bl),
      .p4_cmd_byte_addr(dvi_rd_cmd_addr),
      //.p4_cmd_empty(),
      //.p4_cmd_full(),

      //Data Rd Port signals
      .p4_rd_clk(DVI_RD_CLK),
      .p4_rd_en(dvi_rd_en),
      .p4_rd_data(dvi_rd_data),
      //.p4_rd_full(),
      .p4_rd_empty(dvi_rd_empty),
      .p4_rd_count(dvi_rd_count),
      //.p4_rd_overflow(),
      //.p4_rd_error(),

      //User Port5 Interface Signals (Processing Read)

      .p5_cmd_clk(PROC_RD_CLK),
      .p5_cmd_en(proc_rd_cmd_en),
      .p5_cmd_instr(proc_rd_cmd_instr),
      .p5_cmd_bl(proc_rd_cmd_bl),
      .p5_cmd_byte_addr(proc_rd_cmd_addr),
      //.p5_cmd_empty(),
      //.p5_cmd_full(),

      //Data Rd Port signals
      .p5_rd_clk(PROC_RD_CLK),
      .p5_rd_en(proc_rd_en),
      .p5_rd_data(proc_rd_data),
      //.p5_rd_full(),
      .p5_rd_empty(proc_rd_empty),
      .p5_rd_count(proc_rd_count),
      //.p5_rd_overflow(),
      .p5_rd_error(rd_error),


      .selfrefresh_enter                   (selfrefresh_enter),
      .selfrefresh_mode                    (selfrefresh_mode)      
      );


endmodule
