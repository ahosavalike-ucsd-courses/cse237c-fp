
################################################################
# This is a generated script based on design: design_1
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2023.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_gid_msg -ssname BD::TCL -id 2041 -severity "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source design_1_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xc7z020clg400-1
   set_property BOARD_PART tul.com.tw:pynq-z2:part0:1.0 [current_project]
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name design_1

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_gid_msg -ssname BD::TCL -id 2001 -severity "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_gid_msg -ssname BD::TCL -id 2002 -severity "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_gid_msg -ssname BD::TCL -id 2003 -severity "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_gid_msg -ssname BD::TCL -id 2004 -severity "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_gid_msg -ssname BD::TCL -id 2005 -severity "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_gid_msg -ssname BD::TCL -id 2006 -severity "ERROR" $errMsg}
   return $nRet
}

set bCheckIPsPassed 1
##################################################################
# CHECK IPs
##################################################################
set bCheckIPs 1
if { $bCheckIPs == 1 } {
   set list_check_ips "\ 
xilinx.com:ip:processing_system7:5.5\
xilinx.com:ip:v_tc:6.2\
digilentinc.com:ip:rgb2dvi:1.4\
xilinx.com:ip:v_axi4s_vid_out:4.0\
xilinx.com:ip:xlconstant:1.1\
xilinx.com:ip:ila:6.2\
xilinx.com:ip:proc_sys_reset:5.0\
xilinx.com:hls:frame_gen:1.0\
xilinx.com:hls:frame_buffer:1.0\
xilinx.com:hls:frame_processing:1.0\
"

   set list_ips_missing ""
   common::send_gid_msg -ssname BD::TCL -id 2011 -severity "INFO" "Checking if the following IPs exist in the project's IP catalog: $list_check_ips ."

   foreach ip_vlnv $list_check_ips {
      set ip_obj [get_ipdefs -all $ip_vlnv]
      if { $ip_obj eq "" } {
         lappend list_ips_missing $ip_vlnv
      }
   }

   if { $list_ips_missing ne "" } {
      catch {common::send_gid_msg -ssname BD::TCL -id 2012 -severity "ERROR" "The following IPs are not found in the IP Catalog:\n  $list_ips_missing\n\nResolution: Please add the repository containing the IP(s) to the project." }
      set bCheckIPsPassed 0
   }

}

