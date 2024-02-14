////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 210210
// File : svCLI.c
// Desc. : Command Line Interface utility
//
////////////////////////////////////////////////////////////////////////////////
#include "BSPConfig.h"
#include "svCLI.h"
#include "ASCII_escape.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// Definitions
typedef enum {
	CLI_HISTORY_ADD,
	CLI_HISTORY_NEXT,
	CLI_HISTORY_PREV
} CLI_HISTORY_CMD_e;

#define	DEFAULT_PASSWORD		"tymict1@"

// Macros
// Global variables
// Static Variables
static BOOL					s_bPwdOK = TRUE;	// hellosmk 211110

static char					szCLI_PROMPT[10] = CLI_PROMPT_DEFAULT;
static int					snCLI_PROMPT_LENGTH = CLI_PROMPT_DEFAULT_LENGTH;

static char 	 			szCLIString[MAX_CLI_CHARS];
static char 	 			szCLIHistoryBuffer[MAX_CLI_HISTORY_BUFF_MAX][MAX_CLI_CHARS];
static int					snCLIHistoryCount = 0;
static int					snCLIHistoryPos = 0;

static unsigned int scntCLIString = 0;
static SVCLI_t*			sCLITableBegin = NULL;
static SVCLI_t*			sCLITablEnd = NULL;
static SVCLI_GET_CHAR_FUNC 	svGetChar = NULL;
static SVCLI_PUT_CHAR_FUNC 	svPutChar = NULL;
static SVCLI_COMMIT_FUNC 		svCommit = NULL;		// This is for optimize like using DMA
static SVCLI_BYPASS_FUNC 		svBypass = NULL;		// This is to support AT-command like feature - hellosmk 210225
static char									svExit = 0;					// hellosmk 211216
static BOOL									sbEnter = FALSE;		// hellosmk 211216
#if defined ( __GNUC__ )
extern char Table4CLI_start[];
extern char Table4CLI_end[];

#endif
// extern variables

// Functions
// svCLI_Init must be called before using CLI
void svCLI_Init(SVCLI_GET_CHAR_FUNC funcGetChar, SVCLI_PUT_CHAR_FUNC funcPutChar, SVCLI_COMMIT_FUNC funcCommit)
{
	svGetChar = funcGetChar;
	svPutChar = funcPutChar;
	svCommit = funcCommit;
	szCLIString[0] = 0;
#if defined ( __ICCARM__ )
#pragma section=".Table4CLI"
  sCLITableBegin = (SVCLI_t*)__section_begin(".Table4CLI");
  sCLITablEnd = (SVCLI_t*)__section_end(".Table4CLI");

#elif defined ( __GNUC__ )
  sCLITableBegin = (SVCLI_t*)Table4CLI_start;
  sCLITablEnd = (SVCLI_t*)Table4CLI_end;
#endif

  if (s_bPwdOK) {
    strcpy(szCLI_PROMPT, CLI_PROMPT_DEFAULT);
    snCLI_PROMPT_LENGTH = CLI_PROMPT_DEFAULT_LENGTH;
  }
  else {
    strcpy(szCLI_PROMPT, CLI_PROMPT_PWD);
    snCLI_PROMPT_LENGTH = CLI_PROMPT_PWD_LENGTH;
  }
}

static int svCLI_Printf(char* fmt,...)
{
static char msg[256];
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = vsprintf(msg, fmt, ap);
	va_end(ap);
	for (int i=0;i<len;i++) {
		svPutChar(msg[i]);
	}
	return len;
}

void svCLI_ShowHelp(void)
{
	if (svPutChar) {
		if (sCLITableBegin != sCLITablEnd) {
			for (unsigned int s=0;s<80;s++) svPutChar('=');
			svCLI_Printf("\r\n");
			SVCLI_t* pCLIEntry;
			int nCount = 1;
			for (pCLIEntry=sCLITableBegin;pCLIEntry<sCLITablEnd;pCLIEntry++) {
				svCLI_Printf(AES_RESET"%d."AES_BRIGHT_GREEN_FG" %s :"AES_BRIGHT_WHITE_FG" %s\r\n",nCount++, pCLIEntry->szCmd, pCLIEntry->szHelp ? pCLIEntry->szHelp : ":");
			}
			svCLI_Printf(AES_RESET);
			for (unsigned int s=0;s<80;s++) svPutChar('=');
		}
		else {	// No CLI Entry
			svCLI_Printf("No CLI Entries !!!");
		}
		svCLI_Printf("\r\n");
	}
	if (svCommit) {
		svCommit();
	}
}

