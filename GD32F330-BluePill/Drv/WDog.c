
//
// INCLUDE FILES
//
#include "Hardware.h"

void InitWatchdog(void)
{
  // Configure the Debug Interface so that it can stop the Watchdog
#ifdef WATCHDOG
  dbg_periph_enable(DBG_FWDGT_HOLD);
#endif

  /* enable IRC40K */
  rcu_osci_on(RCU_IRC40K);
  /* wait till IRC40K is ready */
  rcu_osci_stab_wait(RCU_IRC40K);

#ifndef DEBUG
  /* enable write access to FWDGT_PSC and FWDGT_RLD registers.
     FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
  fwdgt_config(625, FWDGT_PSC_DIV64); // set for ~1 second.
  fwdgt_enable();
#endif //not DBEUG
}

void PatWatchdog(void)
{
#ifndef DEBUG
  fwdgt_counter_reload();
#else
  __no_operation();
#endif
}
// EOF WDog.c
