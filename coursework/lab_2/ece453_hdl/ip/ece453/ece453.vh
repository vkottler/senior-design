`ifndef _ece453_vh_
`define _ece453_vh_
  //*******************************************************************
  // Register Addresses
  //*******************************************************************
  localparam	DEV_ID_ADDR	      = 5'b0000;      // 0xFF200000
  localparam	GPIO_IN_ADDR	    = 5'b0001;      // 0xFF200004
  localparam	GPIO_OUT_ADDR	    = 5'b0010;      // 0xFF200008
  localparam  CONTROL_ADDR      = 5'b0011;      // 0xFF20000C
  localparam  STEP_COUNT_ADDR   = 5'b0100;      // 0xFF200010
  localparam  STEP_PERIOD_ADDR  = 5'b0101;      // 0xFF200014
  localparam	IM_ADDR	          = 5'b0110;      // 0xFF200018
  localparam	IRQ_ADDR	        = 5'b0111;      // 0xFF20001C

  //*******************************************************************
  // Register Bit Definitions
  //*******************************************************************
  localparam CONTROL_RESET_N_BIT_NUM	= 0;
	localparam CONTROL_DIR_BIT_NUM		  = 1;
	localparam CONTROL_EN_N_BIT_NUM		  = 2;
	localparam CONTROL_SLEEP_N_BIT_NUM	= 3;
	localparam CONTROL_MODE_BIT_NUM	    = 4;

  //*******************************************************************
  // Register Masks
  //*******************************************************************
  localparam CONTROL_RESET_N_MASK		= (32'h1 << CONTROL_RESET_N_BIT_NUM);
	localparam CONTROL_DIR_MASK		    = (32'h1 << CONTROL_DIR_BIT_NUM);
	localparam CONTROL_EN_N_MASK		  = (32'h1 << CONTROL_EN_N_BIT_NUM);
	localparam CONTROL_SLEEP_N_MASK		= (32'h1 << CONTROL_SLEEP_N_BIT_NUM);
	localparam CONTROL_MODE_MASK		  = (32'h1 << CONTROL_MODE_BIT_NUM);

	localparam	ALL_BITS      = 32'hFFFFFFFF;

`endif
