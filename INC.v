`timescale 1ns / 1ps
////////////////////////////////////////////////////////////////////////////////
// University of Arizona Fall 2020
// ECE 574A
// Assignment 1
//
//   Author              Date              Notes
// Michael Krotchie      09/23/2020        Initial Release
//
//////////////////////////////////////////////////////////////////////////////

module INC#(parameter DATAWIDTH = 64)(a, d);
    // inputs
    input [DATAWIDTH-1:0] a;
    
    // outputs
    output reg [DATAWIDTH-1:0] d;
    
    always @ (a) begin
        d <= a + 1;
    end
    
endmodule
