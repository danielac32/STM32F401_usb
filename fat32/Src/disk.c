#include <disk.h>
#include <stdio.h>
#include <w25qxxx.h>

extern const w25qxxx_drv_t w25qxxx_drv;



int sd_init(void)
{
    
    w25qxxx_drv.init();
    flash_info_t *flash_info;
    flash_info = w25qxxx_drv.getcardinfo();
    return flash_info->card_size;
}
//-----------------------------------------------------------------
// sd_readsector: Read a number of blocks from SD card
//-----------------------------------------------------------------
int sd_readsector(unsigned int start_block, unsigned char *buffer, unsigned int sector_count)
{ 

    w25qxxx_drv.read(buffer, start_block, sector_count);
    return 1;
}
//-----------------------------------------------------------------
// sd_writesector: Write a number of blocks to SD card
//-----------------------------------------------------------------
int sd_writesector(unsigned int start_block, unsigned char *buffer, unsigned int sector_count)
{
        
    w25qxxx_drv.write(buffer, start_block, sector_count);

    return 1;
}




