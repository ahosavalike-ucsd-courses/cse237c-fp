<project xmlns="com.autoesl.autopilot.project" name="frame_processing" top="frame_processing" projectType="C/C++">
    <includePaths/>
    <libraryPaths/>
    <Simulation argv="">
        <SimFlow name="csim" optimizeCompile="true" ldflags="-LC:/Users/adyan/Projects/CSE237C/game/opencv/mingw_build/install/x64/mingw/lib -llibopencv_core440 -llibopencv_imgproc440 -llibopencv_highgui440" mflags="" csimMode="0" lastCsimMode="0" setup="false" clean="false"/>
    </Simulation>
    <files xmlns="">
        <file name="tetris.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="conway.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="../../frame_processing_tb.cpp" sc="0" tb="1" cflags="-IC:/Users/adyan/Projects/CSE237C/game/opencv/mingw_build/install/include -Wno-unknown-pragmas" csimflags="" blackbox="false"/>
        <file name="frame_processing.cpp" sc="0" tb="false" cflags="-IC:/Users/adyan/.Xilinx/vitis_hls/2023.1/vitis_libraries/Vitis_Libraries/vision/L1/include -IC:/Users/adyan/Projects/CSE237C/game/opencv/mingw_build/install/include" csimflags="" blackbox="false"/>
        <file name="game.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="gfx_engine.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
    </files>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
        <solution name="solution1" status="active"/>
    </solutions>
</project>

