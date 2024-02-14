////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 200211
// File : svList.c
// Desc. : Linked List helper functions
//
////////////////////////////////////////////////////////////////////////////////
#include "svList.h"
#include <stdlib.h>			// for malloc, free

// Definitions
#define MEM_ALLOC(s)		malloc(s)
#define MEM_FREE(s)			free((void*)s)

// Macros
// Global variables
// Static Variables
// extern variables

// Functions
void svList_Init(svList_t *pList)
{
  pList->size = 0;
  pList->pFirst = NULL;
  pList->pLast = NULL;
}

// Dynamic memory allocation version
void svList_RemoveMembers(svList_t* pList)
{
  svListMember_t* pMember;
  svListMember_t* pNext;
	pMember = pList->pFirst;
  while (pMember != NULL) {
		pNext = pMember->pNext;
		MEM_FREE(pMember);
		pMember = pNext;
  }
	svList_Init(pList);
}

unsigned int svList_Size(svList_t* pList)
{
  return (pList->size);
}

void svList_PushFirst(svList_t* pList, svListMember_t* pMember)
{
  if (pList->pFirst == NULL) {		// No member
	  pMember->pNext = NULL;
    pList->pFirst = pList->pLast = pMember;
  }
  else {
		pMember->pNext = pList->pFirst;
    pList->pFirst = pMember;
  }
  pList->size++;
}

void svList_PushLast(svList_t* pList, svListMember_t* pMember)
{
  pMember->pNext = NULL;
  if (pList->pLast == NULL) {			// No member
    pList->pFirst = pList->pLast = pMember;
  }
  else {
    pList->pLast->pNext = pMember;
    pList->pLast = pMember;
  }
  pList->size++;
}

svListMember_t* svList_PopFirst(svList_t* pList)
{
  svListMember_t* pMember = pList->pFirst;
  if (pMember != NULL) {
    if (pMember == pList->pLast) {		// Only One member !!
      pList->pLast  = NULL;
      pList->pFirst = NULL;
    }
    else {
      pList->pFirst = pMember->pNext;
    }
    pMember->pNext = NULL;	// disconnect link of this member
    pList->size--;
  }
  return pMember;
}

svListMember_t* svList_PeekFirst(svList_t* pList)
{
  return (pList->pFirst);
}

svListMember_t* svList_PeekLast(svList_t* pList)
{
  return (pList->pLast);
}

svListMember_t* svList_GetNext(svListMember_t* pMember)
{
  return (pMember->pNext);
}

