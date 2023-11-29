<project xmlns="com.autoesl.autopilot.project" name="frame_gen" top="frame_gen" projectType="C/C++">
    <includePaths/>
    <libraryPaths/>
    <Simulation argv="">
        <SimFlow name="csim" ldflags="-LC:/Users/adyan/Projects/CSE237C/game/opencv/mingw_build/install/x64/mingw/lib -llibopencv_core440 -llibopencv_imgproc440 -llibopencv_highgui440" csimMode="0" lastCsimMode="0"/>
    </Simulation>
    <files xmlns="">
        <file name="frame_gen.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="../../frame_gen_tb.cpp" sc="0" tb="1" cflags="-IC:/Users/adyan/Projects/CSE237C/game/opencv/mingw_build/install/include -Wno-unknown-pragmas" csimflags="" blackbox="false"/>
    </files>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
    </solutions>
</project>

