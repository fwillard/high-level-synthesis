`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/30/2020 11:40:29 AM
// Design Name: 
// Module Name: circuit_2
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module circuit_2#(parameter DATAWIDTH = 32)(Clk, Rst, a, b, c, z, x);

    input Clk, Rst;
    input [DATAWIDTH-1:0] a, b, c;
    output [DATAWIDTH-1:0] z, x;
    wire [DATAWIDTH-1:0] d, e, f, g, h;
    wire [DATAWIDTH-1:0] dLTe, dEQe;
    wire [DATAWIDTH-1:0] zwire, xwire;
    wire [DATAWIDTH-1:0] unuseful_GT;
    
    ADD #(.DATAWIDTH(32)) myAdd1(a, b, d);
    ADD #(.DATAWIDTH(32)) myAdd2(a, c, e);
    SUB #(.DATAWIDTH(32)) mySub(a, b, f);
    COMP #(.DATAWIDTH(1)) myComp1(d, e, dEQe, dLTe, unuseful_GT);
    COMP #(.DATAWIDTH(1)) myComp2(d, e, dEQe, dLTe, unuseful_GT);
    MUX2x1 #(.DATAWIDTH(32)) myMUX2x11(d, e, dLTe, g);
    MUX2x1 #(.DATAWIDTH(32)) myMUX2x12(g, f, dEQe, h);
    SHL #(.DATAWIDTH(32)) mySHL(g, dLTe, xwire);
    SHR #(.DATAWIDTH(32)) mySHR(h, dQEe, zwire);
    REG #(.DATAWIDTH(32)) myReg1(xwire, Clk, Rst, x);
    REG #(.DATAWIDTH(32)) myReg2(zwire, Clk, Rst, z);
    
    

    
endmodule
