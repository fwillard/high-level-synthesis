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


module REG#(parameter DATAWIDTH = 2) (d,Clk,Rst,q);
    //Inputs
    input  [DATAWIDTH-1:0] d;
    input  Clk;
    input  Rst;
    
    //Outputs
    output reg [DATAWIDTH-1:0] q;
    
    always @(posedge Clk)
        begin
            if (Rst)
                q = 1'b1;
            else
                q = d;    
    
    
    end
endmodule
