`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/26 21:43:03
// Design Name: 
// Module Name: signext_tb
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


module signext_tb(

    );
    reg [15:0] inst;
    wire [31:0] data;
    signext u0(
        .inst(inst),
        .data(data));
    
    initial begin
        inst = 16'h0000;
        #200;
        inst = 16'h0001;
        #200
        inst = 16'hffff;
        #200
        inst = 16'h0002;
        #200;
        inst = 16'hfffe;
    end
endmodule
