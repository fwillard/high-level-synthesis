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

module COMP#(parameter DATAWIDTH = 2) (a, b, gt, lt, eq);
input [DATAWIDTH-1:0] a;
input [DATAWIDTH-1:0] b;

output reg gt, lt, eq;

always @(a, b) begin 
    if (a > b) begin
        gt <= 1;
        lt <= 0;
        eq <= 0;
    end
    else if (a < b) begin
        gt <= 0;
        lt <= 1;
        eq <= 0;
    end
    else begin
        gt <= 0;
        lt <= 0;
        eq <= 1;
    end
end

endmodule 