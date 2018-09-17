//-------------------------------------------------
// File: ece453_tb.v
// Purpose: ECE453 Test Bench
//-----------------------------------------------------------
`timescale 1ns / 100ps
module ece453_reg_file_tb ();

 reg 		      clk_50;
 reg 		      rst;

 reg [4:0] 	  slv_addr;
 reg 		      slv_read;
 reg 		      slv_write;
 wire [31:0] 	slv_read_data;
 reg  [31:0] 	slv_write_data;
 reg  [3:0]  	slv_byte_enable;

 wire 			  step;
 wire 			  reset_n;
 wire			    direction;
 wire			    fault_n;
 wire 			  en_n;
 wire			    sleep_n;
 wire [31:0] 	gpio_out;

 wire         ece453_irq;
 
 assign reset_n 	= gpio_out[0];
 assign direction 	= gpio_out[1];
 assign en_n 		= gpio_out[2];
 assign sleep_n 	= gpio_out[3];
 assign step 		= gpio_out[4];

 `include "ece453.vh"
	
 // instantiate the Device Under Test (DUT)
 // using named instantiation
ece453 u1(
	// signals to connect to an Avalon clock source interface
	.clk(clk_50),
	.reset(rst),
	
	// signals to connect to an Avalon-MM slave interface
	.slave_address(slv_addr),
	.slave_read(slv_read),
	.slave_write(slv_write),
	.slave_readdata(slv_read_data),
	.slave_writedata(slv_write_data),
	.slave_byteenable(slv_byte_enable),
	.gpio_inputs(32'h0),
	.gpio_outputs(gpio_out),
	.irq_out(ece453_irq)
);

 //----------------------------------------------------------
 // create a 50Mhz clock
 always
 #10 clk_50 = ~clk_50; // every ten nanoseconds invert


 initial
 begin
 $display($time, " << Starting the Simulation >>");
 clk_50 = 1'b0;
 rst = 1'b1;		// reset is active

 slv_addr = DEV_ID_ADDR;	
 slv_read = 1'b1;
 slv_write = 1'b0;
 slv_write_data = 32'h0;
 slv_byte_enable = 4'h0;



 #20 rst = 1'b0	;	// at time 20 release reset
 $display($time, " << Coming out of reset >>");



 @(negedge clk_50); // wait till the negedge of

  //===============================================
  // Read device ID
  //===============================================
   read_register(DEV_ID_ADDR,32'hECE45301, ALL_BITS);
  
  
  //===============================================
  // Write the number of CLK pulses before 
  // toggling step
  //===============================================
  
  write_register(PULSE_COUNT_ADDR, 50);
  #50
  
  write_register(CONTROL_ADDR,	CONTROL_SLEEP_N_MASK | CONTROL_DIR_MASK | CONTROL_RESET_N_MASK);
  wait(gpio_out == (CONTROL_SLEEP_N_MASK | CONTROL_DIR_MASK | CONTROL_RESET_N_MASK));

  $display($time, " ");
  $display($time, " << ---------------------------------------------- >>");
  $display($time, " << Waiting for step to be 0>>");
  $display($time, " << ---------------------------------------------- >>");
  wait ( step == 0);
  
  $display($time, " ");
  $display($time, " << ---------------------------------------------- >>");
  $display($time, " << Waiting for step to be 1>>");
  $display($time, " << ---------------------------------------------- >>");
  wait ( step == 1);
  
  $display($time, " ");
  $display($time, " << ---------------------------------------------- >>");
  $display($time, " << Waiting for step to be 0>>");
  $display($time, " << ---------------------------------------------- >>");
  wait ( step == 0);
  
  $display($time, " ");
  $display($time, " << ---------------------------------------------- >>");
  $display($time, " << Waiting for step to be 1>>");
  $display($time, " << ---------------------------------------------- >>");
  wait ( step == 1);

  $display($time, " << Simulation Complete >>");
  $stop;

 end

task write_register;
	input [3:0] reg_addr;
	input [31:0] data;
	begin
		@(negedge clk_50);
		$display($time, " << Writing register %h with value %h", reg_addr, data);
		slv_read=1'b0;
		slv_write=1'b1;
		slv_addr = reg_addr;
		slv_write_data = data;

		@(negedge clk_50); 
		slv_read=1'b0;
		slv_write=1'b0;
	end
endtask

task read_register;
	input [3:0] reg_addr;
	input [31:0] data;
	input [31:0] mask;
	begin
		@(negedge clk_50);
		
		slv_read=1'b1;
		slv_write=1'b0;
		slv_addr = reg_addr;
		slv_write_data = 0;

		@(negedge clk_50);
 		if( (data & mask)  != (slv_read_data & mask))
		begin
			$display($time, " << Reading Register %h :  ERROR !!!! Expected %h : Read %h", reg_addr, mask & data, mask & slv_read_data);
			$stop;
		end
		else
		begin
			$display($time, " << Reading Register %h:  Expected %h: Read %h", reg_addr, mask & data, mask & slv_read_data);
		end
		slv_read=1'b0;
		slv_write=1'b0;
	end
endtask

endmodule

