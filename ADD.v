`timescale 1ns / 1ps
////////////////////////////////////////////////////////////////////////////////
// University of Arizona Fall 2020
// ECE 574A
// Assignment 1
//
//   Author              Date              Notes
// Paul Niman      09/23/2020        Initial Release
//
////////////////////////////////////////////////////////////////////////////////



module ADD #(parameter DATAWIDTH = 2) (a,b,sum);
    //Inputs
    input  [DATAWIDTH-1:0] a;
    input  [DATAWIDTH-1:0] b;
    
    //Outputs
    output [DATAWIDTH-1:0] sum;
    
    
    assign sum = a+b;
    
endmodule