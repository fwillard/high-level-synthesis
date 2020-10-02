`timescale 1ns / 1ps
////////////////////////////////////////////////////////////////////////////////
// University of Arizona Fall 2020
// ECE 574A
// Assignment 1
//
//   Author              Date              Notes
// Finn Willard     09/23/2020        Initial Release
//
////////////////////////////////////////////////////////////////////////////////


module Circuit1(a, b, c, z, x, clk, rst);
    input clk, rst;
    input [7:0] a, b, c;
    output [7:0] z;
    output [15:0] x;
    
    wire [7:0] d, e;
    wire [15:0] f, g;
    wire [15:0] xwire;
    
    ADD #(.DATAWIDTH(8)) add_1(.a(a),.b(b),.sum(d));
    ADD #(.DATAWIDTH(8)) add_2(.a(a),.b(c),.sum(e));
    COMP #(.DATAWIDTH(8)) comp_1(.a(d),.b(e),.gt(g[0]));
    MUX2x1 #(.DATAWIDTH(8)) mux_1(.a(d),.b(e),.sel(g[0]),.d(z));
    MUL #(.DATAWIDTH(8)) mul_1(.a(a),.b(c),.d(f));
    SUB #(.DATAWIDTH(16)) sub_1(.a(f),.b(d),.diff(xwire));
    REG #(.DATAWIDTH(16)) reg_1(.d(xwire),.Clk(clk),.Rst(rst),.q(x));
    
    
endmodule
