/*
 *  drv_flash.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */



#include "hw.h"
#include "drv_flash.h"



typedef struct
{
  uint32_t addr;
  uint32_t length;
} flash_page_attr_t;




uint32_t            flash_page_total = FLASH_SECTOR_TOTAL;
flash_page_attr_t   flash_page_attr[FLASH_SECTOR_TOTAL];



static err_code_t drvFlashEraseSector(uint32_t sector);







bool drvFlashInit(void)
{


  flash_page_attr[0].addr   = 0x08000000;
  flash_page_attr[0].length = 32*1024;

  flash_page_attr[1].addr   = 0x08008000;
  flash_page_attr[1].length = 32*1024;

  flash_page_attr[2].addr   = 0x08010000;
  flash_page_attr[2].length = 32*1024;

  flash_page_attr[3].addr   = 0x08018000;
  flash_page_attr[3].length = 32*1024;

  flash_page_attr[4].addr   = 0x08020000;
  flash_page_attr[4].length = 128*1024;

  flash_page_attr[5].addr   = 0x08040000;
  flash_page_attr[5].length = 256*1024;

  flash_page_attr[6].addr   = 0x08080000;
  flash_page_attr[6].length = 256*1024;

  flash_page_attr[7].addr   = 0x080C0000;
  flash_page_attr[7].length = 256*1024;

  return true;
}

err_code_t drvFlashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  err_code_t err_code = OK;
  HAL_StatusTypeDef HAL_FLASHStatus = HAL_OK;
  uint32_t StartAddress = addr;
  uint32_t WriteSize;
  uint32_t WriteData;
  uint32_t i;
  uint32_t DataIndex;


  WriteSize = length / 4; // 32Bit

  if( (length%4) > 0 ) WriteSize++;

  DataIndex = 0;
  HAL_FLASH_Unlock();
  for( i=0; i<WriteSize; i++ )
  {
    WriteData  = p_data[ DataIndex++ ] << 0;
    WriteData |= p_data[ DataIndex++ ] << 8;
    WriteData |= p_data[ DataIndex++ ] << 16;
    WriteData |= p_data[ DataIndex++ ] << 24;

    HAL_FLASHStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartAddress+i*4, (uint64_t)WriteData);

    if( HAL_FLASHStatus != HAL_OK )
    {
        err_code = ERR_FLASH_WRITE;
      break;
    }
  }
  HAL_FLASH_Lock();

  return err_code;
}


err_code_t drvFlashRead(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  err_code_t err_code = OK;
  uint32_t Dataindex;
  uint32_t addr_cnt;


  Dataindex = 0;
  for (addr_cnt=0;addr_cnt<length;addr_cnt++)
  {
    p_data[Dataindex++] = *(volatile uint8_t*)(addr+addr_cnt);
  }

  return err_code;
}

err_code_t drvFlashErase(uint32_t addr, uint32_t length)
{
  err_code_t err_code = OK;
  uint32_t addr_begin;
  uint32_t addr_end;
  uint32_t target_addr_begin;
  uint32_t target_addr_end;

  uint32_t i;


  target_addr_begin = addr;
  target_addr_end   = addr + length;


  for (i=0; i<flash_page_total; i++)
  {
    addr_begin = flash_page_attr[i].addr;
    addr_end   = flash_page_attr[i].addr + flash_page_attr[i].length - 1;

    if ((addr_begin >= target_addr_begin) && (addr_begin <= target_addr_end))
    {
      err_code = drvFlashEraseSector(i);
    }
    else if((addr_end >= target_addr_begin) && (addr_end <= target_addr_end) )
    {
      err_code = drvFlashEraseSector(i);
    }
  }


  return err_code;
}

err_code_t drvFlashEraseSector(uint32_t sector)
{
  err_code_t err_code = OK;
  HAL_StatusTypeDef HAL_FLASHStatus = HAL_OK;
  FLASH_EraseInitTypeDef pEraseInit;
  uint32_t SectorError;


  pEraseInit.TypeErase  = FLASH_TYPEERASE_SECTORS;
  pEraseInit.Sector     = sector;
  pEraseInit.NbSectors  = 1;
  pEraseInit.VoltageRange = VOLTAGE_RANGE_3;


  HAL_FLASH_Unlock();


  HAL_FLASHStatus = HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
  if(HAL_FLASHStatus != HAL_OK)
  {
    err_code = ERR_FLASH_ERASE;
  }

  HAL_FLASH_Lock();

  return err_code;
}

err_code_t drvFlashEraseSectors(uint32_t start_sector, uint32_t sector_cnt )
{

  err_code_t err_code = OK;
  HAL_StatusTypeDef HAL_FLASHStatus = HAL_OK;
  FLASH_EraseInitTypeDef pEraseInit;
  uint32_t SectorError;


  pEraseInit.TypeErase  = FLASH_TYPEERASE_SECTORS;
  pEraseInit.Sector     = start_sector;
  pEraseInit.NbSectors  = sector_cnt;
  pEraseInit.VoltageRange = VOLTAGE_RANGE_3;


  HAL_FLASH_Unlock();

  HAL_FLASHStatus = HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
  if(HAL_FLASHStatus != HAL_OK)
  {
    err_code = ERR_FLASH_ERASE;
  }

  HAL_FLASH_Lock();

  return err_code;
}

