`timescale 1ns / 1ps
////////////////////////////////////////////////////////////////////////////////
// University of Arizona Fall 2020
// ECE 574A
// Assignment 1
//
//   Author              Date              Notes
// Michael Krotchie      09/23/2020        Initial Release
//
////////////////////////////////////////////////////////////////////////////////

module MUX2x1#(parameter DATAWIDTH = 64)(a, b, sel, d);
    // inputs
    input [DATAWIDTH-1:0] a, b;
    input sel; 
    
    // outputs
    output reg [DATAWIDTH-1:0] d; 
    
    always @(a, b, sel) begin
        if (sel == 1)
            d <= a;  
        else
            d <= b;
            end
endmodule
