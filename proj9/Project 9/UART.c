/*
 * UART.c
 *
 *  Created on: Oct 28, 2024
 *      Author: happy
 */


#include  "msp430.h"
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"
#include "Include\header.h"


extern read_pointer;

extern char display_line[4][11]; //

extern unsigned int iot_en_start ;
extern unsigned int iot_en_timer ;
extern unsigned int iotcase;


char process_buffer[25];                 // Size for appropriate Command Length
char pb_index;
#define BEGINNING (0)
#define SMALL_RING_SIZE (11)
#define LARGE_RING_SIZE (11)

//----------------------------------------------------------------------------
volatile unsigned int usb_rx_ring_wr;
volatile unsigned int usb_rx_ring_rd;
volatile unsigned int usb_tx_ring_wr;
volatile unsigned int usb_tx_ring_rd;
unsigned int tx_count = 1;
volatile unsigned int t_to_iot; //enable for transmitting information into the iot.
unsigned int temp3;


char IOT_Char_Rx[11] = "          \0";
char IOT_Char_Tx[40];
unsigned int iot_rx_ring_wr;
unsigned int iot_tx_count;
unsigned int transmit_serial; //off to keep from transmitting to pc until i receive a character from the pc
unsigned int transfer;
unsigned int transfer2;

volatile char USB_Char_Rx[11];
volatile char USB_Char_Tx[11];

char RingBuffer[11];
char rd;
char wr;
void Init_Serial_UCA1(char speed){
 //-----------------------------------------------------------------------------
//                                               TX error (%) RX error (%)
 //  BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
 //  8000000    4800     1     104     2     0xD6   -0.08 0.04 -0.10 0.14
 //  8000000    9600     1      52     1     0x49   -0.08 0.04 -0.10 0.14
 //  8000000   19200     1      26     0     0xB6   -0.08 0.16 -0.28 0.20
 //  8000000   57600     1       8    10     0xF7   -0.32 0.32 -1.00 0.36
 //  8000000  115200     1       4     5     0x55   -0.80 0.64 -1.12 1.76
 //  8000000  460800     0      17     0     0x4A   -2.72 2.56 -3.76 7.28
 //-----------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
  UCA1CTLW0 = 0;
  UCA1CTLW0 |=  UCSWRST ;              // Put eUSCI in reset
  UCA1CTLW0 |=  UCSSEL__SMCLK;         // Set SMCLK as fBRCLK
  UCA1CTLW0 &= ~UCMSB;                 // MSB, LSB select
  UCA1CTLW0 &= ~UCSPB;                 // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
  UCA1CTLW0 &= ~UCPEN;                 // No Parity
  UCA1CTLW0 &= ~UCSYNC;
  UCA1CTLW0 &= ~UC7BIT;
  UCA1CTLW0 |=  UCMODE_0;
 //    BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
 //    8000000  115200     1       4     5     0x55   -0.80 0.64 -1.12 1.76
 //    UCA?MCTLW = UCSx + UCFx + UCOS16
  UCA1BRW = 4 ;                        // 115,200 baud
  UCA1MCTLW = 0x5551 ;
  UCA1CTLW0 &= ~UCSWRST ;              // release from reset
  UCA1TXBUF = 0x00;                    // Prime the Pump
  UCA1IE |= UCRXIE;                    // Enable RX interrupt

//  transmit_serial = 0;

 //----
}
void Init_Serial_UCA0(char speed){
 //-----------------------------------------------------------------------------
//                                               TX error (%) RX error (%)
 //  BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
 //  8000000    4800     1     104     2     0xD6   -0.08 0.04 -0.10 0.14
 //  8000000    9600     1      52     1     0x49   -0.08 0.04 -0.10 0.14
 //  8000000   19200     1      26     0     0xB6   -0.08 0.16 -0.28 0.20
 //  8000000   57600     1       8    10     0xF7   -0.32 0.32 -1.00 0.36
 //  8000000  115200     1       4     5     0x55   -0.80 0.64 -1.12 1.76
 //  8000000  460800     0      17     0     0x4A   -2.72 2.56 -3.76 7.28
 //-----------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
  UCA0CTLW0 = 0;
  UCA0CTLW0 |=  UCSWRST ;              // Put eUSCI in reset
  UCA0CTLW0 |=  UCSSEL__SMCLK;         // Set SMCLK as fBRCLK
  UCA0CTLW0 &= ~UCMSB;                 // MSB, LSB select
  UCA0CTLW0 &= ~UCSPB;                 // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
  UCA0CTLW0 &= ~UCPEN;                 // No Parity
  UCA0CTLW0 &= ~UCSYNC;
  UCA0CTLW0 &= ~UC7BIT;
  UCA0CTLW0 |=  UCMODE_0;
 //    BRCLK   Baudrate UCOS16  UCBRx  UCFx    UCSx    neg   pos  neg  pos
 //    8000000  115200     1       4     5     0x55   -0.80 0.64 -1.12 1.76
 //    UCA?MCTLW = UCSx + UCFx + UCOS16
  UCA0BRW = 4 ;                        // 115,200 baud
  UCA0MCTLW = 0x5551 ;
  UCA0CTLW0 &= ~UCSWRST ;              // release from reset

  UCA0TXBUF = 0x00;                    // Prime the Pump
  UCA0IE |= UCRXIE;                    // Enable RX interrupt
 //----
}

