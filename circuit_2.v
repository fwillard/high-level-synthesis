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


module circuit_2 (Clk, Rst, a, b, c, x, z);

    input Clk, Rst;
    input [31:0] a, b, c;
    output [31:0] x, z;
    wire [31:0] d, e, f, g, h;
    wire [0:0] dLTe, dEQe;
    wire [31:0] zwire, xwire;
    wire [31:0] unuseful_GT;
    
    ADD #(.DATAWIDTH(32)) myAdd1(a, b, d);
    ADD #(.DATAWIDTH(32)) myAdd2(a, c, e);
    SUB #(.DATAWIDTH(32)) mySub(a, b, f);
    COMP #(.DATAWIDTH(32)) myComp1(d, e, unuseful_GT, dLTe, dEQe);
    MUX2x1 #(.DATAWIDTH(32)) myMUX2x11(d, e, dLTe, g);
    MUX2x1 #(.DATAWIDTH(32)) myMUX2x12(g, f, dEQe, h);
    SHL #(.DATAWIDTH(32)) mySHL(g, dLTe, xwire);
    SHR #(.DATAWIDTH(32)) mySHR(h, dQEe, zwire);
    REG #(.DATAWIDTH(32)) myReg1(xwire, Clk, Rst, x);
    REG #(.DATAWIDTH(32)) myReg2(zwire, Clk, Rst, z);
    
    

    
endmodule
