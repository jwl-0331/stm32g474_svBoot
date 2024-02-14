////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 210210
// File : svCLI.h
// Desc. : Command Line Interface utility
//
////////////////////////////////////////////////////////////////////////////////

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVCLI_H__
#define __SVCLI_H__

#define USE_CLI_FUNCTIONS			// for image size

#define MAX_CLI_HISTORY_BUFF_MAX		3		
#define MAX_CLI_CHARS								128
#define CLI_PROMPT_DEFAULT					"svCLI>"
#define CLI_PROMPT_DEFAULT_LENGTH		6

#define CLI_PROMPT_PWD							"Password>"
#define CLI_PROMPT_PWD_LENGTH				9


#ifndef NULL
#define NULL		((void*)0)
#endif

// Definitions
typedef void (*SVCLI_CMD_FUNC)(char* pArgLine, unsigned int nCount, unsigned int nFirst, unsigned int nSecond);
typedef char (*SVCLI_GET_CHAR_FUNC)(void);
typedef void (*SVCLI_PUT_CHAR_FUNC)(char chPut);
typedef void (*SVCLI_COMMIT_FUNC)(void);
typedef void (*SVCLI_BYPASS_FUNC)(char* chResult);

typedef struct {
	char* 					szCmd;
	char* 					szHelp;
	SVCLI_CMD_FUNC	func;
} SVCLI_t;

typedef void declare_SVCLI_CMD_FUNC(char* pArgLine, unsigned int nCount, unsigned int nFirst, unsigned int nSecond);

#if defined ( __ICCARM__ )

#define SVCLI_TABLE_ENTRY(_name_,_cmd_,_help_,_func_)	\
static declare_SVCLI_CMD_FUNC _func_;	\
_Pragma ("location = \".Table4CLI\"")	\
const SVCLI_t _SVCLI_ENTRY_##_name_ = {_cmd_, _help_, _func_ };

#elif defined ( __GNUC__ )

#define SVCLI_TABLE_ENTRY(_name_,_cmd_,_help_,_func_)	\
static declare_SVCLI_CMD_FUNC _func_;	\
const SVCLI_t _SVCLI_ENTRY_##_name_ __attribute__ ((section (".Table4CLI"))) = {_cmd_, _help_, _func_ };

#endif

// Function Declarations

void svCLI_Init(SVCLI_GET_CHAR_FUNC funcGetChar, SVCLI_PUT_CHAR_FUNC funcPutChar, SVCLI_COMMIT_FUNC funcCommit);
void svCLI_SetBypassFtn(SVCLI_BYPASS_FUNC pfnBypass, char* szPrompt, char szExit, BOOL bEnter);		// hellosmk 210225

//void svCLI_Init(SVCLI_GET_CHAR_FUNC funcGet, SVCLI_PUT_CHAR_FUNC funcPut);
int svCLI_ParseNumber(char *s, unsigned int *val, char **res, char *delimiter);
void svCLI_ShowHelp(void);
void svCLI_Handler(void);
#endif	// __SVCLI_H__

