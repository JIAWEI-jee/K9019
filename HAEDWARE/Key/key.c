#include "key.h"
#include "delay.h"
#include "flash.h"
#include "timer.h"
u8 calibration_std = 0;
#define KEY_PRESS_TIME 700

void key_init(void)
{
	P3M5 = 0x61;                        //P35¨¦¨¨???a¡¤?¨º??¨¹¨¬?¨ºy¡Á?¡ä?¨¦?¨¤-¨º?¨¨?
	P2M5 = 0x61;                        //P25¨¦¨¨???a¡¤?¨º??¨¹¨¬?¨ºy¡Á?¡ä?¨¦?¨¤-¨º?¨¨?
	P2M6 = 0x61;                        //P26¨¦¨¨???a¡¤?¨º??¨¹¨¬?¨ºy¡Á?¡ä?¨¦?¨¤-¨º?¨¨?
	P0M2 = 0x61;                        //P02¨¦¨¨???a¡¤?¨º??¨¹¨¬?¨ºy¡Á?¡ä?¨¦?¨¤-¨º?¨¨?
	P2M7 = 0XC1;                        //¨¦¨¨???a¨ª?¨ª¨¬¨º?3?
	P0M0 = 0XC1;                        //¨¦¨¨???a¨ª?¨ª¨¬¨º?3?
	P2M1 = 0X51;                        //¨¦¨¨????¡¤?¨º??¨¹¨¬?¡ä???¨¤-¨º?¨¨?
}


static unsigned char key_driver(void) 
{     
    static unsigned char key_state = 0;         
    static unsigned int key_time = 0;           
    unsigned char key_press, key_return; 
 
    key_return = N_KEY;                         
 
     key_press = KEY_1; 
    switch (key_state)     
    {       
        case KEY_STATE_0:                       
            if (key_press == 0)                     
            {
                key_time = 0;                   
                key_state = KEY_STATE_1;       
            }        
            break;
 
        case KEY_STATE_1:                       
            if (key_press == 0)                     
            {
                key_time++;                     
                if(key_time>=SINGLE_KEY_TIME)   
                {
                    key_state = KEY_STATE_2;    
                }
				delay_ms(10);
            }         
            else key_state = KEY_STATE_0;       
            break; 
 
        case KEY_STATE_2:                       
            if(key_press == 1)                       
            { 
                 key_return = KEY_1_SHORT;            
                 key_state = KEY_STATE_0;       
            } 
            else
            {
                key_time++;                     
 
                if(key_time >= LONG_KEY_TIME)   
                {
                    key_return = KEY_1_LONG;         
                    key_state = KEY_STATE_3;    
                }
				delay_ms(10);
            }
            break;
 
      case KEY_STATE_3:                         
          if (key_press == 1) 
          {
              key_state = KEY_STATE_0;          
          }         
          break; 
 
        default:                               
            key_state = KEY_STATE_0;
            break;
    }
 
    return key_return;                         

}
static unsigned char Calibration_key_driver ( void )
{
	static unsigned char key_state = 0;
	static unsigned int key_time = 0;
	unsigned char key_press, key_return;

	key_return = N_KEY;

	key_press = KEY_2;
	switch ( key_state )
	{
		case KEY_STATE_0:
			if ( key_press == 0 )
			{
				key_time = 0;
				key_state = KEY_STATE_1;
			}
			break;

		case KEY_STATE_1:
			if ( key_press == 0 )
			{
				key_time++;
				if ( key_time>=SINGLE_KEY_TIME )
				{
					key_state = KEY_STATE_2;
				}
				delay_ms ( 10 );
			}
			else
			{
				key_state = KEY_STATE_0;
			}
			break;

		case KEY_STATE_2:
			if ( key_press == 1 )
			{
				key_return = KEY_1_SHORT;
				key_state = KEY_STATE_0;
			}
			else
			{
				key_time++;

				if ( key_time >=  LONG_KEY_CALI_TIME )
				{
					key_return = KEY_CALI2_LONG;
					key_state = KEY_STATE_3;
				}
				delay_ms ( 10 );
			}
			break;

		case KEY_STATE_3:
			if ( key_press == 1 )
			{
				key_state = KEY_STATE_0;
			}
			break;

		default:
			key_state = KEY_STATE_0;
			break;
	}

	return key_return;

}

u8 key_scan ( void )
{
	static u8 key_up = 1;

	if ( key_driver() == KEY_1_LONG )
	{
		return KEY_1_PRES;
	}
	else if ( key_up && ( KEY_1 == 0 ) )
	{
		delay_ms ( 50 );
		key_up=0;
		return LED_STAY_ON;
	}
if(Calibration_key_driver() ==  KEY_CALI2_LONG)
	{
		if ( ( get_device_state (  )  == OFF ) && ( flash_info.correct_std == 0 ) )
		{
			calibration_std = 1;
			flash_info.correct_std = 1;
			flah_save_data();
		}
		else
		{
			calibration_std = 0;
		}
	}
	else if ( key_up && ( KEY_2 == 0 || KEY_3 == 0 ) && ( calibration_std == 0 ) )
	{
		delay_ms ( 50 );
		key_up=0;
		if ( KEY_2 == 0 )
		{
			return KEY_3_PRES;
		}
		else if ( KEY_3 == 0 )
		{
			return KEY_2_PRES;
		}
	}
	else if ( ( KEY_2 == 1 ) && ( KEY_3 == 1 ) && ( KEY_1 == 1 ) )
	{
		key_up=1;
	}

	return 0;
}
