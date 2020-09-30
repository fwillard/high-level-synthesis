`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/29/2020 08:54:33 PM
// Design Name: 
// Module Name: circuit2
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module circuit2#(parameter DATAWIDTH = 32,
parameter WIDTH = 2) (a,b,c,z,x,dLTe,dEQe,g,h);
    
    
    input [DATAWIDTH-1:0] a, b, c;

    output [DATAWIDTH-1:0] z, x;
    
    wire  [DATAWIDTH-1:0] d, e, f;
    output reg [DATAWIDTH-1:0] g, h;
    output reg [WIDTH-1:0] dLTe, dEQe;
    wire [DATAWIDTH-1:0] zwire, xwire;

    assign d = a + b;
    assign e = a + c;
    assign f = a - b;
    
    always @(d, e) begin 
    
        if (d < e) begin
        
           dLTe <= 1;
           dEQe <= 0;
        end
        else begin
        
            dLTe <= 0;
            dEQe <= 1;
        end
    end
    
    //assign dEQe = d == e;
    //assign dLTe = d < e;
    //assign g = dLTe ? d : e;
    //assign h = dEQe ? g : f;
    always @(dLTe, d, e) begin
        if (dLTe == 1)
            g <= d;  
        else
            g <= e;
            end
    always @(dEQe, g, f) begin
        if (dEQe == 1)
            h <= g;  
        else
            h <= f;
            end  
                    
    assign xwire = g << dLTe;
    assign zwire = h >> dEQe;
    assign x = xwire;
    assign z = zwire;
    
    
endmodule
