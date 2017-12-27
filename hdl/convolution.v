module convolution
(   clk,
    reset,
    code_in,
    code_out,
    valid_in,
    valid_out,
    //next_state
);
input clk;
input reset;
input code_in;
input valid_in;
output[2:0] code_out;
output valid_out;
//output [2:0] next_state;
reg      valid_out=0;
reg[2:0] code_out=0;
reg[2:0] next_state=0;

always@(posedge clk)
begin
    if(reset)
    begin
        code_out<=0;
        next_state<=0;
        valid_out<=0;
    end
    else
    if(valid_in)
    begin
         next_state[0]<=next_state[1];
         next_state[1]<=next_state[2];
         next_state[2]<=code_in;
         code_out[2]<=next_state[2];
         code_out[1]<=next_state[2]+next_state[1];
         code_out[0]<=next_state[2]+next_state[0];
         valid_out<=1;
    end
    else
    begin
        if(valid_out)
        begin
          valid_out<=0;
        end
    end
end
endmodule


module conv_test;

reg clk;
reg irst;
reg en;
wire [2:0] out;
wire en_o;
reg in,valid;


convolution conv_test(
    .clk(clk),
    .reset(irst),
    .code_in(in),
    .code_out(out),
    .valid_in(en),
    .valid_out(en_o)
);

initial 
begin
    clk=0;
    irst=1;
    in=0;
    en=0;
    
end

always@(negedge en_o)
begin




end

always #100 clk=~clk;