if { $bCheckIPsPassed != 1 } {
  common::send_gid_msg -ssname BD::TCL -id 2023 -severity "WARNING" "Will not continue with creation of design due to the error(s) above."
  return 3
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set DDR_0 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 DDR_0 ]

  set FIXED_IO_0 [ create_bd_intf_port -mode Master -vlnv xilinx.com:display_processing_system7:fixedio_rtl:1.0 FIXED_IO_0 ]


  # Create ports
  set TMDS_Clk_p_0 [ create_bd_port -dir O -type clk TMDS_Clk_p_0 ]
  set TMDS_Clk_n_0 [ create_bd_port -dir O -type clk TMDS_Clk_n_0 ]
  set TMDS_Data_p_0 [ create_bd_port -dir O -from 2 -to 0 TMDS_Data_p_0 ]
  set TMDS_Data_n_0 [ create_bd_port -dir O -from 2 -to 0 TMDS_Data_n_0 ]
  set btn_0 [ create_bd_port -dir I btn_0 ]
  set btn_1 [ create_bd_port -dir I btn_1 ]
  set led_0 [ create_bd_port -dir O led_0 ]
  set led_1 [ create_bd_port -dir O led_1 ]
  set btn_3 [ create_bd_port -dir I -type data btn_3 ]
  set btn_2 [ create_bd_port -dir I -type data btn_2 ]

  # Create instance: processing_system7_0, and set properties
  set processing_system7_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0 ]
  set_property -dict [list \
    CONFIG.PCW_ACT_APU_PERIPHERAL_FREQMHZ {666.666687} \
    CONFIG.PCW_ACT_CAN_PERIPHERAL_FREQMHZ {10.000000} \
    CONFIG.PCW_ACT_DCI_PERIPHERAL_FREQMHZ {10.158730} \
    CONFIG.PCW_ACT_ENET0_PERIPHERAL_FREQMHZ {10.000000} \
    CONFIG.PCW_ACT_ENET1_PERIPHERAL_FREQMHZ {10.000000} \
    CONFIG.PCW_ACT_FPGA0_PERIPHERAL_FREQMHZ {100.000000} \
    CONFIG.PCW_ACT_FPGA1_PERIPHERAL_FREQMHZ {40.000000} \
    CONFIG.PCW_ACT_FPGA2_PERIPHERAL_FREQMHZ {200.000000} \
    CONFIG.PCW_ACT_FPGA3_PERIPHERAL_FREQMHZ {10.000000} \
    CONFIG.PCW_ACT_PCAP_PERIPHERAL_FREQMHZ {200.000000} \
    CONFIG.PCW_ACT_QSPI_PERIPHERAL_FREQMHZ {10.000000} \
    CONFIG.PCW_ACT_SDIO_PERIPHERAL_FREQMHZ {10.000000} \
    CONFIG.PCW_ACT_SMC_PERIPHERAL_FREQMHZ {10.000000} \
    CONFIG.PCW_ACT_SPI_PERIPHERAL_FREQMHZ {10.000000} \
    CONFIG.PCW_ACT_TPIU_PERIPHERAL_FREQMHZ {200.000000} \
    CONFIG.PCW_ACT_TTC0_CLK0_PERIPHERAL_FREQMHZ {111.111115} \
    CONFIG.PCW_ACT_TTC0_CLK1_PERIPHERAL_FREQMHZ {111.111115} \
    CONFIG.PCW_ACT_TTC0_CLK2_PERIPHERAL_FREQMHZ {111.111115} \
    CONFIG.PCW_ACT_TTC1_CLK0_PERIPHERAL_FREQMHZ {111.111115} \
    CONFIG.PCW_ACT_TTC1_CLK1_PERIPHERAL_FREQMHZ {111.111115} \
    CONFIG.PCW_ACT_TTC1_CLK2_PERIPHERAL_FREQMHZ {111.111115} \
    CONFIG.PCW_ACT_UART_PERIPHERAL_FREQMHZ {10.000000} \
    CONFIG.PCW_ACT_WDT_PERIPHERAL_FREQMHZ {111.111115} \
    CONFIG.PCW_CLK0_FREQ {100000000} \
    CONFIG.PCW_CLK1_FREQ {40000000} \
    CONFIG.PCW_CLK2_FREQ {200000000} \
    CONFIG.PCW_CLK3_FREQ {10000000} \
    CONFIG.PCW_DDR_RAM_HIGHADDR {0x1FFFFFFF} \
    CONFIG.PCW_EN_CLK1_PORT {1} \
    CONFIG.PCW_EN_CLK2_PORT {1} \
    CONFIG.PCW_FCLK_CLK1_BUF {TRUE} \
    CONFIG.PCW_FCLK_CLK2_BUF {TRUE} \
    CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {100} \
    CONFIG.PCW_FPGA1_PERIPHERAL_FREQMHZ {40} \
    CONFIG.PCW_FPGA2_PERIPHERAL_FREQMHZ {200} \
    CONFIG.PCW_FPGA_FCLK0_ENABLE {1} \
    CONFIG.PCW_FPGA_FCLK1_ENABLE {1} \
    CONFIG.PCW_FPGA_FCLK2_ENABLE {1} \
    CONFIG.PCW_UIPARAM_ACT_DDR_FREQ_MHZ {533.333374} \
  ] $processing_system7_0


  # Create instance: v_tc_0, and set properties
  set v_tc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_tc:6.2 v_tc_0 ]
  set_property -dict [list \
    CONFIG.HAS_AXI4_LITE {false} \
    CONFIG.SYNC_EN {false} \
    CONFIG.enable_detection {false} \
  ] $v_tc_0


  # Create instance: rgb2dvi_0, and set properties
  set rgb2dvi_0 [ create_bd_cell -type ip -vlnv digilentinc.com:ip:rgb2dvi:1.4 rgb2dvi_0 ]
  set_property -dict [list \
    CONFIG.kClkRange {3} \
    CONFIG.kGenerateSerialClk {true} \
  ] $rgb2dvi_0


  # Create instance: v_axi4s_vid_out_0, and set properties
  set v_axi4s_vid_out_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_axi4s_vid_out:4.0 v_axi4s_vid_out_0 ]
  set_property CONFIG.C_HAS_ASYNC_CLK {1} $v_axi4s_vid_out_0


  # Create instance: enable_1, and set properties
  set enable_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 enable_1 ]

  # Create instance: ila_frame_gen, and set properties
  set ila_frame_gen [ create_bd_cell -type ip -vlnv xilinx.com:ip:ila:6.2 ila_frame_gen ]
  set_property CONFIG.C_SLOT_0_AXI_PROTOCOL {AXI4S} $ila_frame_gen


  # Create instance: ila_frame_process, and set properties
  set ila_frame_process [ create_bd_cell -type ip -vlnv xilinx.com:ip:ila:6.2 ila_frame_process ]
  set_property CONFIG.C_SLOT_0_AXI_PROTOCOL {AXI4S} $ila_frame_process


  # Create instance: ila_video_out, and set properties
  set ila_video_out [ create_bd_cell -type ip -vlnv xilinx.com:ip:ila:6.2 ila_video_out ]
  set_property -dict [list \
    CONFIG.C_MONITOR_TYPE {Native} \
    CONFIG.C_NUM_OF_PROBES {10} \
    CONFIG.C_PROBE0_WIDTH {24} \
    CONFIG.C_PROBE5_WIDTH {11} \
    CONFIG.C_PROBE6_WIDTH {32} \
    CONFIG.C_SLOT_0_AXI_PROTOCOL {AXI4S} \
  ] $ila_video_out


  # Create instance: proc_sys_reset_0, and set properties
  set proc_sys_reset_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 proc_sys_reset_0 ]

  # Create instance: ila_frame_buffer, and set properties
  set ila_frame_buffer [ create_bd_cell -type ip -vlnv xilinx.com:ip:ila:6.2 ila_frame_buffer ]
  set_property CONFIG.C_SLOT_0_AXI_PROTOCOL {AXI4S} $ila_frame_buffer


  # Create instance: frame_gen_0, and set properties
  set frame_gen_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:frame_gen:1.0 frame_gen_0 ]

  # Create instance: frame_buffer_0, and set properties
  set frame_buffer_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:frame_buffer:1.0 frame_buffer_0 ]

  # Create instance: frame_processing_0, and set properties
  set frame_processing_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:frame_processing:1.0 frame_processing_0 ]

  # Create interface connections
  connect_bd_intf_net -intf_net frame_buffer_0_out_r [get_bd_intf_pins frame_buffer_0/out_r] [get_bd_intf_pins v_axi4s_vid_out_0/video_in]
