`timescale 1ns / 1ps

///////////////////////////////////////////////////////////////////////////////
// University of Arizona Fall 2020
// ECE 574A
// Assignment 1
//
//   Author              Date              Notes
// Sterling Davis       09/23/2020        Initial Release
//
//////////////////////////////////////////////////////////////////////////////

module DIV #(parameter DATAWIDTH = 64) (a,b,d);
    //Inputs
    input  [DATAWIDTH-1:0] a;
    input  [DATAWIDTH-1:0] b;
    
    //Outputs
    output [DATAWIDTH-1:0] d;
    
    assign d = a/b;
    
endmodule

