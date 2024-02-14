////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 180223
// File : Debug.c
// Desc. : Debug message related stuffs
//
////////////////////////////////////////////////////////////////////////////////
#include "BSPConfig.h"
#include "svDebug.h"
#include "svPlatform.h"
#include "lwrb.h"
#include "svCLI.h"
#include "UART.h"

#include <stdio.h>
#include <stdarg.h>

// Definitions
#define DEBUG_MESSAGE_SIZE									128
#define DEBUG_TX_BUFFER_SIZE								512

// Macros
svRINGBUFF_DEF(g_rbDebugRx, DEBUG_MESSAGE_SIZE);

// Global variables
uint32_t g_DebugMsgLevel = DEBUG_VERBOSE;
BOOL g_bDebugTx = FALSE;
lwrb_t	g_lwrbDebugTx;

// Static Variables
static uint8_t s_byDebugTxRingBuffer[DEBUG_TX_BUFFER_SIZE+4];			// Tx Ring Buffer

// extern variables

// Functions
extern BOOL  IsUartConnected(uint8_t ch);

// for printf stream to Debug UART
#ifdef SUPPORT_printf
int fputc(int ch, FILE *f) {
  while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
	huart1.Instance->DR = ch;
  return ch;
}
#endif

char svDebugRxChar(void)
{
	if (svRingBuffer_GetCount(&g_rbDebugRx) > 0) {
		return svRingBuffer_Pop(&g_rbDebugRx);
	}
	else {
		return 0;
	}
}

void svDebugProcess(void)
{
	if (IsUartConnected(_DEF_UART1)) {
	  svCLI_Handler();
		if (g_bDebugTx == TRUE) {
			size_t nLength = lwrb_get_linear_block_read_length(&g_lwrbDebugTx);
			if (nLength > 0) {
			  /*
				if (nLength > UART_TX_DATA_SIZE) {
					nLength = UART_TX_DATA_SIZE;
				}
				*/
				UART_Write(_DEF_UART1, lwrb_get_linear_block_read_address(&g_lwrbDebugTx), nLength);
				lwrb_skip(&g_lwrbDebugTx, nLength);
			}
		}
		/*
		else {	// hellosmk 220523 Bug ??
			extern	UART_HandleTypeDef huart1;		// reside usb_device.c
		  if(huart1.gState == 0)
		  {
				g_bDebugTx = TRUE;
		  }
		}
		*/
	}
}

void svDebugCallback(LWRB_VOLATILE struct lwrb* buff, lwrb_evt_type_t evt, size_t bp)
{
	if (evt == LWRB_EVT_FULL) {
		if (IsUartConnected(_DEF_UART1)) {
			uint32_t uTick = HAL_GetTick();
			while (GetElapsedTime(uTick) < 100) {
				if (g_bDebugTx == TRUE) {
					break;
				}
				// HAL_Delay(1);
			}
      g_bDebugTx = TRUE;
			svDebugProcess();
		}
	}
}

__inline size_t svDebugGetTxFree(void)
{
	return lwrb_get_free(&g_lwrbDebugTx);
}

void svDebugInit(void)
{
	svRingBuffer_Reset(&g_rbDebugRx);
	svCLI_Init(&svDebugRxChar, &svDebugPutChar, NULL);

  lwrb_init(&g_lwrbDebugTx, &s_byDebugTxRingBuffer[0], DEBUG_TX_BUFFER_SIZE);
	lwrb_set_evt_fn(&g_lwrbDebugTx, svDebugCallback);
}

void svSetDebugMsgLevel(uint32_t uDebugMsgLevel)
{
	if (uDebugMsgLevel > DEBUG_VERBOSE)
		uDebugMsgLevel = DEBUG_VERBOSE;
	g_DebugMsgLevel = uDebugMsgLevel;
}

void svDebugPutChar(char cPut)
{
	lwrb_write(&g_lwrbDebugTx, (const void*)(&cPut), 1);
}

void svDebugPutString(char* szMsg)
{
	if ((szMsg == NULL) || (szMsg[0] == 0)) {
		return;
	}
#if 0	
	size_t lenMax =	lwrb_get_free(&g_lwrbDebugTx);
	for (int i=1;i<lenMax;i++) {
		if (szMsg[i] == 0) {
			lenMax = i;
			break;
		}
	}
#else
	size_t lenMax = 0;
	char* pChar = szMsg;
	while (*pChar != 0) {
		lenMax++;
		pChar++;
	}
#endif
	lwrb_write(&g_lwrbDebugTx, (const void*)szMsg, lenMax);
}

void svDebugMessage(char* fmt,...)
{
	static char s_msg4Debug[DEBUG_MESSAGE_SIZE];
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = vsprintf(s_msg4Debug, fmt, ap);
	va_end(ap);

	lwrb_write(&g_lwrbDebugTx, (const void*)s_msg4Debug, len);
}

static int _isPrintable(char c)
{
	if ((' ' <= c) && (c <= '~'))
		return 1;
	return 0;
}

void svDebugFormattedString(uint8_t* byData, int nDataLength, int nDispLength)
{
	if (byData == NULL)
		return;
	for (int i=0;i<nDispLength*6+3+nDispLength;i++) {
		svDebugPutChar('=');
	}
	svDebugPrintf_LFCR();
	int nLine = (nDataLength + nDispLength - 1) / nDispLength;
	for (int i=0;i<nLine;i++) {
		int nCount = 0;
		// Print Data
		while (nCount < nDispLength) {
			if ((i*nDispLength + nCount) < nDataLength) {
				if (byData[i*nDispLength + nCount] == 0x00)
					svDebugPutString(AES_WHITE_FG);
				else if (byData[i*nDispLength + nCount] == 0xff)
					svDebugPutString(AES_GREEN_FG);
				else
					svDebugPutString(AES_BRIGHT_YELLOW_FG);
				svDebugMessage("0x%02x, "AES_RESET, byData[i*nDispLength + nCount]);
			}
			else
				svDebugPutString("      ");
			nCount++;
		}
		svDebugPutString(AES_RESET);
		// Delimiter
		svDebugPutString(" | ");
		// Print ASCII
		nCount = 0;
		while (nCount < nDispLength) {
			if ((i*nDispLength + nCount) < nDataLength)
				svDebugMessage("%c", _isPrintable(byData[i*nDispLength + nCount]) ? byData[i*nDispLength + nCount] : '.');
			else
				svDebugPutChar(' ');
			nCount++;
		}
		svDebugPrintf_LFCR();
	}
	for (int i=0;i<nDispLength*6+3+nDispLength;i++) {
		svDebugPutChar('=');
	}
	svDebugPrintf_LFCR();
}