// String length after stripped off space[0x20]
static int _strlenOffspace(const char* szString)
{
	int nCount = 0;
	if ((szString == NULL) || (*szString == 0))
		return 0;
	// Strip off first spaces
	while (*szString == ' ') szString++;
	// Count
	while (*szString) {
		if (*szString++ != ' ') {
			nCount++;
		}
		else
			break;
	}
	return nCount;
}

static char _toLower(char c)
{
  if ((c >= 'A') && (c <= 'Z')) {
    c = (c - 'A') + 'a';
  }
  return c;
}

static int _isPrintable(char c)
{
	if ((' ' <= c) && (c <= '~'))
		return 1;
	return 0;
}

static int _isHex(char c)
{
  if (((c >= '0') && (c <= '9')) ||				\
      ((c >= 'A') && (c <= 'F')) ||  	\
      ((c >= 'a') && (c <= 'f')))
    return 1;
	return 0;
}

static int _fromHex(char c) 
{
  int ret = 0;

  if ((c >= '0') && (c <= '9')) {
    ret = (c - '0');
  } 
	else if ((c >= 'a') && (c <= 'f')) {
    ret = (c - 'a' + 0x0a);
  } 
	else if ((c >= 'A') && (c <= 'F')) {
    ret = (c - 'A' + 0x0A);
  }
  return ret;
}

static int _strNoCaseCmp(const char *s1, const char *s2, int nLenghth)
{
  char c1, c2;

  if (nLenghth == 0)
    return 0;
  do {
		if ((c1 = _toLower(*s1++)) != (c2 = _toLower(*s2++))) {
			if ((c1 == 0) || (c2 == 0))
					break;
			return ((unsigned char)c1 - (unsigned char)c2);
		}
  } while (--nLenghth != 0);
  return 0;
}

static void _svMemCpy(char* dst, char *src, int nLength)
{
	while (nLength--) {
		*dst++=*src++;
	}
}

static void _svClearLine(void)
{
	svPutChar('\r');
	// Remove Strings
	for(int i=0;i<scntCLIString+snCLI_PROMPT_LENGTH;i++)
		svPutChar(' ');
	for(int i=0;i<scntCLIString+snCLI_PROMPT_LENGTH;i++)
		svPutChar('\b');
}

static void svCLIHandleHistory(CLI_HISTORY_CMD_e eHistoryCmd)
{
	switch (eHistoryCmd) {
		case CLI_HISTORY_ADD:
			if (snCLIHistoryCount == MAX_CLI_HISTORY_BUFF_MAX) {	// Full
				// Move
				for (int i=1;i<snCLIHistoryCount;i++) {
	        _svMemCpy(&szCLIHistoryBuffer[i-1][0],&szCLIHistoryBuffer[i][0],MAX_CLI_CHARS);
				}
				snCLIHistoryCount--;
			}
			_svMemCpy(&szCLIHistoryBuffer[snCLIHistoryCount][0], &szCLIString[0],MAX_CLI_CHARS);
			if (snCLIHistoryCount < MAX_CLI_HISTORY_BUFF_MAX)
				snCLIHistoryCount++;
			snCLIHistoryPos = snCLIHistoryCount;
			break;
		case CLI_HISTORY_PREV:
			snCLIHistoryPos--;
			if (snCLIHistoryPos < 0)
				snCLIHistoryPos = snCLIHistoryCount - 1;
			_svMemCpy(&szCLIString[0], &szCLIHistoryBuffer[snCLIHistoryPos][0],MAX_CLI_CHARS);
			break;
		case CLI_HISTORY_NEXT:
			snCLIHistoryPos++;
			if (snCLIHistoryPos >= snCLIHistoryCount)
				snCLIHistoryPos = 0;
			_svMemCpy(&szCLIString[0], &szCLIHistoryBuffer[snCLIHistoryPos][0],MAX_CLI_CHARS);
			break;
		default:
			break;
	}
}

