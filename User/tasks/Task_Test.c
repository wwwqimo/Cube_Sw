#include  <includes.h>

uint8_t TestBuf[116];
char gps_flg = 0;
char semple_flg = 0;

char rtc_info_switch = 0;
char flash_read_switch = 0;
char flash_write_switch = 0;
char flash_eraser_switch = 0;

char gpsa_sw = 2;
char gpsb_sw = 2;
char mtq_sw = 2;
char wheela_sw = 2;
char wheelb_sw = 2;
char fipex_sw = 2;
char s0_sw = 2;
char s1_sw = 2;

char pl_on = 2;

void TEST_TASK(void *p_arg)
{	
	(void)p_arg;
	
	while(1)
	{
		GndTsRxHandle();
		BSP_OS_TimeDlyMs(1);
		
		
		#if 1
		if(gpsa_sw != 2)
		{
			if(gpsa_sw == 1)
			{
				gpsa_sw = 2;
				SW_GPSA_ENABLE;
			}
			if(gpsa_sw == 0)
			{
				gpsa_sw = 2;		
				SW_GPSA_DISABLE;				
			}
		}

		if(gpsb_sw != 2)
		{
			if(gpsb_sw == 1)
			{
				gpsb_sw = 2;
				SW_GPSB_ENABLE;
			}
			if(gpsb_sw == 0)
			{
				gpsb_sw = 2;		
				SW_GPSB_DISABLE;				
			}
		}
		
		if(gpsa_sw != 2)
		{
			if(gpsa_sw == 1)
			{
				gpsa_sw = 2;
				SW_GPSA_ENABLE;
			}
			if(gpsa_sw == 0)
			{
				gpsa_sw = 2;		
				SW_GPSA_DISABLE;				
			}
		}
		
		if(mtq_sw != 2)
		{
			if(mtq_sw == 1)
			{
				mtq_sw = 2;
				SW_MTQ_ENABLE;
			}
			if(mtq_sw == 0)
			{
				mtq_sw = 2;		
				SW_MTQ_DISABLE;			
				
			}
		}		
		
		if(wheela_sw != 2)
		{
			if(wheela_sw == 1)
			{
				wheela_sw = 2;
				SW_WHEELA_ENABLE;
			}
			if(wheela_sw == 0)
			{
				wheela_sw = 2;		
				SW_WHEELA_DISABLE;				
			}
		}		
		
		if(wheelb_sw != 2)
		{
			if(wheelb_sw == 1)
			{
				wheelb_sw = 2;
				SW_WHEELB_ENABLE;
			}
			if(wheelb_sw == 0)
			{
				wheelb_sw = 2;		
				SW_WHEELB_DISABLE;				
			}
		}	
		if(fipex_sw != 2)
		{
			if(fipex_sw == 1)
			{
				fipex_sw = 2;
				bsp_FipexPowerOn();
			}
			if(fipex_sw == 0)
			{
				fipex_sw = 2;
				bsp_FipexPowerOff();
			}
		}
		
		if(s1_sw != 2)
		{
			if(s1_sw == 1)
			{
				s1_sw = 2;
				SW_S1_ENABLE;
			}
			if(s1_sw == 0)
			{
				s1_sw = 2;
				SW_S1_DISABLE;
			}
		}
		
		if(s0_sw != 2)
		{
			if(s0_sw == 1)
			{
				s0_sw = 2;
				SW_S0_ENABLE;
			}
			if(s0_sw == 0)
			{
				s0_sw = 2;
				SW_S0_DISABLE;
			}
		}
		
		if(pl_on != 2)
		{
			if(pl_on == 1)
			{
				pl_on = 2;
				FipexScriptStart();
			}
			if(pl_on == 0)
			{
				pl_on = 2;
				FipexScriptStop();
			}
		}
		
	#endif
		
		
	}
		
	
}
