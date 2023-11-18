############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
## Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
############################################################
open_project frame_gen
set_top frame_gen
add_files frame_gen.cpp
add_files -tb frame_gen_tb.cpp -cflags "-IC:/Users/adyan/Projects/CSE237C/game/opencv/mingw_build/install/include -Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xc7z020-clg400-1}
create_clock -period 10 -name default
config_export -format ip_catalog -rtl verilog
source "./frame_gen/solution1/directives.tcl"
csim_design -ldflags {-LC:/Users/adyan/Projects/CSE237C/game/opencv/mingw_build/install/x64/mingw/lib -llibopencv_core440 -llibopencv_imgproc440 -llibopencv_highgui440}
csynth_design
cosim_design -ldflags {-LC:/Users/adyan/Projects/CSE237C/game/opencv/mingw_build/install/x64/mingw/lib -llibopencv_core440 -llibopencv_imgproc440 -llibopencv_highgui440}
export_design -rtl verilog -format ip_catalog
