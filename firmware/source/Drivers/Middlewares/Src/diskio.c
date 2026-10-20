#include "malloc_sram.h"
#include "sdcard.h"
#include "norflash.h"
#include "diskio.h"
#define SD_CARD                 0
#define NOR_FLASH               1
#define SPI_FLASH_SECTOR_SIZE   512
#define SPI_FLASH_SECTOR_COUNT  12*1024*2
#define SPI_FLASH_BLOCK_SIZE    8
#define SPI_FLASH_FATFS_BASE    0
DSTATUS disk_status(BYTE pdrv){
    return RES_OK;
}
DSTATUS disk_initialize(BYTE pdrv){
    return 0;
}
DRESULT disk_read(BYTE pdrv,BYTE *buff, DWORD sector,UINT count){
    if(!count)return RES_PARERR;
    if(pdrv==SD_CARD){
        SD_card_read_disk_with_buf(buff,sector,count);
        return RES_OK;
    }
    if(pdrv==NOR_FLASH){
        Norflash_read(buff,SPI_FLASH_FATFS_BASE+sector*SPI_FLASH_SECTOR_SIZE,count*SPI_FLASH_SECTOR_SIZE);
        return RES_OK;
    }
    return RES_ERROR;
}
DRESULT disk_write(BYTE pdrv,const BYTE *buff,DWORD sector,UINT count){
    if(!count)return RES_PARERR;
    if(pdrv==SD_CARD){
        SD_card_write_disk_with_buf((uint8_t*)buff,sector,count);
        return RES_OK;
    }
    if(pdrv==NOR_FLASH){
        Norflash_write((uint8_t*)buff,SPI_FLASH_FATFS_BASE+sector*SPI_FLASH_SECTOR_SIZE,SPI_FLASH_SECTOR_SIZE*count);
        return RES_OK;
    }
    return RES_ERROR;
}
DRESULT disk_ioctl (BYTE pdrv,BYTE cmd,void *buff){
    if(pdrv==SD_CARD){
        if(cmd==CTRL_SYNC){
            return RES_OK;
        }
        if(cmd==GET_SECTOR_SIZE){
            *(DWORD *)buff=512;
            return RES_OK;
        }
        if(cmd==GET_BLOCK_SIZE){
            *(WORD *)buff=SD_card_info_handle.LogBlockSize;
            return RES_OK;
        }
        if(cmd==GET_SECTOR_COUNT){
            *(DWORD *)buff=SD_card_info_handle.LogBlockNbr;
            return RES_OK;
        }
        return RES_PARERR;
    }
    if(pdrv==NOR_FLASH){
        if(cmd==CTRL_SYNC){
            return RES_OK;
        }
        if(cmd==GET_SECTOR_SIZE){
            *(WORD *)buff=SPI_FLASH_SECTOR_SIZE;
            return RES_OK;
        }
        if(cmd==GET_BLOCK_SIZE){
            *(WORD *)buff=SPI_FLASH_BLOCK_SIZE;
            return RES_OK;
        }
        if(cmd==GET_SECTOR_COUNT){
            *(DWORD *)buff=SPI_FLASH_SECTOR_COUNT;
            return RES_OK;
        }
        return RES_PARERR;
    }
    return RES_ERROR;
}
