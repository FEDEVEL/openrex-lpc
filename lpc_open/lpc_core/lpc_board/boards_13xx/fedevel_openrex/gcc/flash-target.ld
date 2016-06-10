/**
 *  Copyright 2016 
 *  Marian Cingel - cingel.marian@gmail.com
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */

MEMORY 
{
    rom : ORIGIN = 0, LENGTH = 64K
    ram : ORIGIN = 0x10000000, LENGTH = 8K
}

SECTIONS {
    /* boot stack address */
    _vStackTop = 0x10002000;

    text ALIGN(4) : {
        *(.isr_vector)
        *(.after_vectors)
        *(.rodata)
        *(.rodata*)
        *(.text)
        *(.text*)

        /* table to init data and bss */
        . = ALIGN(4);
        __data_section_table = .;
        LONG(LOADADDR(data))
        LONG(ADDR(data))
        LONG(SIZEOF(data))
        __data_section_table_end = .;
        LONG(ADDR(bss))
        LONG(SIZEOF(bss))
        __bss_section_table_end = .;
        . = ALIGN(4);
    } >rom

    data : ALIGN(4) {
        *(.data)
        *(.data*)
        . = ALIGN(4);
    } >ram AT>rom

    bss : ALIGN(4) {
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
    } >ram AT>rom
}