connect_bd_intf_net -intf_net [get_bd_intf_nets frame_buffer_0_out_r] [get_bd_intf_pins frame_buffer_0/out_r] [get_bd_intf_pins ila_frame_buffer/SLOT_0_AXIS]
  connect_bd_intf_net -intf_net frame_gen_0_output_r1 [get_bd_intf_pins frame_gen_0/output_r] [get_bd_intf_pins frame_processing_0/input_r]
connect_bd_intf_net -intf_net [get_bd_intf_nets frame_gen_0_output_r1] [get_bd_intf_pins frame_gen_0/output_r] [get_bd_intf_pins ila_frame_gen/SLOT_0_AXIS]
  connect_bd_intf_net -intf_net frame_processing_0_output_r [get_bd_intf_pins frame_processing_0/output_r] [get_bd_intf_pins frame_buffer_0/in_r]
connect_bd_intf_net -intf_net [get_bd_intf_nets frame_processing_0_output_r] [get_bd_intf_pins frame_processing_0/output_r] [get_bd_intf_pins ila_frame_process/SLOT_0_AXIS]
  connect_bd_intf_net -intf_net processing_system7_0_DDR [get_bd_intf_ports DDR_0] [get_bd_intf_pins processing_system7_0/DDR]
  connect_bd_intf_net -intf_net processing_system7_0_FIXED_IO [get_bd_intf_ports FIXED_IO_0] [get_bd_intf_pins processing_system7_0/FIXED_IO]
  connect_bd_intf_net -intf_net v_tc_0_vtiming_out [get_bd_intf_pins v_tc_0/vtiming_out] [get_bd_intf_pins v_axi4s_vid_out_0/vtiming_in]

  # Create port connections
  connect_bd_net -net Net [get_bd_pins processing_system7_0/FCLK_CLK1] [get_bd_pins v_axi4s_vid_out_0/vid_io_out_clk] [get_bd_pins rgb2dvi_0/PixelClk] [get_bd_pins v_tc_0/clk]
  connect_bd_net -net btn_0_1 [get_bd_ports btn_0] [get_bd_ports led_0] [get_bd_pins frame_processing_0/move_cw]
  connect_bd_net -net btn_1_1 [get_bd_ports btn_1] [get_bd_ports led_1] [get_bd_pins frame_processing_0/move_ccw]
  connect_bd_net -net game_select_0_1 [get_bd_ports btn_3] [get_bd_pins frame_processing_0/game_select]
  connect_bd_net -net proc_sys_reset_0_peripheral_aresetn [get_bd_pins proc_sys_reset_0/peripheral_aresetn] [get_bd_pins v_axi4s_vid_out_0/aresetn] [get_bd_pins frame_gen_0/ap_rst_n] [get_bd_pins frame_buffer_0/ap_rst_n] [get_bd_pins frame_processing_0/ap_rst_n]
  connect_bd_net -net processing_system7_0_FCLK_CLK0 [get_bd_pins processing_system7_0/FCLK_CLK0] [get_bd_pins ila_frame_gen/clk] [get_bd_pins ila_frame_process/clk] [get_bd_pins ila_video_out/clk] [get_bd_pins processing_system7_0/M_AXI_GP0_ACLK] [get_bd_pins v_axi4s_vid_out_0/aclk] [get_bd_pins proc_sys_reset_0/slowest_sync_clk] [get_bd_pins ila_frame_buffer/clk] [get_bd_pins frame_gen_0/ap_clk] [get_bd_pins frame_buffer_0/ap_clk] [get_bd_pins frame_processing_0/ap_clk]
  connect_bd_net -net processing_system7_0_FCLK_RESET0_N [get_bd_pins processing_system7_0/FCLK_RESET0_N] [get_bd_pins proc_sys_reset_0/ext_reset_in]
  connect_bd_net -net rgb2dvi_0_TMDS_Clk_n [get_bd_pins rgb2dvi_0/TMDS_Clk_n] [get_bd_ports TMDS_Clk_n_0]
  connect_bd_net -net rgb2dvi_0_TMDS_Clk_p [get_bd_pins rgb2dvi_0/TMDS_Clk_p] [get_bd_ports TMDS_Clk_p_0]
  connect_bd_net -net rgb2dvi_0_TMDS_Data_n [get_bd_pins rgb2dvi_0/TMDS_Data_n] [get_bd_ports TMDS_Data_n_0]
  connect_bd_net -net rgb2dvi_0_TMDS_Data_p [get_bd_pins rgb2dvi_0/TMDS_Data_p] [get_bd_ports TMDS_Data_p_0]
  connect_bd_net -net rotate_x_0_1 [get_bd_ports btn_2] [get_bd_pins frame_processing_0/rotate_x]
  connect_bd_net -net v_axi4s_vid_out_0_fifo_read_level [get_bd_pins v_axi4s_vid_out_0/fifo_read_level] [get_bd_pins ila_video_out/probe7]
  connect_bd_net -net v_axi4s_vid_out_0_locked [get_bd_pins v_axi4s_vid_out_0/locked] [get_bd_pins ila_video_out/probe4]
  connect_bd_net -net v_axi4s_vid_out_0_overflow [get_bd_pins v_axi4s_vid_out_0/overflow] [get_bd_pins ila_video_out/probe5]
  connect_bd_net -net v_axi4s_vid_out_0_sof_state_out [get_bd_pins v_axi4s_vid_out_0/sof_state_out] [get_bd_pins ila_video_out/probe9]
  connect_bd_net -net v_axi4s_vid_out_0_status [get_bd_pins v_axi4s_vid_out_0/status] [get_bd_pins ila_video_out/probe8]
  connect_bd_net -net v_axi4s_vid_out_0_underflow [get_bd_pins v_axi4s_vid_out_0/underflow] [get_bd_pins ila_video_out/probe6]
  connect_bd_net -net v_axi4s_vid_out_0_vid_active_video [get_bd_pins v_axi4s_vid_out_0/vid_active_video] [get_bd_pins rgb2dvi_0/vid_pVDE] [get_bd_pins ila_video_out/probe3]
  connect_bd_net -net v_axi4s_vid_out_0_vid_data [get_bd_pins v_axi4s_vid_out_0/vid_data] [get_bd_pins rgb2dvi_0/vid_pData] [get_bd_pins ila_video_out/probe0]
  connect_bd_net -net v_axi4s_vid_out_0_vid_hsync [get_bd_pins v_axi4s_vid_out_0/vid_hsync] [get_bd_pins rgb2dvi_0/vid_pHSync] [get_bd_pins ila_video_out/probe1]
  connect_bd_net -net v_axi4s_vid_out_0_vid_vblank [get_bd_pins v_axi4s_vid_out_0/vid_vblank] [get_bd_pins rgb2dvi_0/vid_pVSync] [get_bd_pins ila_video_out/probe2]
  connect_bd_net -net v_axi4s_vid_out_0_vtg_ce [get_bd_pins v_axi4s_vid_out_0/vtg_ce] [get_bd_pins v_tc_0/gen_clken]
  connect_bd_net -net xlconstant_0_dout [get_bd_pins enable_1/dout] [get_bd_pins v_axi4s_vid_out_0/vid_io_out_ce] [get_bd_pins v_tc_0/clken] [get_bd_pins v_axi4s_vid_out_0/aclken] [get_bd_pins v_tc_0/resetn] [get_bd_pins frame_gen_0/ap_start] [get_bd_pins frame_buffer_0/ap_start] [get_bd_pins frame_processing_0/ap_start]

  # Create address segments


  # Restore current instance
  current_bd_instance $oldCurInst

  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


