/*
	UVAXUT 2021
 */

#include <rc.h>

void tplink_leds(int st){
	tplink_led_modes(st);
	led_control(LED_WPS, st);
	led_control(LED_USB, st);
}

void tplink_led_modes(int st){
	led_control(LED_LAN1   , st);
	led_control(LED_LAN2   , st);
	led_control(LED_LAN3   , st);
	led_control(LED_LAN4   , st);
	led_control(LED_WAN    , st);
	led_control(LED_WAN_RED, st);
	led_control(LED_2G     , st);
	led_control(LED_5G     , st);
}

int tplink_night_mode(){
	tplink_led_modes(LED_OFF);
	nvram_set_int("leds_mode", LED_OFF);
	return 0;
}

int tplink_normal_mode(){
	tplink_led_modes(LED_ON);
	nvram_set_int("leds_mode", LED_ON);
	return 0;
}

int LanWanLedCtrl_tplink(void)
{
	int ports[5];
	int i, mask;
	int state = nvram_get_int("wan0_state_t");
	int mode = sw_mode();
	
	/* WAN L1 L2 L3 L4 */
	ports[0]=0; ports[1]=1; ports[2]=2, ports[3]=3; ports[4]=4;

	for (i=0; i<5; i++) {
		mask = 0;
		mask |= 0x0001<<ports[i];
		if (get_phy_status(mask)==0) {/*Disconnect*/
			if (i==0) {
				led_control(LED_WAN    , LED_OFF);
				led_control(LED_WAN_RED, LED_OFF);
			} else {
				if (i == 1) led_control(LED_LAN1, LED_OFF);
				if (i == 2) led_control(LED_LAN2, LED_OFF);
				if (i == 3) led_control(LED_LAN3, LED_OFF);
				if (i == 4) led_control(LED_LAN4, LED_OFF);
			}
		}
		else { /*Connect, keep check speed*/
			if (i==0) {
				if(mode == SW_MODE_ROUTER){
					if(state == WAN_STATE_CONNECTED){
						led_control(LED_WAN_RED, LED_OFF);
						led_control(LED_WAN    , LED_ON);
					}
					else{
						led_control(LED_WAN_RED, LED_ON);
						led_control(LED_WAN    , LED_OFF);
					}
				}
				else{
					led_control(LED_WAN    , LED_OFF);
					led_control(LED_WAN_RED, LED_OFF);
				}
			} 
			else {
				if (i == 1) led_control(LED_LAN1, LED_ON);
				if (i == 2) led_control(LED_LAN2, LED_ON);
				if (i == 3) led_control(LED_LAN3, LED_ON);
				if (i == 4) led_control(LED_LAN4, LED_ON);
			}
		}
	}
return 0;
}

int led_tplink_main(){
	LanWanLedCtrl_tplink();
	return 0;
}