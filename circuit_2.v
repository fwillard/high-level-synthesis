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
    wire [1:0] dLTe, dEQe;
    wire [DATAWIDTH-1:0] zwire, xwire;
    
    ADD #(.DATAWIDTH(8)) myAdd1(a, b, d);
    ADD #(.DATAWIDTH(8)) myAdd2(a, c, e);
    SUB #(.DATAWIDTH(8)) mySub(a, b, f);
    COMP #(.DATAWIDTH(8)) myComp1(d, e, dEQe);
    COMP #(.DATAWIDTH(8)) myComp2(d, e, dLTe);
    MUX2x1 #(.DATAWIDTH(8)) myMUX2x11(d, e, dLTe, g);
    MUX2x1 #(.DATAWIDTH(8)) myMUX2x12(g, f, dEQe, h);
    SHL #(.DATAWIDTH(8)) mySHL(g, dLTe, xwire);
    SHR #(.DATAWIDTH(8)) mySHR(h, dQEe, zwire);
    REG #(.DATAWIDTH(8)) myReg1(xwire, Clk, Rst, x);
    REG #(.DATAWIDTH(8)) myReg2(zwire, Clk, Rst, z);
    
    

    
endmodule
