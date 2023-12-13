# Video games on FPGAs

This repository contains the final project for CSE 237C at UCSD.

The frame_gen, frame_processing and frame_buffer needs to be synthesized and exported as Vivado IP using Vitis. To simulate, an OpenCV installation needs to be present. Modify the paths as needed in the `script.tcl` files.

In Vivado, the `design_1_wrapper.xsa` is an export of hardware and bitstream, but not sure on how to import those back.

First try sourcing the `vivado/design_1.tcl` file. This will create the project but fail to create the block diagram, which is expected. Then, add the three exported IPs as custom user IP repositories in Vivado. Next, download and add the [Digilent Vivado Library](https://github.com/Digilent/vivado-library) as a custom repository too.

Now try sourcing the tcl file again and you should have the block diagram. Next, create a HDL wrapper for the block diagram and add the constraint file which specifies what external pins should match the pins on the Pynq board. We use it for buttons, LEDs and HDMI.

It should now allow you to generate a bitstream which you can write to the board.