// Global Variables
void USCI_A0_transmit(void){             // Transmit Function for USCI_A0
// Contents must be in process_buffer
// End of Transmission is identified by NULL character in process_buffer
// process_buffer includes Carriage Return and Line Feed
pb_index = 0;                          // Set Array index to first location
UCA0IE |= UCTXIE;                      // Enable TX interrupt
}
// Global Variables                    // Index for process_buffer
void USCI_A1_transmit(void){             // Transmit Function for USCI_A0
// Contents must be in process_buffer
// End of Transmission is identified by NULL character in process_buffer
// process_buffer includes Carriage Return and Line Feed
pb_index = 0;                          // Set Array index to first location
UCA1IE |= UCTXIE;                      // Enable TX interrupt
}
//------------------------------------------------------------------------------

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    unsigned int temp;
    switch(__even_in_range(UCA0IV,0x08)){
    case 0: // Vector 0 - no interrupt
        break;
    case 2: // Vector 2 - RXIFG
        temp = iot_rx_ring_wr++;
        transfer2 = UCA0RXBUF;
//        if(transfer2 == '^') {
//            read_commands = 1;
//        }
        IOT_Char_Rx[temp] = transfer2; // RX -> USB_Char_Rx character
        if (iot_rx_ring_wr >= (sizeof(IOT_Char_Rx))){
            iot_rx_ring_wr = BEGINNING; // Circular buffer back to beginning
        }
        //project 9 code
        USB_Char_Tx[0] = transfer2;
        USB_Char_Tx[1] = 0x00;
        UCA1IE |= UCTXIE;
        break;
    case 4: // Vector 4 – TXIFG
        temp3 = IOT_Char_Tx[iot_tx_count];
        UCA0TXBUF = IOT_Char_Tx[iot_tx_count];
        IOT_Char_Tx[iot_tx_count++] = 0;
        if(IOT_Char_Tx[iot_tx_count] == 0x00){
            UCA0IE &= ~UCTXIE;
            //to turn back on, do UCA0IE |= UCTXIE;
            iot_tx_count = 0;
        }
        break;
    default: break;
    }
}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    unsigned int temp;
    switch(__even_in_range(UCA1IV,0x08)){
    case 0: // Vector 0 - no interrupt
        break;
    case 2: // Vector 2 - RXIFG
        temp = usb_rx_ring_wr++;
        transfer = UCA1RXBUF;
        if(transfer == '^'){ //if detect a caret, dont transfer out
            t_to_iot = 1;
        }
        if(t_to_iot != 1){ //if allowed to transmit to iot, dont enable the loop through.
            IOT_Char_Tx[0] = transfer; //start the loop part
            IOT_Char_Tx[1] = 0x00;
            UCA0IE |= UCTXIE; // enable the interrupt
        }
        if(t_to_iot == 1 && transfer == 10){
            t_to_iot = 0;
        }
        USB_Char_Rx[temp] = transfer; // RX -> USB_Char_Rx character
        if (usb_rx_ring_wr >= (sizeof(USB_Char_Rx))){
            usb_rx_ring_wr = BEGINNING; // Circular buffer back to beginning
        }
        break;
    case 4: // Vector 4 – TXIFG
        UCA1TXBUF = USB_Char_Tx[tx_count];
        USB_Char_Tx[tx_count++] = 0;
        if(USB_Char_Tx[tx_count] == 0x00){
            UCA1IE &= ~UCTXIE;
            //to turn back on, do UCA0IE |= UCTXIE;
            tx_count = 0;
        }

        break;
    default: break;
    }
}
//------------------------------------------------------------------------------

void init_iot(void){
    if(iotcase == 2 && iot_en_timer >= 1000){ //give car time to process, then send server/iot commands

        strcpy(IOT_Char_Tx,"AT+CIPSERVER=1,8080\r\n");
        UCA0IE |= UCTXIE;

        iot_en_start = 0;
        iot_en_timer = 0;
        iotcase = 0;

    } else if(iotcase == 1 && iot_en_timer >= 800){


        strcpy(IOT_Char_Tx,"AT+CIPMUX=1\r\n");
        UCA0IE |= UCTXIE;
        iotcase = 2;


    } else if(iotcase == 0 && iot_en_timer >= 275){
      strcpy(display_line[0], "   NCSU   ");
      strcpy(display_line[1], "IP ADDRESS");
      strcpy(display_line[2], "  10.155. ");
      strcpy(display_line[3], "   3.55   ");
      display_changed = TRUE;

      iotcase = 1;
      P3OUT |= IOT_EN_CPU;
//        iot_en_start = 0;
//        iot_en_timer = 0;
  }

}
