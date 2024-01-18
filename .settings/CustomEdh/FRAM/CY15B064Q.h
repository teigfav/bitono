

#include "cmsis_os.h"




/*** CY series command hex code definition ***/
//Comands
#define    FRAM_CMD_WREN       0x06    //SET write enable
#define    FRAM_CMD_WRDI       0x04    //Write disable
#define    FRAM_CMD_RSR        0x05    //Read Status Register
#define    FRAM_CMD_WSR        0x01    //Write Status Register
#define    FRAM_CMD_READ       0x03    //Read memory data
#define    FRAM_CMD_WRITE        0x02    //Write memory data

void ReadByteFram(uint16_t offset,uint8_t * data, uint32_t num_bytes);
void WriteByteFram(uint16_t offset,uint8_t * data, uint32_t num_bytes);
