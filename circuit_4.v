`timescale 1ns / 1ps

module circuit_4#(parameter DW = 64)(Clk, Rst, a, b, c, z, x);

    input Clk, Rst;
    input [DW-1:0] a, b, c;
    output [DW/2-1:0] z, x;
    
    wire [DW-1:0] d, e, f, g, h;
    wire [DW-1:0] dLTe, dEQe;  //64 bit due to the datawidth of the shift registers, this behaves like a single wire. 
    wire [DW-1:0] zrin, xrin;
    wire [DW-1:0] unuseful_GT;
    wire [DW-1:0] Qgreg, Qhreg;
    
    ADD #(.DATAWIDTH(DW)) add_1(a, b, d);
    ADD #(.DATAWIDTH(DW)) add_2(a, c, e);
    SUB #(.DATAWIDTH(DW)) sub_1(a, b, f);
    
    COMP #(.DATAWIDTH(DW)) comp_1(d, e, dEQe, dLTe, unuseful_GT);
    COMP #(.DATAWIDTH(DW)) comp_2(d, e, dEQe, dLTe, unuseful_GT);
    
    MUX2x1 #(.DATAWIDTH(DW)) mux_1(d, e, dLTe, g);
    MUX2x1 #(.DATAWIDTH(DW)) mux_2(g, f, dEQe, h);
    
    REG #(.DATAWIDTH(DW)) greg(g,Clk,Rst,Qgreg);
    REG #(.DATAWIDTH(DW)) hreg(h,Clk,Rst,Qhreg);
    
    SHL #(.DATAWIDTH(DW)) shl_1(Qhreg, dLTe, xrin);
    SHR #(.DATAWIDTH(DW)) shr_1(Qgreg, dQEe, zrin);
    
    REG #(.DATAWIDTH(DW/2)) outreg_1(xrin[DW/2-1:0], Clk, Rst, x);
    REG #(.DATAWIDTH(DW/2)) outreg_2(zrin[DW/2-1:0], Clk, Rst, z);

endmodule