static SVCLI_t* svCLI_Search(char **line)
{
	SVCLI_t* pCLIEntry;
	char* pCurrent = *line;
  // strip off leading spaces
  while (*pCurrent && *pCurrent == ' ') pCurrent++;
	for (pCLIEntry=sCLITableBegin;pCLIEntry<sCLITablEnd;pCLIEntry++) {
		int nCmdLength = _strlenOffspace(pCLIEntry->szCmd);
		int nArgLength = _strlenOffspace(pCurrent);
		pCurrent[nArgLength] = 0;	// NULL termination just after command for comparing
		if (nCmdLength >= nArgLength) {
			if (_strNoCaseCmp(pCurrent, pCLIEntry->szCmd, nCmdLength) == 0) {	// Partial Command OK !!
				pCurrent += (nArgLength + 1);
			  // strip off trailing spaces
			  while (*pCurrent && *pCurrent == ' ') pCurrent++;
				*line = pCurrent;		// remove command & return
				
				return pCLIEntry;
			}
		}
	}
	return NULL;
}

int svCLI_ParseNumber(char *s, unsigned int *val, char **res, char *delimiter)
{
  int first = 1;
  int radix = 10;
  char c;
  unsigned long result = 0;
  int digit;
		// strip off leading spaces
    while (*s == ' ') s++;
		if (*s == 0)
			return 0;
    while (*s) {
      if (first && (s[0] == '0') && (_toLower(s[1]) == 'x')) {
        radix = 16;
        s += 2;
      }
      first = 0;
      c = *s++;
      if (_isHex(c) && ((digit = _fromHex(c)) < radix)) {
        // Valid digit
        result = (result * radix) + digit;
      } 
			else {
        if (delimiter != (char *)0) {
          // See if this character is one of the delimiters
          char *dp = delimiter;
          while (*dp && (c != *dp)) dp++;
          if (*dp) break;  // Found a good delimiter
        }
        return 0;  // invalid number
      }
    }
    *val = result;
    if (res != (char **)0) {
      *res = s;
    }
    return 1;
}

static void svCLI_ParsenExecute()
{
	if (s_bPwdOK == FALSE) {	// Input Password
		if (strcmp(szCLIString, DEFAULT_PASSWORD) == 0) {
			s_bPwdOK = TRUE;
			svCLI_Printf("Welcome !!\r\n");
			strcpy(szCLI_PROMPT, CLI_PROMPT_DEFAULT);
			snCLI_PROMPT_LENGTH = CLI_PROMPT_DEFAULT_LENGTH;
			svCLI_ShowHelp();
		}
		else {
			svCLI_Printf("Invalid Password !!\r\n");
		}
		return;
	}
	SVCLI_t* pCLIEntry;
	if (sCLITableBegin == sCLITablEnd) {
		svCLI_Printf("No CLI Entries !!!\r\n");
		return;
	}
	char* pLine = szCLIString;
	pCLIEntry = svCLI_Search(&pLine);
	if (pCLIEntry) {
		// Parse more for CLI function
		unsigned int nFirst, nSecond, nCount;
		nCount = 0;
		nFirst = 0;
		nSecond = 0;
		char* pParseLine = pLine;
		char delim[3] = ", ";
		if (svCLI_ParseNumber(pLine, &nFirst, &pLine, delim)) {
			nCount++;
			if (svCLI_ParseNumber(pLine, &nSecond, &pLine, delim)) {
				nCount++;
			}
		}
		pCLIEntry->func(pParseLine,nCount,nFirst,nSecond);
	}
	else {
		svCLI_Printf("CLI command Not Found !! - Press Help\r\n");
	}
}

// hellosmk 210225 added
void svCLI_SetBypassFtn(SVCLI_BYPASS_FUNC pfnBypass, char* szPrompt, char szExit, BOOL bEnter)
{
	if ((pfnBypass == NULL) || (szPrompt == NULL) || (szExit == NULL)) {
		if (svBypass == NULL) {
			// Nothing to do..
			return;
		}
		strcpy(szCLI_PROMPT, CLI_PROMPT_DEFAULT);
		snCLI_PROMPT_LENGTH = CLI_PROMPT_DEFAULT_LENGTH;
		svBypass = NULL;

		svExit = NULL;
		sbEnter = FALSE;
		
		scntCLIString = 0;	// Clean Up Char
		// clean up line string
		for (int i=0;i<MAX_CLI_CHARS;i++) {
			szCLIString[i] = '\0';
		}
		svCLI_Printf("\r\nExit Bypass Mode\r\n%s", szCLI_PROMPT);

		if (svCommit) {
			svCommit();
		}
	}
	else {
		strncpy(szCLI_PROMPT, szPrompt, sizeof(szCLI_PROMPT) - 1);
		snCLI_PROMPT_LENGTH = strlen(szCLI_PROMPT);
		svBypass = pfnBypass;
		svExit = szExit;
		sbEnter = bEnter;
	}
}

