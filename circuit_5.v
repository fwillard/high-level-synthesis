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


module circuit_5(Clk, Rst, a, b, c, d, zero, z);

    input Clk, Rst;
    input [63:0] a, b, c, d,zero;
    output [63:0] z;
    wire [63:0] e, f, g,zwire;
    wire [0:0] gEQz;
    wire [63:0] unuseful_GT, unuseful_LT;
    
    DIV #(.DATAWIDTH(64)) myDiv1(a, b, e);
    DIV #(.DATAWIDTH(64)) myDiv2(c, d, f);
    MOD #(.DATAWIDTH(64)) myMod(a, b, g);
    COMP #(.DATAWIDTH(64)) myComp(g, zero, unuseful_GT, unuseful_LT, gEQz);
    MUX2x1 #(.DATAWIDTH(64)) myMUX2x1(e, f, gEQz, zwire);
    REG #(.DATAWIDTH(64)) myReg(zwire, Clk, Rst, z);
    
    
    

    
endmodule
