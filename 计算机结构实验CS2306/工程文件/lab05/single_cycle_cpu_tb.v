`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/03/29 13:57:41
// Design Name: 
// Module Name: single_cycle_cpu_tb
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


module single_cycle_cpu_tb(

    );

    reg clk;
    reg reset;

    top processor(
        .clk(clk),
        .reset(reset)
    );

    initial begin
        $readmemb("C:/lab/lab05/mem_inst.dat",top.inst_mem.instFile);
        $readmemh("C:/lab/lab05/mem_data.dat",top.memory.memFile);
        reset = 1;
        clk = 0;
    end

    always #20 clk = ~clk;

    initial begin
        #30 reset = 0;
        #1500;
        $finish;
    end
endmodule