void svCLI_Handler(void)
{
	static unsigned char sbInEsc = 0;
#if 0	// Initial prompt
	static unsigned char sbFirst = 1;
	if (sbFirst) {
		svCLI_Printf("\r\n%s", szCLI_PROMPT);
		sbFirst = 0;
	}
#endif	
	if (svGetChar) {
		char bSkipPrompt = 0;
		char szChar = svGetChar();
		if ((szChar != 0) && (szChar != '\n')) {
			if (szChar == '\r') {
				sbInEsc = 0;
				svCLI_Printf("\r\n");
				if (scntCLIString > 0) {
					// Parse and Execute
					szCLIString[scntCLIString] = '\0';	// NULL Termination
					// Save to history Buffer
					svCLIHandleHistory(CLI_HISTORY_ADD);					
					// Call Parser
					if (svBypass) {	// hellosmk 210225 added
						bSkipPrompt = 1;
						szCLIString[scntCLIString] = '\r';
						szCLIString[scntCLIString+1] = '\0';
						svBypass(szCLIString);
					}
					else {
						svCLI_ParsenExecute();
					}
					scntCLIString = 0;	// Clean Up Char
					// clean up line string
					for (int i=0;i<MAX_CLI_CHARS;i++) {
						szCLIString[i] = '\0';
					}
				}
				else if (sbEnter && svBypass) {
					const char cszRet[2] = {'\n',0};
					svBypass((char*)cszRet);
					bSkipPrompt = 1;
				}
				
				if (bSkipPrompt == 0) {
					svCLI_Printf(szCLI_PROMPT);
				}
			}
			else if (svBypass && (szChar == svExit)) {
				// Exit Bypass mode
				if (0x11 == svExit) {
					svCLI_Printf("^Q");
				}
				svBypass(NULL);	// Notify Exit !!
				svCLI_SetBypassFtn(NULL,NULL,NULL,FALSE);
				sbInEsc = 0;
			}
			else if (svBypass && (szChar == 0X1A)) {		// Ctrl+Z
				bSkipPrompt = 1;
				szCLIString[scntCLIString] = 0X1A;	//Ctrl+z
				szCLIString[scntCLIString+1] = '\0';	// NULL Termination
				svBypass(szCLIString);
			}
			else {
				if (szChar == '\b')	{ // Back space
					sbInEsc = 0;
					if (scntCLIString > 0) {
						_svClearLine();
						scntCLIString--;
						szCLIString[scntCLIString] = '\0';	// NULL Termination
						svCLI_Printf("%s%s", szCLI_PROMPT, szCLIString);
					}
				}
				else if (szChar == '[') {	// ESC sequence started
					sbInEsc = 1;
				}
				else if (_isPrintable(szChar)) {
					if (sbInEsc) {
						sbInEsc = 0;
						if (snCLIHistoryCount > 0) {
							switch (szChar) {
								case 'A' : // Up
									svCLIHandleHistory(CLI_HISTORY_PREV);
									// clear line
									_svClearLine();
									svCLI_Printf(szCLI_PROMPT);
									// echo back
									scntCLIString = svCLI_Printf("%s", szCLIString);
									break;
								case 'B' : // Down
									svCLIHandleHistory(CLI_HISTORY_NEXT);
									// clear line
									_svClearLine();
									svCLI_Printf(szCLI_PROMPT);
									// echo back
									scntCLIString = svCLI_Printf("%s", szCLIString);
									// echo back
									break;
								default:	// Do Nothing
									break;
							}
						}
						if (svCommit) {
							svCommit();
						}
						return;
					}
					if (scntCLIString < (MAX_CLI_CHARS-1)) {
						// echo back
						svPutChar(szChar);
						szCLIString[scntCLIString++] = szChar;
					}
				}
			}
		}
	}
	if (svCommit) {
		svCommit();
	}
}

// for CLI
#ifdef USE_CLI_FUNCTIONS
SVCLI_TABLE_ENTRY (HELP,"Help","Help : Show menu",CLI_Help);

static void CLI_Help(char* pArgLine, unsigned int nCount, unsigned int nFirst, unsigned int nSecond)
{
	svCLI_ShowHelp();
}
#endif // USE_CLI_FUNCTIONS



