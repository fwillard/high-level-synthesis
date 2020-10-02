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


module Circuit6(a, b, c, zero, z, clk, rst);

input [63:0] a, b, c, zero;
input clk, rst;
output [63:0] z;

wire [63:0] e, f, g, zwire;
wire gEQz;

SUB #(.DATAWIDTH(64)) sub_1 (.a(a),.b(1),.diff(e));
ADD #(.DATAWIDTH(64)) add_1 (.a(c),.b(1),.sum(f));
MOD #(.DATAWIDTH(64)) mod_1 (.a(a),.b(c),.d(g));
COMP #(.DATAWIDTH(64)) comp_1 (.a(g),.b(zero),.eq(gEQz));
MUX2x1 #(.DATAWIDTH(64)) mux_1 (.a(e),.b(f),.sel(gEQz),.d(zwire));
REG #(.DATAWIDTH(64)) reg_1 (.d(zwire),.Clk(clk),.Rst(rst),.q(z));

endmodule
