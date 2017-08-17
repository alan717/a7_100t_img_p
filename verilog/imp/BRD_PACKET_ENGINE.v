`timescale  1 ns / 1 ns
//-------------------------------------
// BRD_PACKET_ENGINE.v
//-------------------------------------
// History of Changes:
//	06-08-2009 Initial creation
//-------------------------------------
// This module manages all of the ethernet communication with 
// the Base Reference Design Interface software.  This communication
// consists of the following packet types:
//
//	Control - various settings to control the processing (ie. coefficients)
//  Status - various status from FPGA to PC
//	Write - Image data to be written into the frame buffer
//  Read - Image data to be read from the frame buffer
//  User Control - Controls for user logic in the FPGA
// 
// When a packet is received a response acknowledge packet is generated
// to inform the PC that the packet was received.
// 
// Although doing all of this ethernet processing in logic
// is somewhat expensive, it allows for fast reading and writing
// of image data.
//-------------------------------------
module BRD_PACKET_ENGINE(

// Master Clock
input			CLK,

// Frame Buffer Write interface
input		[9:0]	IN_ETH_STREAM,
output	reg			ETH_BUSY=0, // Set when a command has been issued, cleared when ack has been sent
output		[9:0]	OUT_ETH_STREAM,
input				OUT_TICK,

// Filter Control Interface
output	reg [15:0]	CTRL_ENABLES = 16'h0000, 
output	reg	[15:0]	IMG_SIZE_X=0,
output	reg	[15:0]	IMG_SIZE_Y=0,
output	reg	[399:0]	CTRL_COEFF=0,
output	reg			CTRL_BUSY=0,
output	reg			CTRL_STRB=0,

// Status
input		[23:0]	PROCESS_TIME, 
input		[15:0]	TEMPERATURE,
input		[15:0]	V_VCCINT,
input		[15:0]	V_VCCAUX,
input		[7:0]	BUTTONS,

// Ethernet Write Port - writes original image
output	wire		ETH_WR_CLK,        // expect to be CLK125, but whatever
output	reg	[15:0]	ETH_WR_START_X=0,    // start X
output	reg	[15:0]	ETH_WR_LINE=0,       // Line to write to
output	reg	[15:0]	ETH_WR_NUM_PIXELS=0, // number of pixels to write
output	reg			ETH_WR_GO=0,         // strobe before set of writes - latches parameters
input 				ETH_WR_DONE,       // Strobes when done
input 				ETH_WR_BUSY,       // Asserted from GO to DONE
output	reg	[23:0]	ETH_WR_PIX=0,
output	wire		ETH_WR_STB,        // Start strobing in pixels after GO

// Ethernet Read Port - Reads processed image
output	wire		ETH_RD_CLK,        // expect to be CLK125, but whatever
output	reg	[15:0]	ETH_RD_START_X=0,    // start X
output	reg	[15:0]	ETH_RD_LINE=0,       // Line to read from
output	reg	[15:0]	ETH_RD_NUM_PIXELS=0, // number of pixels to read
output	reg			ETH_RD_GO=0,         // strobe before set of reads - latches parameters, starts
input		 		ETH_RD_DONE,       // Strobes when done
input	 			ETH_RD_BUSY,       // Asserted from GO to DONE
input				ETH_RD_READY,      // strobe that indicates that read FIFO has all data pixels ready
input		[23:0]	ETH_RD_PIX,
output	reg			ETH_RD_STB,        // Strobe to pull RD_PIX from FIFO

// User Controls and Status
output	reg	[7:0]	USER_CTRL_EN=0,    // 8 seperate enables controlled via GUI
output	reg	[15:0]	USER_CTRL=0,       // 16-bit value entered via GUI
input		[127:0]	USER_STATUS,	   // 16 bytes of status that reported back to GUI
output  reg			USER_CTRL_STB        // strobe to signal when user controls written

);

parameter	[7:0]	FIRMWARE_VERSION=8'hff;
parameter	[39:0]	BOARD_TYPE=40'h53_50_36_30_31; //"SP601"

// MAC addresses
parameter	SERVER_MAC	=	48'h00_10_8B_F1_F5_7D; 
parameter	MY_MAC		=	48'h00_10_8B_F1_F5_7E; 

// Opcoded for different packet types
parameter	OP_CONTROL		=	8'h01;
parameter	OP_STATUS		=	8'h02;
parameter	OP_WRITE		=	8'h03; 
parameter	OP_READ			=	8'h04;
parameter	OP_USER_CONTROL	=	8'h05;

parameter	MAX_PAYLOAD	=	8'd56; // # of payload bytes (non-image data) past the opcode field


//------- Receive Video packets -------
wire			in_cke = IN_ETH_STREAM[9];
wire			in_frm = IN_ETH_STREAM[8];
wire	[7:0]	in_dat = IN_ETH_STREAM[7:0];


// Byte Counter
reg 	[10:0] 	rx_cnt=0; 
always @(posedge CLK) if (in_cke) rx_cnt <= !in_frm ? 0 : rx_cnt +1;

// Use shift register to store packet bytes (no reason to be space sensitive!)
reg		[7:0]			rx_opcode=0;
reg		[MAX_PAYLOAD:1]	pyld_byte_en=0;
reg						opcode_is_control=0; //added for timing		
always @(posedge CLK)
	begin
	rx_opcode <= (in_cke&&(rx_cnt==10'd14))? in_dat[7:0] : rx_opcode;
	opcode_is_control <= in_cke&&(rx_cnt==10'd14)? (in_dat[7:0]==OP_CONTROL) : opcode_is_control;
	pyld_byte_en <= (in_cke)? {pyld_byte_en[MAX_PAYLOAD-1:1],(rx_cnt==10'd14)} : pyld_byte_en;
	end



/***** Received packet processing for each type: *****/
/* CTRL */
reg				snd_ctrl_ack=0;
always @(posedge CLK) 
	begin
//	if ((rx_opcode==OP_CONTROL)&&in_cke)
	if (opcode_is_control&&in_cke)
		begin
		CTRL_ENABLES[15:8] <= (pyld_byte_en[1])? in_dat : CTRL_ENABLES[15:8];
		CTRL_ENABLES[7:0] <= (pyld_byte_en[2])? in_dat : CTRL_ENABLES[7:0];	
		IMG_SIZE_X[15:8] <= (pyld_byte_en[3])? in_dat : IMG_SIZE_X[15:8];
		IMG_SIZE_X[7:0] <= (pyld_byte_en[4])? in_dat : IMG_SIZE_X[7:0];	
		IMG_SIZE_Y[15:8] <= (pyld_byte_en[5])? in_dat : IMG_SIZE_Y[15:8];
		IMG_SIZE_Y[7:0] <= (pyld_byte_en[6])? in_dat : IMG_SIZE_Y[7:0];		
		end
	CTRL_BUSY <= (in_cke&&(rx_cnt==10'd14)&&(in_dat[7:0]==OP_CONTROL))? 1'b1 : snd_ctrl_ack? 1'b0 : CTRL_BUSY;
	snd_ctrl_ack <= pyld_byte_en[MAX_PAYLOAD]&&(rx_opcode==OP_CONTROL)&&in_cke;
	CTRL_STRB <= snd_ctrl_ack;
	end

// Latch all of the filter Coefficients
genvar i;
generate
  for (i=49; i>=0; i=i-1) 
  begin: coeff_regs
  always @(posedge CLK)
//  	if ((rx_opcode==OP_CONTROL)&&in_cke)
	if ((opcode_is_control)&&in_cke)
  		CTRL_COEFF[i*8+7:i*8] <= (pyld_byte_en[(49-i)+7])? {in_dat} : CTRL_COEFF[i*8+7:i*8];
  end
endgenerate


/* STATUS */
reg				snd_status_ack=0;
always @(posedge CLK) 
	snd_status_ack <= pyld_byte_en[1]&&(rx_opcode==OP_STATUS)&&in_cke;

/* WRITE */	
assign			ETH_WR_CLK=CLK;
reg				wr_dat_en = 0;
reg		[2:0]	wr_1_every_3_sr=0;
reg		[10:0]	wr_cnt = 0;
//assign 			ETH_WR_STB = wr_1_every_3_sr[2];
assign 			ETH_WR_STB = wr_1_every_3_sr[2]&&(rx_opcode==OP_WRITE)&&in_cke; // issue strobe when full 24 bit pixel available
reg				snd_wr_ack=0;
always @(posedge CLK)
	begin 
	if ((rx_opcode==OP_WRITE)&&in_cke)
		begin
		ETH_WR_START_X[15:8] <= (pyld_byte_en[1])? in_dat : ETH_WR_START_X[15:8];
		ETH_WR_START_X[7:0] <= (pyld_byte_en[2])? in_dat : ETH_WR_START_X[7:0];	
		ETH_WR_LINE[15:8] <= (pyld_byte_en[3])? in_dat : ETH_WR_LINE[15:8];
		ETH_WR_LINE[7:0] <= (pyld_byte_en[4])? in_dat : ETH_WR_LINE[7:0];	
		ETH_WR_NUM_PIXELS[15:8] <= (pyld_byte_en[5])? in_dat : ETH_WR_NUM_PIXELS[15:8];
		ETH_WR_NUM_PIXELS[7:0] <= (pyld_byte_en[6])? in_dat : ETH_WR_NUM_PIXELS[7:0];
		wr_cnt <= (pyld_byte_en[6])? {ETH_WR_NUM_PIXELS[10:8],in_dat} : ((wr_cnt!=0)&&ETH_WR_STB)? wr_cnt-1 : wr_cnt;
		wr_dat_en <= (wr_cnt!=11'd0)? 1'b1 : 1'b0;
		wr_1_every_3_sr <= (!wr_dat_en)? 3'b001 : {wr_1_every_3_sr[1:0],wr_1_every_3_sr[2]}; // generate strobe every 3 cycles			
		ETH_WR_PIX <= {ETH_WR_PIX[15:0],in_dat};
		end
	snd_wr_ack <= (wr_cnt==11'd1)&&ETH_WR_STB&&(rx_opcode==OP_WRITE)&&in_cke; // signal ack after completed writing data
	ETH_WR_GO <= pyld_byte_en[6]&&(rx_opcode==OP_WRITE)&&in_cke;
	end
		
/* READ */
assign			ETH_RD_CLK=CLK;	
reg		[15:0]	rd_pyld_len=0; // calculated ethenet payload length field (can tig paths to this register)
reg		[15:0]	rd_tx_cnt=0; // calculated ethenet packet length field (can tig paths to this register)
reg				rd_rdy_lat=0; // set when ETH_RD_READY, cleared when snd_rd_ack
reg				snd_rd_ack=0;
reg		[2:0]	rd_1_every_3_sr=0;
reg		[10:0]	rd_cnt = 0;
wire			e_rd_stb=rd_1_every_3_sr[2]&&(rx_opcode==OP_READ)&&OUT_TICK;
always @(posedge CLK)
	begin 
	if ((rx_opcode==OP_READ)&&in_cke)
		begin
		ETH_RD_START_X[15:8] <= (pyld_byte_en[1])? in_dat : ETH_RD_START_X[15:8];
		ETH_RD_START_X[7:0] <= (pyld_byte_en[2])? in_dat : ETH_RD_START_X[7:0];	
		ETH_RD_LINE[15:8] <= (pyld_byte_en[3])? in_dat : ETH_RD_LINE[15:8];
		ETH_RD_LINE[7:0] <= (pyld_byte_en[4])? in_dat : ETH_RD_LINE[7:0];	
		ETH_RD_NUM_PIXELS[15:8] <= (pyld_byte_en[5])? in_dat : ETH_RD_NUM_PIXELS[15:8];
		ETH_RD_NUM_PIXELS[7:0] <= (pyld_byte_en[6])? in_dat : ETH_RD_NUM_PIXELS[7:0];
		rd_pyld_len <= ETH_RD_NUM_PIXELS*3+7; // adds in 7 rd fields that are echoed back
		rd_tx_cnt <= ETH_RD_NUM_PIXELS*3+18+7-1; // dst_mac(6) + src_mac(6) + len(2) + rd fields(7) + crc(4) -1
		end
	rd_rdy_lat <= ETH_RD_READY? 1'b1 : snd_rd_ack? 1'b0 : rd_rdy_lat; // save this until seen by out_tick
	snd_rd_ack <= rd_rdy_lat&&(rx_opcode==OP_READ)&&OUT_TICK&&!snd_rd_ack; // data ready to fill Tx packet
	ETH_RD_GO <= pyld_byte_en[6]&&(rx_opcode==OP_READ)&&in_cke;
	rd_cnt <= 	(pyld_byte_en[6]&&(rx_opcode==OP_READ)&&in_cke)? {ETH_RD_NUM_PIXELS[10:8],in_dat} : 
				((rd_cnt!=0)&&(e_rd_stb||snd_rd_ack))? rd_cnt-1 : rd_cnt;	
	ETH_RD_STB <= e_rd_stb||snd_rd_ack; // register this to help timing, have 2 extra cycles before updated ETH_RD_PIX is used 
	rd_1_every_3_sr <= 	(rd_cnt==0)? 3'b000 : 
						(snd_rd_ack)? 3'b001 : 
						(OUT_TICK)? {rd_1_every_3_sr[1:0],rd_1_every_3_sr[2]} : rd_1_every_3_sr; // generate strobe every 3 cycles
	end

/* USER CTRL */
reg				snd_user_ctrl_ack=0;
always @(posedge CLK) 
	begin
	if ((rx_opcode==OP_USER_CONTROL)&&in_cke)
		begin
		USER_CTRL_EN <= (pyld_byte_en[1])? in_dat : USER_CTRL_EN;
		USER_CTRL[15:8] <= (pyld_byte_en[2])? in_dat : USER_CTRL[15:8];
		USER_CTRL[7:0] <= (pyld_byte_en[3])? in_dat : USER_CTRL[7:0];	
		end
	snd_user_ctrl_ack <= pyld_byte_en[4]&&(rx_opcode==OP_USER_CONTROL)&&in_cke;
	USER_CTRL_STB <= snd_user_ctrl_ack;
	end



//------- Transmit Ack packets -------
reg 	[10:0]	tx_cnt = 0;
reg 			tx_frm = 0;
reg 	[567:0]	tx_shift=0; // holds up to 71 bytes of packet data
always @(posedge CLK)
	begin
	if (tx_cnt==11'd1) 
		ETH_BUSY <= 1'b0; // clear when almost done sending
	else if (in_cke&&(rx_cnt==10'd14)&&((in_dat==OP_CONTROL)||(in_dat==OP_STATUS)||(in_dat==OP_WRITE)||(in_dat==OP_READ)||(in_dat==OP_USER_CONTROL))) 
		ETH_BUSY <= 1'b1; // set when a new opcode received

	if (snd_ctrl_ack)
		begin
		tx_shift <= {SERVER_MAC, MY_MAC, 16'd57, OP_CONTROL+8'h80, CTRL_ENABLES,IMG_SIZE_X,IMG_SIZE_Y,CTRL_COEFF};
		tx_cnt <= 11'd74; // dst_mac(6) + src_mac(6) + len(2) + data(57) + crc(4) -1
		tx_frm <= 1;
		end
	else if (snd_status_ack)
		begin
		tx_shift <= {SERVER_MAC, MY_MAC, 16'd17, OP_STATUS+8'h80, {FIRMWARE_VERSION, BOARD_TYPE, PROCESS_TIME, TEMPERATURE, V_VCCINT, V_VCCAUX,BUTTONS}, 320'd0};
		tx_cnt <= 11'd63; // minimum frame length is 64 bytes, not including header
		tx_frm <= 1;
		end 
	else if (snd_wr_ack)
		begin
		tx_shift <= {SERVER_MAC, MY_MAC, 16'h0007, OP_WRITE+8'h80, ETH_WR_START_X,ETH_WR_LINE,ETH_WR_NUM_PIXELS, 400'd0};
		tx_cnt <= 11'd63; // minimum frame length is 64 bytes, not including header
		tx_frm <= 1;
		end 	
	else if (snd_rd_ack) 
		begin
		tx_shift <= {SERVER_MAC, MY_MAC, rd_pyld_len, OP_READ+8'h80, ETH_RD_START_X,ETH_RD_LINE,ETH_RD_NUM_PIXELS,ETH_RD_PIX,376'd0};
		tx_cnt <= (rd_tx_cnt<11'd63)? 11'd63 : rd_tx_cnt; // minimum frame length is 64 bytes, not including header
		tx_frm <= 1;
		end
	else if (snd_user_ctrl_ack) 
		begin
		tx_shift <= {SERVER_MAC, MY_MAC, 16'd20, OP_USER_CONTROL+8'h80, {USER_CTRL_EN,USER_CTRL,USER_STATUS}, 296'd0};
		tx_cnt <= 11'd63; // minimum frame length is 64 bytes, not including header
		tx_frm <= 1;
		end	
	else if (OUT_TICK)
		begin
		tx_cnt <= (tx_cnt == 0) ? tx_cnt : tx_cnt -1;
		tx_frm <= (tx_cnt != 0);
		if ((rx_opcode==OP_READ)&&e_rd_stb)
			tx_shift <= {tx_shift[559:392],ETH_RD_PIX,368'd0, 8'h00};  // inserts 3 bytes once every 3 cycles
		else
			tx_shift <= {tx_shift[559:0], 8'h00};
		end		
	end

// Add CRC to the stream
ETH_TX_CRC txcrc(.CLK(CLK), .IN_ETH_STREAM({OUT_TICK, tx_frm, tx_shift[567:560]}), .OUT_ETH_STREAM(OUT_ETH_STREAM)	);

endmodule

//-------------------------------------
// Simulation test bench
//-------------------------------------
// History of Changes:
//	1-08-2008 Initial creation
//-------------------------------------

module brd_packet_engine_tst;
// Clock generator
reg				CLK = 0;
initial #4 forever #4 CLK = !CLK;



// test pkt interface
wire	[1:0]	speed=2'd1; // 1=fast ethernet, 2=gigabit

// Input data stream fast ethernet
reg		[3:0]	out_cnt=4'd0; //generate tick for readout of fifo
wire			test_cke=1;
reg				out_cke=0;
always @(posedge CLK) 
	begin
	out_cnt <= (out_cnt==4'd9)? 0 : out_cnt+1;
	out_cke <= (speed==1)? (out_cnt==4'd0) : 1'b1;
	end
reg 			test_frm = 0;
reg 	[7:0]	test_dat;

reg 	[10:0]	test_pkt_size = 1054; // 14+12+1024+4
reg 	[10:0]	test_cnt = 11'd1536;
always @(posedge CLK) if (test_cke)
	begin
	test_cnt <= (test_cnt < test_pkt_size) ? test_cnt + 1 : test_cnt;
	test_frm <= (test_cnt < test_pkt_size);
	case (test_cnt)
/*	
	//Control Packet
	11'd00: test_dat <= 8'h00; // Dest MAC
	11'd01: test_dat <= 8'h18;
	11'd02: test_dat <= 8'hb7;
	11'd03: test_dat <= 8'h00;
	11'd04: test_dat <= 8'h20;
	11'd05: test_dat <= 8'hf3;
	11'd06: test_dat <= 8'h00; // Src MAC
	11'd07: test_dat <= 8'h18;
	11'd08: test_dat <= 8'hB7;
	11'd09: test_dat <= 8'hFF;
	11'd10: test_dat <= 8'hFF;
	11'd11: test_dat <= 8'hFF;
	11'd12: test_dat <= 8'h04; // Length
	11'd13: test_dat <= 8'h02;
	11'd14: test_dat <= 8'h01; // Opcode = control
	11'd15: test_dat <= 8'hFF; // CTRL
	11'd16: test_dat <= 8'hFF; 
	11'd17: test_dat <= 8'h00; // Width
	11'd18: test_dat <= 8'hFF; 
	11'd19: test_dat <= 8'h00; // Height
	11'd20: test_dat <= 8'hFF;
	default: test_dat <= test_cnt-20; 
*/
/*
	//Status Packet
	11'd00: test_dat <= 8'h00; // Dest MAC
	11'd01: test_dat <= 8'h18;
	11'd02: test_dat <= 8'hb7;
	11'd03: test_dat <= 8'h00;
	11'd04: test_dat <= 8'h20;
	11'd05: test_dat <= 8'hf3;
	11'd06: test_dat <= 8'h00; // Src MAC
	11'd07: test_dat <= 8'h18;
	11'd08: test_dat <= 8'hB7;
	11'd09: test_dat <= 8'hFF;
	11'd10: test_dat <= 8'hFF;
	11'd11: test_dat <= 8'hFF;
	11'd12: test_dat <= 8'h04; // Length
	11'd13: test_dat <= 8'h02;
	11'd14: test_dat <= 8'h02; // Opcode = status 
	default: test_dat <= test_cnt-14;
*/
/*
	//Write Packet
	11'd00: test_dat <= 8'h00; // Dest MAC
	11'd01: test_dat <= 8'h18;
	11'd02: test_dat <= 8'hb7;
	11'd03: test_dat <= 8'h00;
	11'd04: test_dat <= 8'h20;
	11'd05: test_dat <= 8'hf3;
	11'd06: test_dat <= 8'h00; // Src MAC
	11'd07: test_dat <= 8'h18;
	11'd08: test_dat <= 8'hB7;
	11'd09: test_dat <= 8'hFF;
	11'd10: test_dat <= 8'hFF;
	11'd11: test_dat <= 8'hFF;
	11'd12: test_dat <= 8'h04; // Length
	11'd13: test_dat <= 8'h02;
	11'd14: test_dat <= 8'h03; // Opcode = write
	11'd15: test_dat <= 8'h00; // StartX
	11'd16: test_dat <= 8'h01; 
	11'd17: test_dat <= 8'h00; // StartY
	11'd18: test_dat <= 8'h02; 
	11'd19: test_dat <= 8'h00; // NumBytes
	11'd20: test_dat <= 8'h15;
	default: test_dat <= test_cnt-21;
*/	

	//Read Packet
	11'd00: test_dat <= 8'h00; // Dest MAC
	11'd01: test_dat <= 8'h18;
	11'd02: test_dat <= 8'hb7;
	11'd03: test_dat <= 8'h00;
	11'd04: test_dat <= 8'h20;
	11'd05: test_dat <= 8'hf3;
	11'd06: test_dat <= 8'h00; // Src MAC
	11'd07: test_dat <= 8'h18;
	11'd08: test_dat <= 8'hB7;
	11'd09: test_dat <= 8'hFF;
	11'd10: test_dat <= 8'hFF;
	11'd11: test_dat <= 8'hFF;
	11'd12: test_dat <= 8'h04; // Length
	11'd13: test_dat <= 8'h02;
	11'd14: test_dat <= 8'h04; // Opcode = read
	11'd15: test_dat <= 8'h00; // StartX
	11'd16: test_dat <= 8'h01; 
	11'd17: test_dat <= 8'h00; // StartY
	11'd18: test_dat <= 8'h02; 
	11'd19: test_dat <= 8'h00; // NumPixels
	11'd20: test_dat <= 8'h04;
	default: test_dat <= test_cnt-20;

	
	endcase
	end

// Add CRC to the stream
wire	[9:0]	IN_TEST_STREAM;
ETH_TX_CRC ctrlcrc(.CLK(CLK), .IN_ETH_STREAM({test_cke, test_frm, test_dat[7:0]}), .OUT_ETH_STREAM(IN_TEST_STREAM)	);

wire	[9:0]	OUT_TEST_STREAM;
wire			status_cke = OUT_TEST_STREAM[9];
wire			stat_frm = OUT_TEST_STREAM[8];
wire	[7:0]	stat_dat = OUT_TEST_STREAM[7:0];


// Filter Control Interface
wire	[15:0]	CTRL_ENABLES;
wire	[15:0]	IMG_SIZE_X;
wire	[15:0]	IMG_SIZE_Y;
wire	[399:0]	CTRL_COEFF;

// Status
wire	[23:0]	PROCESS_TIME=24'h111111; 
wire	[15:0]	TEMPERATURE=16'h2222;
wire	[15:0]	V_VCCINT=16'h3333;
wire	[15:0]	V_VCCAUX=16'h4444;

// Frame Buffer Write Interface
wire			ETH_WR_CLK;        // expect to be CLK125, but whatever
wire	[15:0]	ETH_WR_START_X;    // start X
wire	[15:0]	ETH_WR_LINE;       // Line to write to
wire	[15:0]	ETH_WR_NUM_PIXELS; // number of pixels to write
wire			ETH_WR_GO;         // strobe before set of writes - latches parameters
wire			ETH_WR_DONE;       // Strobes when done
wire			ETH_WR_BUSY;       // Asserted from GO to DONE
wire	[23:0]	ETH_WR_PIX;
wire			ETH_WR_STB;        // Start strobing in pixels after GO
// Frame Buffer Read Interface
wire			ETH_RD_CLK;        // expect to be CLK125, but whatever
wire	[15:0]	ETH_RD_START_X;    // start X
wire	[15:0]	ETH_RD_LINE;       // Line to read from
wire	[15:0]	ETH_RD_NUM_PIXELS; // number of pixels to read
wire			ETH_RD_GO;         // strobe before set of reads - latches parameters, starts
wire		 	ETH_RD_DONE;       // Strobes when done
wire	 		ETH_RD_BUSY;       // Asserted from GO to DONE
wire	[23:0]	ETH_RD_PIX = 24'haa_bb_cc;
wire			ETH_RD_STB;        // Strobe to pull RD_PIX from FIFO

reg		[29:0]	rd_dly=0;
always @(posedge CLK)
	rd_dly <= (ETH_RD_GO)? 30'd1 : {rd_dly[28:0],1'b0};
wire			ETH_RD_READY=rd_dly[29];      // indicates that read FIFO has pixels ready for RD_STB


/*
SP601_FB_sim FBsim (
    .ETH_WR_CLK(ETH_WR_CLK), 
    .ETH_WR_START_X(ETH_WR_START_X), 
    .ETH_WR_LINE(ETH_WR_LINE), 
    .ETH_WR_NUM_PIXELS(ETH_WR_NUM_PIXELS), 
    .ETH_WR_GO(ETH_WR_GO), 
    .ETH_WR_DONE(ETH_WR_DONE), 
    .ETH_WR_BUSY(ETH_WR_BUSY), 
    .ETH_WR_PIX(ETH_WR_PIX), 
    .ETH_WR_STB(ETH_WR_STB), 
    .ETH_RD_CLK(ETH_RD_CLK), 
    .ETH_RD_START_X(ETH_RD_START_X), 
    .ETH_RD_LINE(ETH_RD_LINE), 
    .ETH_RD_NUM_PIXELS(ETH_RD_NUM_PIXELS), 
    .ETH_RD_GO(ETH_RD_GO), 
    .ETH_RD_DONE(ETH_RD_DONE), 
    .ETH_RD_BUSY(ETH_RD_BUSY), 
    .ETH_RD_READY(ETH_RD_READY), 
    .ETH_RD_PIX(ETH_RD_PIX), 
    .ETH_RD_STB(ETH_RD_STB)
    );
*/

wire		ETH_BUSY; // Set when a command has been issued, cleared when ack has been sent


BRD_PACKET_ENGINE #(	.SERVER_MAC(48'h00_18_B7_FF_FF_FF),
					.MY_MAC(48'h00_18_B7_FF_FF_FE),
					.FIRMWARE_VERSION(8'haa),
					.BOARD_TYPE(40'h33_30_16_10_11)	) 
	dut(
    .CLK(CLK), 
    .IN_ETH_STREAM(IN_TEST_STREAM),
    .ETH_BUSY(ETH_BUSY),
	.CTRL_ENABLES(CTRL_ENABLES), .IMG_SIZE_X(IMG_SIZE_X), .IMG_SIZE_Y(IMG_SIZE_Y), .CTRL_COEFF(CTRL_COEFF),
	.PROCESS_TIME(PROCESS_TIME), .TEMPERATURE(TEMPERATURE), .V_VCCINT(V_VCCINT), .V_VCCAUX(V_VCCAUX),
	.ETH_WR_CLK(ETH_WR_CLK),
	.ETH_WR_START_X(ETH_WR_START_X),.ETH_WR_LINE(ETH_WR_LINE),.ETH_WR_NUM_PIXELS(ETH_WR_NUM_PIXELS),
	.ETH_WR_GO(ETH_WR_GO),.ETH_WR_DONE(ETH_WR_DONE),.ETH_WR_BUSY(ETH_WR_BUSY),
	.ETH_WR_PIX(ETH_WR_PIX),.ETH_WR_STB(ETH_WR_STB),
	.ETH_RD_CLK(ETH_RD_CLK),
	.ETH_RD_START_X(ETH_RD_START_X),.ETH_RD_LINE(ETH_RD_LINE),.ETH_RD_NUM_PIXELS(ETH_RD_NUM_PIXELS),
	.ETH_RD_GO(ETH_RD_GO),.ETH_RD_DONE(ETH_RD_DONE),.ETH_RD_BUSY(ETH_RD_BUSY),
	.ETH_RD_READY(ETH_RD_READY),.ETH_RD_PIX(ETH_RD_PIX),.ETH_RD_STB(ETH_RD_STB),
    .OUT_ETH_STREAM(OUT_TEST_STREAM), .OUT_TICK(out_cke) );
    
wire	[9:0]	TX_ETH_STREAM;
ETH_TX_CRC txcrc(.CLK(CLK), .IN_ETH_STREAM(OUT_TEST_STREAM), .OUT_ETH_STREAM(TX_ETH_STREAM)	);    
    
wire			phy_cke = TX_ETH_STREAM[9];
wire			phy_frm = TX_ETH_STREAM[8];
wire	[7:0]	phy_dat = TX_ETH_STREAM[7:0];

initial 
	begin
	#43 test_cnt = 0;
	end

endmodule

