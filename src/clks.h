/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _CLKS_H    /* Guard against multiple inclusion */
#define _CLKS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        CLKS_STATE_INIT = 0,
        CLKS_STATE_EVENT,
        CLKS_STATE_IDLE
    } CLK_STATES;
 
    typedef struct
    {
        CLK_STATES clkstate;
        int idlecnt;
        unsigned int cpucycles;
        unsigned int last_CP0;
        int idlecntpersec;
    } CLKDATA;
    
    typedef struct
    {
        int ticks;
        int timeout;
        void (*action)();
    } SUBCLK;
    
    void CLKS_Initialize(void);
    void CLKS_SM(void);
    void clk1000(void);
    void clk100(void);


#ifdef __cplusplus
}
#endif

#endif /* _CLKS_H */

/* *****************************************************************************
 End of File
 */
