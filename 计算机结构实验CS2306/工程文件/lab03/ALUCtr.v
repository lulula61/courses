`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/22 15:14:51
// Design Name: 
// Module Name: ALUCtr
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


module ALUCtr(
    input [1:0] aluOp,
    input [5:0] funct,
    output [3:0] aluCtrOut
    );
    reg [3:0] ALUCtrOut;
    
    always @ (aluOp or funct)
    begin
        casex({aluOp,funct})
        8'b00xxxxxx: ALUCtrOut = 4'b0010;   //add
        8'b01xxxxxx: ALUCtrOut = 4'b0110;   //sub
        8'b10xx0000: ALUCtrOut = 4'b0010;   //add
        8'b10xx0010: ALUCtrOut = 4'b0110;   //sub
        8'b10xx0100: ALUCtrOut = 4'b0000;   //and
        8'b10xx0101: ALUCtrOut = 4'b0001;   //or
        8'b10xx1010: ALUCtrOut = 4'b0111;   //slt
        endcase
    end
    assign aluCtrOut = ALUCtrOut;
    
endmodule
