/* Overlay 129 */
OUTPUT_ARCH(arm)
MEMORY {

        rom     : ORIGIN = 0x023D8000 + 0x600, LENGTH = 0x8000-0x600
        ewram   : ORIGIN = 0x023D8000 + 0x600, LENGTH = 0x8000-0x600
}

SECTIONS {
        .text : {

                FILL (0xABCD)

                __text_start = . ;
                *(.cheatcfg)  /* pinned first: gCheatConfig stays at 0x023D8600 across all builds */
                *(.init)
                *(.text)
                *(.ctors)
                *(.dtors)
                *(.rodata)
                *(.fini)
                *(COMMON)
                __text_end  = . ;

                __bss_start__ = . ;
                *(.bss)
                __bss_end__ = . ;
        _end = __bss_end__ ;
        __end__ = __bss_end__ ;
        } >rom = 0xff
}