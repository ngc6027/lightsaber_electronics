/* ************************************************************************** */
/*
  @Company
    Shippensburg University of Pennsylvania

  @File Name
    pmodsf.h

  @Summary
    This file defines functions for interacting with the PMODSF peripheral from
        Digilent.
 */
/* ************************************************************************** */

#ifndef _PMODSF_H    /* Guard against multiple inclusion */
#define _PMODSF_H

/*
 *  The memory of the ST Microelectronics M25P16 (the chip on the PMOD SF)
 *  is orgainized as 32 sectors containing 256 pages, with each page being
 *  256 bytes wide (p. 6 of the M25P16 datasheet). The power-up sequence should
 *  take no longer than 10 ms. The sector ranges are as follows:
 * 
 *     Sector       |    Starting Address   |   Ending Address
 * -----------------|-----------------------|---------------------
 *       00         |       0x000000        |    0x00FFFF                      
 *       01         |       0x010000        |    0x01FFFF                  
 *       02         |       0x020000        |    0x02FFFF                  
 *       03         |       0x030000        |    0x03FFFF                  
 *       04         |       0x040000        |    0x04FFFF                  
 *       05         |       0x050000        |    0x05FFFF                  
 *       06         |       0x060000        |    0x06FFFF                    
 *       07         |       0x070000        |    0x07FFFF                  
 *       08         |       0x080000        |    0x08FFFF                  
 *       09         |       0x090000        |    0x09FFFF                  
 *       10         |       0x0A0000        |    0x0AFFFF                  
 *       11         |       0x0B0000        |    0x0BFFFF                  
 *       12         |       0x0C0000        |    0x0CFFFF                  
 *       13         |       0x0D0000        |    0x0DFFFF                  
 *       14         |       0x0E0000        |    0x0EFFFF                      
 *       15         |       0x0F0000        |    0x0FFFFF                    
 *       16         |       0x100000        |    0x10FFFF                     
 *       17         |       0x110000        |    0x11FFFF                   
 *       18         |       0x120000        |    0x12FFFF                  
 *       19         |       0x130000        |    0x13FFFF                  
 *       20         |       0x140000        |    0x14FFFF                   
 *       21         |       0x150000        |    0x15FFFF                  
 *       22         |       0x160000        |    0x16FFFF                  
 *       23         |       0x170000        |    0x17FFFF                  
 *       24         |       0x180000        |    0x18FFFF                   
 *       25         |       0x190000        |    0x19FFFF                  
 *       26         |       0x1A0000        |    0x1AFFFF                  
 *       27         |       0x1B0000        |    0x1BFFFF                  
 *       28         |       0x1C0000        |    0x1CFFFF                  
 *       29         |       0x1D0000        |    0x1DFFFF                  
 *       30         |       0x1E0000        |    0x1EFFFF                  
 *       31         |       0x1F0000        |    0x1FFFFF                  
 */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */

// Sector Addresses
#define SECTOR00    0x000000
#define SECTOR01    0x010000
#define SECTOR02    0x020000
#define SECTOR03    0x030000
#define SECTOR04    0x040000
#define SECTOR05    0x050000
#define SECTOR06    0x060000
#define SECTOR07    0x070000
#define SECTOR08    0x080000
#define SECTOR09    0x090000
#define SECTOR10    0x0A0000
#define SECTOR11    0x0B0000
#define SECTOR12    0x0C0000
#define SECTOR13    0x0D0000
#define SECTOR14    0x0E0000
#define SECTOR15    0x0F0000
#define SECTOR16    0x100000
#define SECTOR17    0x110000
#define SECTOR18    0x120000
#define SECTOR19    0x130000
#define SECTOR20    0x140000
#define SECTOR21    0x150000
#define SECTOR22    0x160000
#define SECTOR23    0x170000
#define SECTOR24    0x180000
#define SECTOR25    0x190000
#define SECTOR26    0x1A0000
#define SECTOR27    0x1B0000
#define SECTOR28    0x1C0000
#define SECTOR29    0x1D0000
#define SECTOR30    0x1E0000
#define SECTOR31    0x1F0000
    
    
// Commands to the SPI Flash. All instructions other than READ have a maximum
// clock frequency of fC (50 MHz). The READ instruction has a maximum clock
// frequency of fR (20 MHz). See pp 18 - 34 of the M25P16 datasheet for more
// information on the instruction set.
    
// Write Enable (WREN) enables writing to the memory. It must be sent before
// each PP, SE, BE, and WRSR instruction. Sets the WEL bit of the status
// register.
#define M25P16_WREN 0x06
    
// Write Disable (WRDI) disables writing to the memory. The same effect as this
// instruction occurs upon power-up and after PP, SE, BE, and WRSR instructions
// are completed. Resets the WEL bit of the status register.
#define M25P16_WRDI 0x04
    
// Read Identification (RDID) returns the 8-bit manufacturer identification.
#define M25P16_RDID         0x9F
    
// Read Status Register (RDSR) returns the contents of the 8-bit status
// register.
#define M25P16_RDSR         0x05
    
// Write Status Register (WRSR) writes an 8-bit value to the status register.
// The format is {SRWD, 0, 0, BP2, BP1, BP0, WEL, WIP}.
// SRWD -    Status Register Write Disable (irrelevant as Write Protect is hard-
//           wired to Vcc, disabling hardware protection mode).
// BP[2:0] - Block Protect bits allow write-protection against PP and SE
//           commands. This value sets the size of the area to be protected.
// WEL -     Write Enable Latch shows whether the memory can be written.
// WIP -     Write In Progress shows whether a write is currently in progress.
#define M25P16_WRSR         0x01
    
// Read Data Bytes (READ) reads data from the starting address sent after this
// instruction. The read will continue until the CS line is driven high.
#define M25P16_READ         0x03
    
// Read Data Bytes at Higher Speed (FAST_READ) reads data from the starting
// address sent after this instruction. The read will continue until the CS line
// is driven high. This instruction requires a dummy byte to be sent after the
// command is sent.
#define M25P16_FAST_READ    0x0B
    
// Page Program (PP) allows the memory can be programmed 1 - 256 bytes
// at a time (between 1 byte and 1 full page). The 3-byte starting address
// must be sent immediately following the command. If the amount of bytes
// between the starting address and the edge of the page is exceeded, the
// address will loop around to the start of the page.
#define M25P16_PP           0x02

// Sector Erase (SE) erases one sector of memory. This must be followed by a
// 3-byte address. The WIP bit in the status register will read 1 until the
// erasure is complete.
#define M25P16_SE           0xD8
    
// Bulk Erase (BE) erases the entire memory, all 32 sectors, at once. The BP
// bits in the status register must be set to 0 before this instruction is sent
// or else the erasure will not be executed. The WIP bit in the status register
// will read 1 until the erasure is complete.
#define M25P16_BE           0xC7
    
// Deep Power-down (DP) puts the memory module into a low-power idle state.
#define M25P16_DP           0xB9
    
// Release from Deep Power-down (RES)
#define M25P16_RES          0xAB

    // *************************************************************************
    // *************************************************************************
    // Section: Data Types
    // *************************************************************************
    // *************************************************************************



    // *************************************************************************
    // *************************************************************************
    // Section: Interface Functions
    // *************************************************************************
    // *************************************************************************

    void initialize_spi();
    void initialize_M25P16();

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PMODSF_H */

/* *****************************************************************************
 End of File
 */
