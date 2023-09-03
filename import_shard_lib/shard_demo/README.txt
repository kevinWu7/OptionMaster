一个最简单的c++项目，仅包含main.cpp文件以及CMakeLists.txt
libs存放需要引入的第三方动态库，此处以.dll为例
include存放第三方库提供的头文件

CMakeLists.txt描述了如何将动态库引入可执行文件中.
选择install选项，或者执行make install 会将可执行文件和*.dll一起输出到bin目录下。可直接运行exe