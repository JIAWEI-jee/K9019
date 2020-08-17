#include "timer.h"
#include "flash.h"
#include "uart.h"
#include "lcd_display.h"

//----------------time---------------
u8 time_cnt = 0;
u16 time_sec = 0;
u16 cnt_heat_time = 0;
u8 over_rang_time_std = 0,On_stay = 0;
u8 Gap_protect_std = 0 ;
void set_time_sec_val ( u16 sec )
{
	time_sec = sec;
	time_cnt = 0;
	gm_printf ( "set time second:%d\r\n",sec );
}


void set_time_sec ( void )
{
	time_cnt = 0;
	if ( flash_info.timer == TIMER_ON )
	{
		time_sec = 0;
		cnt_heat_time = 0;
		On_stay = 2;
	}
	else if ( flash_info.timer == TIMER_0_5H )
	{
		time_sec = 30*60;
		cnt_heat_time = 0;

	}
	else if ( flash_info.timer == TIMER_1H )
	{
		time_sec = 60*60;
		cnt_heat_time = 0;

	}
	else if ( flash_info.timer == TIMER_2H )
	{
		time_sec = 2*60*60;
		cnt_heat_time = 0;
		Gap_protect_std = 2;
	}
	else if ( flash_info.timer == TIMER_4H )
	{
		time_sec = 4*60*60;
		cnt_heat_time = 0;
		Gap_protect_std = 2;
	}
	else if ( flash_info.timer == TIMER_8H )
	{
		time_sec = 8*60*60;
		cnt_heat_time = 0;
		Gap_protect_std = 2;
	}
	gm_printf ( "time second:%d \r\n",time_sec );
}

void time0_init ( void )
{
	/**********************************TIM0���ó�ʼ��**************************************/
	TCON1 = 0x00;						//Tx0��ʱ��ʱ��ΪFosc/12
	TMOD = 0x00;						//16λ��װ�ض�ʱ��/������
	//��ʱ10ms
	//���Ƴ�ֵ 	= 65536 - ((10/1000) / (1/(Fosc / Timer��Ƶϵ��)))
	//		   	= 65536 - ((10/1000) / (1/(16000000 / 12)))
	//			= 65536 - 13333
	//			= 0xCBEC
	// TH0 = 0xCB;
	// TL0 = 0xEB;							//T0��ʱʱ��10ms  16MHZ
	TH0 = 0xF2;
	TL0 = 0xFB;							//T0��ʱʱ��10ms  4MHZ
	TF0 = 0;//����жϱ�־
	ET0 = 1;//��T0�ж�
	TR0 = 1;//ʹ��T0
	EA = 1;								//�����ж�
}



//10ms
void TIMER0_Rpt ( void ) interrupt TIMER0_VECTOR
{
	if ( get_device_state() == ON ) //flash_info.timer != TIMER_ON &&
	{
		time_cnt++;
		if ( time_cnt >= 1 )
		{

			if ( On_stay == 2 )  // stay on ģʽ�½��б���
			{
				cnt_heat_time++;
				if ( cnt_heat_time > overrang_time )
				{
					On_stay = 0;
					over_rang_time_std = 1;
					gm_printf ( "On_stay_overrang \r\n" );
				}

			}
			if ( time_sec > 0 )
			{

				time_sec--;
				if ( Gap_protect_std == 2 )
				{
					cnt_heat_time++;

					if ( cnt_heat_time > overrang_time )
					{
						over_rang_time_std = 1;	Gap_protect_std = 0;
						gm_printf ( "protect \r\n" );
					}
				}
			}
			else
			{
				if ( flash_info.timer != TIMER_ON )
				{
					set_device_state ( OFF );
					time_sec = 0;
					cnt_heat_time = 0;
					Gap_protect_std = 0;
					On_stay = 0;
					ht1621_all_clear(); //��ʱʱ�䵽
					gm_printf ( "time off \r\n" );
				}
			}
			//			gm_printf("time_sec=%d \r\n",time_sec);
			time_cnt = 0;
		}
	}
}