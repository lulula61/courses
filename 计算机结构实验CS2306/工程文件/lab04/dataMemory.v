`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/26 21:06:10
// Design Name: 
// Module Name: dataMemory
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


module dataMemory(
    input clk,
    input [31:0] address,
    input [31:0] writeData,
    input memWrite,
    input memRead,
    output [31:0] readData
    );

    reg [31:0] memFile [0:63];
    reg [31:0] ReadData;
    
    always @(memRead or address) 
    begin
        if(memRead)
        begin
            if(address<63)
                ReadData = memFile[address];
            else
                ReadData = 0;
        end
    end

    always @(negedge clk)
    begin
        if(memWrite)
            if(address<63)
                memFile[address] = writeData;
        if(memRead)
            if(address<63)
                ReadData = writeData;
    end

    assign readData = ReadData;
endmodule