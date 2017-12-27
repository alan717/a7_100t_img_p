module add(input_a,input_b,answer,carry);
    input input_a;
    input input_b;
    output answer,carry;
    assign answer=input_a^input_b;
    assign carry=input_a&input_b;
endmodule
module HELLO_TEST;
    reg iclk=0;
    reg a=0,b=0;
    wire c,d;
    always begin #1; iclk=!iclk; a=iclk; end
    always begin #3; b=!b; end
    add temp_add(.input_a(a),.input_b(b),.answer(c),.carry(d));
    initial
    begin
        $dumpfile("main.vcd");
        $dumpvars(0,temp_add);
        #100;
        $finish;
    end
    endmodule
