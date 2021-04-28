


    .global _start
    .type _start,%function

_start:
    str lr,[sp,#-8]!
    bl main

