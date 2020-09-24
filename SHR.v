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

module SHR#(parameter DATAWIDTH = 2) (a, sh_amt, d);
input [DATAWIDTH-1:0] a;
input [DATAWIDTH-1:0] sh_amt;

output [DATAWIDTH-1:0] d;

assign d = a >> sh_amt;

endmodule 