//
//  INCLUDE FILES
//
#include    "Hardware.h"

void CRCCheck(void)
{
  /* Peripheral clock enable */
  rcu_periph_clock_enable(RCU_CRC);

//   pTempPtr  = (stAppHdr.pFlashLoadAddress);
//   pTempPtr += sizeof(app_header_st);
//   pAppStart = (uint32_t*)pTempPtr;
//   u32AppSize = ((uint32_t)stAppHdr.pTotalAppSize - sizeof(app_header_st));
//   u32AppSize /= sizeof(uint32_t);
//   u32CRCResult = CRC_CalcBlockCRC(pAppStart, u32AppSize);
//
  rcu_periph_clock_disable(RCU_CRC);

}
