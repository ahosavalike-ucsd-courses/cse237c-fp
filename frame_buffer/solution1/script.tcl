############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
## Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
############################################################
open_project frame_buffer
set_top frame_buffer
add_files frame_buffer/frame_buffer.cpp
add_files -tb frame_buffer/frame_buffer_tb.cpp -cflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xc7z020-clg400-1}
create_clock -period 10 -name default
config_export -format ip_catalog -rtl verilog
source "./frame_buffer/solution1/directives.tcl"
csim_design -O
csynth_design
cosim_design
export_design -rtl verilog -format ip_catalog
