`timescale 1ns / 1ps
////////////////////////////////////////////////////////////////////////////////
// University of Arizona Fall 2020
// ECE 574A
// Assignment 1
//
//   Author              Date              Notes
// Michael Krotchie   09/23/2020        Initial Release
//
////////////////////////////////////////////////////////////////////////////////

module Circuit3(a, b, c, d, e, f, g, h, sa, avg, clk, rst);
    input clk, rst;
    input [7:0] sa;
    input [15:0] a, b, c, d, e, f, g, h;
    
    output [15:0] avg;
    
    wire [31:0] l00, l01, l02, l03, l10, l11, l2, l2div2, l2div4, l2div8;
    
    ADD #(.DATAWIDTH(32)) add_1(a, b, l00);
    ADD #(.DATAWIDTH(32)) add_2(c, d, l01);
    ADD #(.DATAWIDTH(32)) add_3(e, f, l02);
    ADD #(.DATAWIDTH(32)) add_4(g, h, l03);
    ADD #(.DATAWIDTH(32)) add_5(l00, l01, l10);
    ADD #(.DATAWIDTH(32)) add_6(l02, l03, l11);
    ADD #(.DATAWIDTH(32)) add_7(l10, l11, l2);
    
    SHR #(.DATAWIDTH(32)) shr_1(l2, sa, l2div2);
    SHR #(.DATAWIDTH(32)) shr_2(l2div2, sa, l2div4);
    SHR #(.DATAWIDTH(32)) shr_3(l2div4, sa, l2div8);

    REG #(.DATAWIDTH(16)) reg_1(l2div8, clk, rst, avg);
    
endmodule