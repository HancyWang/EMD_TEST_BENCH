#ifndef __KEY_CHECK_TASK
#define __KEY_CHECK_TASK
#include "datatype.h"
#include "stdint.h"
#include "hardware.h"

#define KEY_CHECK_PERIOD  5

#ifdef INTERRUPT_3_KEYS
extern BOOL b_startKey_pressed;
extern BOOL b_startKey_pressed;
extern BOOL b_nextKey_pressed;
extern BOOL b_stopKey_pressed;

extern BOOL b_startKey_falling;
extern BOOL b_nextKey_falling;
extern BOOL b_stopKey_falling;
extern BOOL b_startKey_falled;
extern BOOL b_nextKey_falled;
extern BOOL b_stopKey_falled;

extern uint8_t startKey_falling_cnt;
extern uint8_t nextKey_falling_cnt;
extern uint8_t stopKey_falling_cnt;
extern uint8_t startKey_raising_cnt;
extern uint8_t nextKey_raising_cnt;
extern uint8_t stopKey_raising_cnt;

extern BOOL b_startKey_raising;
extern BOOL b_nextKey_raising;
extern BOOL b_stopKey_raising;
extern BOOL b_startKey_raised;
extern BOOL b_nextKey_raised;
extern BOOL b_stopKey_raised;

extern BOOL* p_b_Key_pressed;
extern BOOL* p_b_KeyFalling;
extern BOOL* p_b_KeyFalled;
extern BOOL* p_b_KeyRaising;
extern BOOL* p_b_KeyRaised;
extern uint8_t* p_KeyFallingCnt;
extern uint8_t* p_KeyRaisingCnt;
#endif

void key_check_task(void);
void Init_all_keys(void);
#endif //__KEY_CHECK_TASK
