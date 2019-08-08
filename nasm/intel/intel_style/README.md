gcc 生成intel汇编
    gcc -S -masm=intel sample.c -o sample.s
gdb则是设置环境变量
    set disassembly-flavor intel

Nasm编译器是一款x86、x86_64的汇编器，使用Intel汇编格式。模块化设计、可移植。
