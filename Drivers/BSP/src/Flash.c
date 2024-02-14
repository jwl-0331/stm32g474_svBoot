/*
 * Flash.c
 *
 *  Created on: Jan 29, 2024
 *      Author: mm940
 */


#include "Flash.h"
#include "svCLI.h"
#include "svDebug.h"

/*
#define FLASH_MAX_BANK            1
#define FLASH_MAX_SECTOR          64
#define FLASH_WRITE_SIZE          8    // 2 , 4, 8  , FLASH_TYPEPROGRAM_DOUBLEWORD - 64bit  allign
#define FLASH_SECTOR_SIZE         2048
*/

// SINGLE ?

#define FLASH_MAX_BANK            1
#define FLASH_MAX_SECTOR          64
#define FLASH_WRITE_SIZE          8    // 2 , 4, 8  , FLASH_TYPEPROGRAM_DOUBLEWORD - 64bit  allign
#define FLASH_SECTOR_SIZE         2048


static BOOL Flash_InSector(uint16_t sector_num, uint32_t addr, uint32_t length);

BOOL Flash_Init(void)
{
  BOOL ret = TRUE;


  return ret;
}
BOOL Flash_Erase(uint32_t addr, uint32_t length)
{
  BOOL ret = FALSE;

  int32_t start_sector = -1;
  int32_t end_sector = -1;

  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

  start_sector = -1;
  end_sector = -1;

  for(int i=0; i < FLASH_MAX_SECTOR; i++)
  {
    if(Flash_InSector(i, addr, length) == TRUE)
    {
      if(start_sector < 0)
      {
        start_sector = i;
      }
      end_sector = i;
    }
  }

  if(start_sector >= 0)
  {
    //HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError)
    HAL_StatusTypeDef status;
    // TypeErase : Mass or Page Erase ,Banks :   Page/PageAddress:  Nbpages:
    FLASH_EraseInitTypeDef EraseInit;
    uint32_t SectorError;
    //@ref FLASH_Type_Erase
    EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    //@ref FLASH_Banks  - FLASH_BANK_BOTH  , 1
    EraseInit.Banks = FLASH_BANK_BOTH;
    EraseInit.Page = start_sector;    // sector 번호 OR sector address , 확인해야댐 (페이지 번호를 주거나, 지우는 주소를 주거나)
    EraseInit.NbPages = (end_sector - start_sector) + 1;     // 페이지 개수 (시작 페이지 부터 몇개의 페이지를 지울 것인가)

    status = HAL_FLASHEx_Erase(&EraseInit, &SectorError);

    if(status == HAL_OK)
    {
      ret = TRUE;
    }
  }

  HAL_FLASH_Lock();
  return ret;
}
BOOL Flash_Write(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  BOOL ret = TRUE;
  HAL_StatusTypeDef status;

  // 주소도 64 bit 설정한 값으로 align 되어있어야한다 (FLASH_TYPEPROGRAM_DOUBLEWORD)
  if(addr%FLASH_WRITE_SIZE != 0) //16bit %2 , double-word (64-bit) %8
  {
    return FALSE;
  }
  // FLASH 메모리를 access 하기 위해 필요
  HAL_FLASH_Unlock();
  // erase 와 같은 방법
  for(int i = 0; i < length; i+=FLASH_WRITE_SIZE) // 64bit-8 , 16bit-2
  {
    uint64_t data;

    memcpy(&data, &p_data[i], FLASH_WRITE_SIZE);
    //data = p_data[i+0] << 0; //처음 data, shift하지 않는다 명시적으로 (<<0)
    //data |= p_data[i+1] << 8;  // 두번쨰 부터는 OR 연산, 8비트 shift

    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr + i, data);
    if(status != HAL_OK)
    {
      ret = FALSE;
      break;
    }
  }
  // FLASH 메모리를 access 하기 위해 필요
  HAL_FLASH_Lock();

  return ret;
}
BOOL Flash_Read(uint32_t addr, uint32_t *p_data, uint32_t length)
{
  BOOL ret = TRUE;

  // 1byte 데이터 포인터
  uint8_t *p_byte = (uint8_t *)addr;

  for(int i=0; i < length; i++)
  {
    p_data[i] = p_byte[i];
  }



  return ret;
}

BOOL Flash_InSector(uint16_t sector_num, uint32_t addr, uint32_t length)
{
  BOOL ret = FALSE;

  uint32_t sector_start;
  uint32_t sector_end;
  uint32_t flash_start;
  uint32_t flash_end;


  sector_start = FLASH_BASE + (sector_num * FLASH_SECTOR_SIZE);
  sector_end   = sector_start + FLASH_SECTOR_SIZE - 1;
  flash_start  = addr;
  flash_end    = addr + length - 1;


  if (sector_start >= flash_start && sector_start <= flash_end)
  {
    ret = TRUE;
  }

  if (sector_end >= flash_start && sector_end <= flash_end)
  {
    ret = TRUE;
  }

  if (flash_start >= sector_start && flash_start <= sector_end)
  {
    ret = TRUE;
  }

  if (flash_end >= sector_start && flash_end <= sector_end)
  {
    ret = TRUE;
  }

  return ret;
}
// Read, Erase, Write
SVCLI_TABLE_ENTRY (FLASH_TEST, "Flash", "Flash [1(Read),2(Erase),3(Write)] [0x0800 F000 ~ address]", CLI_FlashTest)
static void CLI_FlashTest(char *pArgLine, unsigned int nCount, unsigned int nFirst, unsigned int nAddress)
{
  if(nCount == 2)
  {
    switch(nFirst)
    {
      case 1:
        uint32_t rbuf[12];
        DebugMsg(DEBUGMSG_CLI,"=============================================\r\n");
        Flash_Read(nAddress, &rbuf[0],12);
        for(int i=0; i<12; i++)
        {
          DebugMsg(DEBUGMSG_CLI, "0x%X : 0x%X\n", nAddress + i, rbuf[i]);
        }
        break;
      case 2:
        if(Flash_Erase(nAddress, 32) == TRUE)
        {
          DebugMsg(DEBUGMSG_CLI, "Erase OK\n");
        }
        else
        {
          DebugMsg(DEBUGMSG_CLI, "Erase Fail\n");
        }
        break;
      case 3:
        uint8_t wbuf[32];
        for(int i=0; i<32; i++)
        {
          wbuf[i] = i;
        }
        if(Flash_Write(nAddress, wbuf, 32) == TRUE)
        {
          DebugMsg(DEBUGMSG_CLI, "Write OK\n");
        }
        else
        {
          DebugMsg(DEBUGMSG_CLI,"Write Fail\n");
        }
        break;
      default:
        DebugMsg(DEBUGMSG_CLI,"\r\nFlash [1(Read),2(Erase),3(Write)] [0x0800 F000 ~ address]\r\n");
        break;
    }
  }
}
