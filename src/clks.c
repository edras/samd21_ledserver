
#include "app.h"
#include "clks.h"
#include "peripheral/tc/plib_tc3.h"
#include "peripheral/port/plib_port.h"
#include "system/debug/sys_debug.h"

#define CLKS_DBG_VERBOSE_PRINT(...)         do { _SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, __VA_ARGS__); } while (0)
#define CLKS_DBG_TRACE_PRINT(...)           do { _SYS_DEBUG_PRINT(SYS_ERROR_INFO, __VA_ARGS__); } while (0)
#define CLKS_DBG_INFORM_PRINT(...)          do { _SYS_DEBUG_PRINT(SYS_ERROR_WARNING, __VA_ARGS__); } while (0)
#define CLKS_DBG_ERROR_PRINT(...)           do { _SYS_DEBUG_PRINT(SYS_ERROR_ERROR, __VA_ARGS__); } while (0)


extern APP_DATA appData;
CLKDATA clkData;
SUBCLK clkCfg[] = {
    // ticks, timeout, action routine
    {0, 1000, clk1000},
    {0, 100, clk100},
    {0, 0, NULL}    
};

void TMR_CallbackFN(uint32_t status, uintptr_t context)
{
    clkData.clkstate = CLKS_STATE_EVENT;
}

void CLKS_Initialize(void)
{
    clkData.cpucycles = 0;
    clkData.last_CP0 = 0;
    clkData.idlecnt = 0;
    clkData.idlecntpersec = 0;
    clkData.clkstate = CLKS_STATE_INIT;
}

void CLKS_SM(void)
{
    SUBCLK *p;
    
    switch(clkData.clkstate)
    {
        case CLKS_STATE_INIT:
            TC3_TimerStop();
            TC3_TimerCallbackRegister((TC_TIMER_CALLBACK)TMR_CallbackFN, (uintptr_t)NULL);
            TC3_Timer16bitPeriodSet(50000);
            TC3_TimerStart();
            CLKS_DBG_INFORM_PRINT("The 1 millisecond timer has started");
            clkData.clkstate = CLKS_STATE_IDLE;
            break;
            
        case CLKS_STATE_EVENT:
            appData.msec_cnt++;
            for (p = clkCfg; p->timeout > 0; p++) {
                p->ticks++;
                if(p->ticks >= p->timeout)
                {
                    p->action();
                    p->ticks = 0;
                }
            }
            clkData.clkstate = CLKS_STATE_IDLE;
            break;
            
        case CLKS_STATE_IDLE:
            clkData.idlecnt++;
            break;
    }
}

void clk1000(void)
{
    static int substate;
    clkData.cpucycles = 0;
    clkData.last_CP0 = 0;
    clkData.idlecntpersec = clkData.idlecnt;
    clkData.idlecnt = 0;
    
    substate++;
    if(substate == 1) LED_BLUE_Toggle();
    else if (substate == 2) LED_GREEN_Toggle();
    else if (substate == 3) LED_YELLOW_Toggle();
    if (substate > 2) substate = 0;
    
    if(SW0_Get() == 0)
    {
        LED_RED_Toggle();
    }
}

void clk100(void)
{
    
}