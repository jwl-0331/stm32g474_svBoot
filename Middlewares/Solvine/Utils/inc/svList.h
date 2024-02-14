////////////////////////////////////////////////////////////////////////////////
// Author : Solvine
// Creation Date : 200211
// File : svList.h
// Desc. : Linked List helper functions
//
////////////////////////////////////////////////////////////////////////////////

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVLIST_H__
#define __SVLIST_H__

// Definitions

#ifndef NULL
#define NULL		((void*)0)
#endif

typedef struct svListMember_s {
  struct svListMember_s *pNext;
} svListMember_t;

typedef struct svLinkedList {
  unsigned int size;
  svListMember_t *pFirst;
  svListMember_t *pLast;
} svList_t;

// Function Declarations

void svList_Init(svList_t* pList);
void svList_RemoveMembers(svList_t* pList);
unsigned int svList_Size(svList_t* pList);

void svList_PushFirst(svList_t* pList, svListMember_t* pMember);
void svList_PushLast(svList_t* pList, svListMember_t* pMember);

svListMember_t* svList_PopFirst(svList_t* pList);

svListMember_t* svList_PeekFirst(svList_t* pList);
svListMember_t* svList_PeekLast(svList_t* pList);

svListMember_t* svList_GetNext(svListMember_t* pMember);

#endif // __SVLIST_H__

