#ifndef _SUPERX4_SAB_H_
#define _SUPERX4_SAB_H_

extern void superx4_Send(void);
void canStartInit(void);
void can_locationStateCheck_thread_resume(void);
void can_locationStateCheck_thread_pause(void);
void can_locationStateCheck_thread_create(void);
void can_locationStateCheck_thread_wait(void);

namespace xagcan
{
    


}

#endif