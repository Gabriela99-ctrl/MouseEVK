#include "main.h"
#include "flechas.h"
static volatile bool main_b_mouse_enable = false;
unsigned int x0 = 0, y0 = 0, x = 0, y = 0, bt0 = 0, bt1 = 0, bt0a = 0, bt1a, bol = 10, del = 500, modo = 0, sel = 0, st = 0, num = 0, sens =
0, fin = 0;
U8 temp;
int dato_FAT;
// Variables para manejo de fat
unsigned int Filessize[10];
static char str_buff[MAX_FILE_PATH_LENGTH];
static char filenames[4][MAX_FILE_PATH_LENGTH];
static bool first_ls;
int hei= 10 ;
int wid = 96 ;
int posY=170;
int posX=40;
int diff=10;
int hab_mod_lect;
int hab_mod_norm;
avr32_pwm_channel_t pwm_channel6 = {
.cdty = 0,
.cprd = 100
};
__attribute__ ((__interrupt__))
void touchSensor() {
if (gpio_get_pin_value(54)) { // UP
funcionUP();
}
if (gpio_get_pin_value(55)) { // DOWN
funcionDOWN();
}
if (gpio_get_pin_value(56)) { // RIGHT
funcionRIGHT();
}
if (gpio_get_pin_value(57)) { // LEFT
funcionLEFT();
}
if (gpio_get_pin_value(58)) { // CENTER
funcionCENTER();
}
eic_clear_interrupt_line(&AVR32_EIC, 1);
}
static void tft_bl_init(void) {
pwm_opt_t opt = {
.diva = 0,
.divb = 0,
.prea = 0,
.preb = 0
};
pwm_init(&opt);
pwm_channel6.CMR.calg = PWM_MODE_LEFT_ALIGNED;
pwm_channel6.CMR.cpol = PWM_POLARITY_HIGH; //PWM_POLARITY_LOW;//PWM_POLARITY_HIGH;
pwm_channel6.CMR.cpd = PWM_UPDATE_DUTY;
pwm_channel6.CMR.cpre = AVR32_PWM_CMR_CPRE_MCK_DIV_2;
pwm_channel_init(6, &pwm_channel6);
pwm_start_channels(AVR32_PWM_ENA_CHID6_MASK);
while(pwm_channel6.cdty < pwm_channel6.cprd) {
pwm_channel6.cdty++;
pwm_channel6.cupd = pwm_channel6.cdty;

//pwm_channel6.cdty--;
pwm_async_update_channel(AVR32_PWM_ENA_CHID6, &pwm_channel6);
delay_ms(10);
}
}
int main(void) {
lcd_init();
adc_init();
int_init();
sysclk_init();
board_init();
udc_start();
ui_init();
pantallaInicio();
pantallaModo();
pantallaMouse();
glove_init();
xTaskCreate(vMouse, (const signed portCHAR *)"Mouse App", ( unsigned portSHORT ) 1024, NULL, 1, NULL);
vTaskStartScheduler();
do {
} while (1);
}
void pantallaModo() {
et024006_DrawFilledRect(0,0, ET024006_WIDTH, ET024006_HEIGHT, BLACK);
et024006_DrawHorizLine(0,0,ET024006_WIDTH,WHITE);
et024006_DrawHorizLine(0,30,ET024006_WIDTH,WHITE);
et024006_DrawVertLine(0,0,30,WHITE);
et024006_DrawVertLine(319,0,30,WHITE);
et024006_PrintString("Proyecto Final Sistemas Embebidos", (const unsigned char *)&FONT8x16, 20, 10, WHITE, -1);
et024006_PrintString("GAB - ARR",(const unsigned char *)&FONT8x16,240,220,WHITE,-1);
et024006_PrintString("Selecciona un Modo",(const unsigned char *)&FONT8x16,60,60,WHITE,-1);
fin = 0;
st = 1;
sel = 0;
while(fin == 0) {
et024006_PrintString("Lectura",(const unsigned char *)&FONT8x16,80,100,WHITE,-1);
et024006_PrintString("Dibujo",(const unsigned char *)&FONT8x16,80,130,WHITE,-1);
et024006_PrintString("3D",(const unsigned char *)&FONT8x16,80,160,WHITE,-1);
et024006_PrintString("Normal",(const unsigned char *)&FONT8x16,80,190,WHITE,-1);
et024006_DrawFilledRect(50,100, 14,14, BLACK);
et024006_DrawFilledRect(50,130, 14,14, BLACK);
et024006_DrawFilledRect(50,160, 14,14, BLACK);
et024006_DrawFilledRect(50,190, 14,14, BLACK);
switch(sel) {
case 0:
et024006_DrawFilledRect(50,100, 14,14, WHITE);
break;
case 1:
et024006_DrawFilledRect(50,130, 14,14, WHITE);
break;
case 2:
et024006_DrawFilledRect(50,160, 14,14, WHITE);
break;
case 3:
et024006_DrawFilledRect(50,190, 14,14, WHITE);
break;
default:
break;
}
}
}
void vMouse(void *pvParameters) {
do {
ch_x();

ch_y();
ch_bt0();
ch_bt1();
flecha(x,y);
vTaskDelay(sens);
} while(1);
}
void flecha(int x, int y){
if((y>550)){
et024006_DrawFilledRect(170,50,140,150,WHITE);
et024006_PutPixmap(abajo,60,0,0,210,85,60, 76);
}else if((y<480)){
et024006_DrawFilledRect(170,50,140,150,WHITE);
et024006_PutPixmap(arriba,60,0,0,210,85,60, 76);
}else if(x>530){
et024006_DrawFilledRect(170,50,140,150,WHITE);
et024006_PutPixmap(izq,87,0,0,210,85,87, 60);
}else if(x<500){
et024006_DrawFilledRect(170,50,140,150,WHITE);
et024006_PutPixmap(der,87,0,0,210,85,87, 60);
} else {
et024006_DrawFilledRect(170,50,140,150,WHITE);
}
}
void lcd_init() {
pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
et024006_Init(FOSC0,FOSC0);
tft_bl_init();
et024006_DrawFilledRect(0,0, ET024006_WIDTH, ET024006_HEIGHT, BLACK);
}
void pantallaInicio() {
et024006_DrawFilledRect(0,0, ET024006_WIDTH, ET024006_HEIGHT, BLACK);
et024006_DrawHorizLine(0,0,ET024006_WIDTH,WHITE);
et024006_DrawHorizLine(0,30,ET024006_WIDTH,WHITE);
et024006_DrawVertLine(0,0,30,WHITE);
et024006_DrawVertLine(319,0,30,WHITE);
et024006_PrintString("Proyecto Final Sistemas Embebidos", (const unsigned char *)&FONT8x16, 20, 10, WHITE, -1);
et024006_PrintString("GAB - ARR",(const unsigned char *)&FONT8x16,240,220,WHITE,-1);
et024006_PrintString("Selecciona tu dispositivo",(const unsigned char *)&FONT8x16,60,80,WHITE,-1);
fin = 0;
st = 0;
while(fin == 0) {
et024006_PrintString("Guante",(const unsigned char *)&FONT8x16,80,120,WHITE,-1);
et024006_PrintString("Joystick",(const unsigned char *)&FONT8x16,80,160,WHITE,-1);
}
}
void pantallaMouse() {
et024006_DrawFilledRect(0,0, ET024006_WIDTH, ET024006_HEIGHT, BLACK);
et024006_DrawHorizLine(0,0,ET024006_WIDTH,WHITE);
et024006_DrawHorizLine(0,30,ET024006_WIDTH,WHITE);
et024006_DrawVertLine(0,0,30,WHITE);
et024006_DrawVertLine(319,0,30,WHITE);
et024006_PrintString("Proyecto Final Sistemas Embebidos", (const unsigned char *)&FONT8x16, 20, 10, WHITE, -1);
et024006_PrintString("GAB - ARR",(const unsigned char *)&FONT8x16,240,220,WHITE,-1);
et024006_PrintString("Valores en x",(const unsigned char *)&FONT8x16,10,50,WHITE,-1);
et024006_PrintString("Valores en y",(const unsigned char *)&FONT8x16,10,100,WHITE,-1);
et024006_PrintString("x0 = ",(const unsigned char *)&FONT8x16,10,70,WHITE,-1);
et024006_PrintString("x = ",(const unsigned char *)&FONT8x16,90,70,WHITE,-1);
et024006_PrintString("y0 = ",(const unsigned char *)&FONT8x16,10,120,WHITE,-1);
et024006_PrintString("y = ",(const unsigned char *)&FONT8x16,90,120,WHITE,-1);
et024006_PrintString("Sensibilidad",(const unsigned char *)&FONT8x16,10,150,WHITE,-1);
}
void adc_init() {
const gpio_map_t ADC_GPIO_MAP = {

{23, 0},
};
gpio_enable_module(ADC_GPIO_MAP, sizeof(ADC_GPIO_MAP) / sizeof(ADC_GPIO_MAP[0]));
AVR32_ADC.mr |= 0x1 << AVR32_ADC_MR_PRESCAL_OFFSET;
adc_configure(&AVR32_ADC);
adc_enable(&AVR32_ADC, 2);
adc_enable(&AVR32_ADC, 0); ///<-
}
void glove_init() {
et024006_DrawFilledRect(50,71, 30, 12, BLACK);
et024006_DrawFilledRect(50,121, 30, 12, BLACK);
char dato1, dato2;
gpio_clr_gpio_pin(AVR32_PIN_PB30);
gpio_clr_gpio_pin(AVR32_PIN_PB31);
delay_ms(100);
adc_start(&AVR32_ADC);
x0 = adc_get_value(&AVR32_ADC,2);
gpio_set_gpio_pin(AVR32_PIN_PB30);
gpio_clr_gpio_pin(AVR32_PIN_PB31);
delay_ms(100);
adc_start(&AVR32_ADC);
y0 = adc_get_value(&AVR32_ADC,2);
gpio_clr_gpio_pin(AVR32_PIN_PB30);
gpio_set_gpio_pin(AVR32_PIN_PB31);
delay_ms(100);
adc_start(&AVR32_ADC);
bt0 = adc_get_value(&AVR32_ADC,2);
gpio_set_gpio_pin(AVR32_PIN_PB30);
gpio_set_gpio_pin(AVR32_PIN_PB31);
delay_ms(100);
adc_start(&AVR32_ADC);
bt1 = adc_get_value(&AVR32_ADC,2);
sprintf(dato1,"%d",x0);
et024006_PrintString(dato1,(const unsigned char *)&FONT8x16,50,70,WHITE,-1);
sprintf(dato2,"%d",y0);
et024006_PrintString(dato2,(const unsigned char *)&FONT8x16,50,120,WHITE,-1);
}
void int_init() {
Disable_global_interrupt();
INTC_init_interrupts();
INTC_register_interrupt(&touchSensor, 33, 0);
eic_options_t eic_options;
eic_options.eic_mode = EIC_MODE_EDGE_TRIGGERED;
eic_options.eic_edge = EIC_EDGE_RISING_EDGE;
eic_options.eic_async = EIC_SYNCH_MODE;
eic_options.eic_line = 1;
eic_init(&AVR32_EIC, &eic_options, 1);
eic_enable_line(&AVR32_EIC, 1);
eic_enable_interrupt_line(&AVR32_EIC, 1);
Enable_global_interrupt();
gpio_enable_module_pin(22,1);
}
void ch_bt0() {
et024006_DrawFilledRect(40,170,50,12,BLACK);
char datobt;
gpio_clr_gpio_pin(AVR32_PIN_PB30);
gpio_set_gpio_pin(AVR32_PIN_PB31);
delay_ms(100);
adc_start(&AVR32_ADC);
bt0a = adc_get_value(&AVR32_ADC,2);
sprintf(datobt,"%d",bt0a);
if(bt0a < 400) {
et024006_PrintString(datobt,(const unsigned char *)&FONT8x16,40,170,GREEN,-1);
udi_hid_mouse_btnleft(1);
} else {
et024006_PrintString(datobt,(const unsigned char *)&FONT8x16,40,170,WHITE,-1);
udi_hid_mouse_btnleft(0);
}

}
void ch_bt1() {
et024006_DrawFilledRect(100,170,50,12,BLACK);
char datobt;
gpio_set_gpio_pin(AVR32_PIN_PB30);
gpio_set_gpio_pin(AVR32_PIN_PB31);
delay_ms(100);
adc_start(&AVR32_ADC);
bt1a = adc_get_value(&AVR32_ADC,2);
sprintf(datobt,"%d",bt1a);
if(bt1a < 400) {
et024006_PrintString(datobt,(const unsigned char *)&FONT8x16,100,170,GREEN,-1);
udi_hid_mouse_btnright(1);
} else {
et024006_PrintString(datobt,(const unsigned char *)&FONT8x16,100,170,WHITE,-1);
udi_hid_mouse_btnright(0);
}
}
void ch_x() {
et024006_DrawFilledRect(120,71, 30, 12,BLACK);
char datox;
gpio_clr_gpio_pin(AVR32_PIN_PB30);
gpio_clr_gpio_pin(AVR32_PIN_PB31);
adc_start(&AVR32_ADC);
x = adc_get_value(&AVR32_ADC,0);
sprintf(datox,"%d",x);
if(x > x0 + 10) {
et024006_PrintString(datox,(const unsigned char *)&FONT8x16,120,70,GREEN,-1);
udi_hid_mouse_moveX(-bol);
} else if(x < x0 - 10) {
et024006_PrintString(datox,(const unsigned char *)&FONT8x16,120,70,BLUE,-1);
udi_hid_mouse_moveX(bol);
} else {
et024006_PrintString(datox,(const unsigned char *)&FONT8x16,120,70,WHITE,-1);
}
}
void ch_y() {
et024006_DrawFilledRect(120,120, 30, 12,BLACK);
char datoy;
gpio_set_gpio_pin(AVR32_PIN_PB30);
gpio_clr_gpio_pin(AVR32_PIN_PB31);
adc_start(&AVR32_ADC);
y = adc_get_value(&AVR32_ADC,2);
sprintf(datoy,"%d",y);
if(y > y0 + 10) {
et024006_PrintString(datoy,(const unsigned char *)&FONT8x16,120,120,GREEN,-1);
udi_hid_mouse_moveY(bol);
} else if(y < y0 - 10) {
et024006_PrintString(datoy,(const unsigned char *)&FONT8x16,120,120,BLUE,-1);
udi_hid_mouse_moveY(-bol);
} else {
et024006_PrintString(datoy,(const unsigned char *)&FONT8x16,120,120,WHITE,-1);
}
if(x > 533) {
sprintf(datox,"%d",x);
et024006_PrintString(datox,(const unsigned char *)&FONT8x16,120,70,GREEN,-1);
} else if(x < 500) {
sprintf(datox,"%d",x);
et024006_PrintString(datox,(const unsigned char *)&FONT8x16,120,70,BLUE,-1);
}else {
sprintf(datox,"%d",x);
et024006_PrintString(datox,(const unsigned char *)&FONT8x16,120,70,WHITE,-1);
}
if(y<480) {
sprintf(datoy,"%d",y);
et024006_DrawFilledRect(120,120,24,16,BLACK);
et024006_PrintString(datoy,(const unsigned char *)&FONT8x16,120,120,BLUE,-1);

} else if(y>550) {
sprintf(datoy,"%d",y);
et024006_DrawFilledRect(120,120,24,16,BLACK);
et024006_PrintString(datoy,(const unsigned char *)&FONT8x16,120,120,GREEN,-1);
}else{
sprintf(datoy,"%d",y);
et024006_DrawFilledRect(120,120,24,16,BLACK);
et024006_PrintString(datoy,(const unsigned char *)&FONT8x16,120,120,WHITE,-1);
}
}
void main_suspend_action(void) {
ui_powerdown();
}
void main_resume_action(void) {
ui_wakeup();
}
void main_sof_action(void) {
if (!main_b_mouse_enable)
return;
ui_process(udd_get_frame_number());
}
void main_remotewakeup_enable(void) {
ui_wakeup_enable();
}
void main_remotewakeup_disable(void) {
ui_wakeup_disable();
}
bool main_mouse_enable(void) {
main_b_mouse_enable = true;
return true;
}
void main_mouse_disable(void) {
main_b_mouse_enable = false;
}
void funcionUP() {
if(st == 0) {
sel = 1;
et024006_DrawFilledRect(50,120, 14,14, WHITE);
et024006_DrawFilledRect(50,160, 14,14, BLACK);
}
}
void funcionDOWN() {
if(st == 0) {
sel = 0;
et024006_DrawFilledRect(50,160, 14,14, WHITE);
et024006_DrawFilledRect(50,120, 14,14, BLACK);
}
}
void funcionLEFT() {
if(st == 1) {
if(sel > 0) {
sel = sel - 1;
}
}
}
void funcionRIGHT() {
if(st == 1) {
if(sel < 3) {

sel = sel + 1;
}
}
}
void funcionCENTER() {
if(st == 0) {
fin = 1;
} else if(st == 1) {
fin = 1;
switch(sel) {
case 0:
modo_lectura();
break;
case 1:
modo_dibujar();
break;
case 2:
modo_3D();
break;
case 3:
modo_normal();
break;
default:
break;
}
} else {
glove_init();
}
}
void modo_lectura(){
num = 2 ; // Abre el archivo 1 de la lista en SDCARD
leerFAT(num);
sens = dato_FAT;
modo =0; // para que no caiga en un loop infinito
hab_mod_lect=1;
}
void modo_dibujar(){
num=3;
leerFAT(num);
sens = dato_FAT-48;
modo =0; // para que no caiga en un loop infinito
}
void modo_3D(){
num=4;
leerFAT(num);
sens = dato_FAT-48;
/*print_dbg_ulong(sens) ;*/
modo =0; // para que no caiga en un loop infinito
}
void modo_normal(){
hab_mod_norm = 1;
modo =0;
gpio_clr_gpio_pin(LED3_GPIO);
}
void InicializarFiles (void)
{
/*Inicialiazamos los archivos de la SD manejada por SPI, para lograr que funcionara tuvo que habilitarla mediante el
conf_access.h y conf_explorer.h, debido a que por default viene habilitada la memoria incluida en la EVK*/
first_ls = true;
if (nav_drive_get() >= nav_drive_nb() || first_ls)
{
first_ls = false;

// Reset navigators .
nav_reset();
// Use the last drive available as default.
nav_drive_set(nav_drive_nb() - 1);
// Mount it.
nav_partition_mount();
}
nav_dir_name((FS_STRING)str_buff, MAX_FILE_PATH_LENGTH);
// Try to sort items by folders
if (!nav_filelist_first(FS_DIR))
{
// Sort items by files
nav_filelist_first(FS_FILE);
}
nav_filelist_reset();
}
static void sd_mmc_resources_init(void)
{
// GPIO pins used for SD/MMC interface
static const gpio_map_t SD_MMC_SPI_GPIO_MAP =
{
{SD_MMC_SPI_SCK_PIN, SD_MMC_SPI_SCK_FUNCTION }, // SPI Clock.
{SD_MMC_SPI_MISO_PIN, SD_MMC_SPI_MISO_FUNCTION}, // MISO.
{SD_MMC_SPI_MOSI_PIN, SD_MMC_SPI_MOSI_FUNCTION}, // MOSI.
{SD_MMC_SPI_NPCS_PIN, SD_MMC_SPI_NPCS_FUNCTION} // Chip Select NPCS.
};
// SPI options.
spi_options_t spiOptions =
{
.reg = 1,
.baudrate = FOSC0,
.bits = 8, // Defined in conf_sd_mmc_spi.h.
.spck_delay = 0,
.trans_delay = 0,
.stay_act = 1,
.spi_mode = 0,
.modfdis = 1
};
// Assign I/Os to SPI.
gpio_enable_module(SD_MMC_SPI_GPIO_MAP,
sizeof(SD_MMC_SPI_GPIO_MAP) / sizeof(SD_MMC_SPI_GPIO_MAP[0]));
// Initialize as master.
spi_initMaster(SD_MMC_SPI, &spiOptions);
// Set SPI selection mode: variable_ps, pcs_decode, delay.
spi_selectionMode(SD_MMC_SPI, 0, 0, 0);
// Enable SPI module.
spi_enable(SD_MMC_SPI);
// Initialize SD/MMC driver with SPI clock (PBA).
sd_mmc_spi_init(spiOptions, FOSC0);
}
void leerFAT(int num){
init_dbg_rs232(FOSC0); // Inicializamos el protocolo serial para Debug
print_dbg("\r\n Probado salida de serial");// @ 57600
sd_mmc_resources_init();
// Inicializamod la SD card
print_dbg("\r\n Revisando SD");
// @ 57600
while (!sd_mmc_spi_mem_check()); // Esperamos a que este insertada una SD card
print_dbg("\r\n SD detectada");
// @ 57600
print_dbg("\r\n Leyendo Archivos en tarjeta..");// @ 57600

temp= get_nb_lun();
//Leer LUN Actual
print_dbg("\r\n LUN actual\t");
print_dbg_char_hex(temp);
print_dbg("\r\n Leyendo tamaño de SD \t")
sd_mmc_spi_get_capacity(); // revisa el tamaño de la SD // el valor esta almacenado en la variabe capacity de
//print_dbg("\r\n Tamaño MB \t");
print_dbg_ulong(capacity >> 20);
print_dbg(" MBytes");
//Hasta aquí todo bien si la capacidad coincide con la información de la SD
print_dbg("\r\n Revisar la unidad\t");
temp=nav_drive_getname();
InicializarFiles();
nav_filelist_reset();
temp=0;
print_dbg("\r\Buscando Archivos: ");
//nav_filelist_reset();
while (nav_filelist_set(0, FS_FIND_NEXT)) // While an item can be found
{
temp++;
}
print_dbg_char_hex(temp);
print_dbg("\nConfiguracion de sensibilidad encontrada : ");
// Imprime y guarda en variable el dato leido de SDCARD
nav_filelist_goto(num); //Imprimir antes el archivo
file_open(FOPEN_MODE_R);
while (!file_eof()) //Hasta encontrar el fin del archivo
{
dato_FAT = file_getc();
print_dbg_char(dato_FAT); // Display next char from file.
}
file_close(); // Close the file.
nav_exit(); // Cerramos sistemas de archivos
}