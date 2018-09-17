
/*

  Author:  Joe Krachey
  Date:  07/12/2018

*/


module ece453(
  // signals to connect to an Avalon clock source interface
  clk,
  reset,
  // signals to connect to an Avalon-MM slave interface
  slave_address,
  slave_read,
  slave_write,
  slave_readdata,
  slave_writedata,
  slave_byteenable,
  // Non-Avalon Interface IO
  gpio_inputs,
  gpio_outputs,
  irq_out
);

  //*******************************************************************
  // Module Interface
  //*******************************************************************
  input clk;
  input reset;
  
  // slave interface
  input [4:0] slave_address;
  input slave_read;
  input slave_write;
  output wire [31:0] slave_readdata;
  input [31:0] slave_writedata;
  input [3:0] slave_byteenable;


  input [31:0] gpio_inputs;
  output [31:0] gpio_outputs;
  output wire irq_out;

 `include "ece453.vh"
  
  
  //*******************************************************************
  // Register Set
  //*******************************************************************
  reg  [31:0] dev_id_r;
  reg  [31:0] gpio_in_r;
  reg  [31:0] gpio_out_r;

   //*******************************************************************
  // Wires/Reg
  //*******************************************************************

  //*******************************************************************
  // Register Read Assignments
  //*******************************************************************

  //*******************************************************************
  // Output Assignments
  //*******************************************************************
   
  //*******************************************************************
  // Registers
  //*******************************************************************
  always @ (posedge clk or posedge reset)
  begin
  end

  //*******************************************************************
  // Instantiate instances of 7 Segment Decoder
  //*******************************************************************
  
endmodule

  
//*****************************************************************************
//  
//****************************************************************************
module hex_to_7_seg(
    input  [3:0]x,
    output reg [6:0]z
    );
endmodule
