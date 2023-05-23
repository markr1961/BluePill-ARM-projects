#ifndef     _ICF_SYMBOLS_H
#define     _ICF_SYMBOLS_H

#define     FLASH_START_ADDRESS         ((void *) 0x08000000)
#define     ICFEDIT_IntVec_Start        &__ICFEDIT_intvec_start__
#define     ICFEDIT_Region_ROM_Start    &__ICFEDIT_region_ROM_start__
#define     ICFEDIT_Region_ROM_End      &__ICFEDIT_region_ROM_end__
#define     ICFEDIT_Region_RAM_Start    &__ICFEDIT_region_RAM_start__
#define     ICFEDIT_Region_RAM_End      &__ICFEDIT_region_RAM_end__
#define     ICFEDIT_Size_CStack         &__ICFEDIT_size_cstack__
#define     ICFEDIT_Size_Heap           &__ICFEDIT_size_heap__

#define     ICF_Region_ROM_Size         &__ICF_region_ROM_size
#define     ICF_Region_RAM_Size         &__ICF_region_RAM_size

//
//  EXPORTED TYPES
//

//
//  EXPORTED VARIABLES
//

//  These are defined in the linker .icf file by default. However they must
//  be exported from the .icf file which isn't done by default.
//  See "Interaction between the tools and your application" in
//  EWARM_DevelopmentGuide.ENU.pdf

//  Note: The linker defines the symbols as though they're variables and
//        therefore the & (address-of) operator MUST be used to get the
//        actual symbol value defined in the .icf file!!! The macro's above
//        should be used to perform this task.

extern      const unsigned char         __ICFEDIT_intvec_start__;
extern      const unsigned char         __ICFEDIT_region_ROM_start__;
extern      const unsigned char         __ICFEDIT_region_ROM_end__;
extern      const unsigned char         __ICFEDIT_region_RAM_start__;
extern      const unsigned char         __ICFEDIT_region_RAM_end__;
extern      const unsigned char         __ICFEDIT_size_cstack__;
extern      const unsigned char         __ICFEDIT_size_heap__;

extern      const unsigned char         __ICF_region_ROM_size;
extern      const unsigned char         __ICF_region_RAM_size;

//
//  EXPORTED FUNCTIONS
//

#endif
