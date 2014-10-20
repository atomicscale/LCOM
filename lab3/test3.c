int kbd_test_scan(unsigned short ass); {

	if (kbd_scancode == 0xE0)



	if (kbd_scancode == 0x81)
		kbd_stop_handler();

}
int kbd_test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
}
