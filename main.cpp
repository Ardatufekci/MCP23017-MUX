#include "mbed.h"
#include "MCP23017.h"

I2C i2c(I2C_SDA,I2C_SCL);
int hata,  devices;
int address;
#define ENABLED 1
#define DISABLED 0 
const int addr7bit = 0x20;
const int addr8bit = 0x40;
MCP23017::MCP23017 io(i2c,MCP23017_DEFAULT_ADDR);
AnalogIn ain1(A0,MBED_CONF_TARGET_DEFAULT_ADC_VREF);
//AnalogIn ain2(A1,MBED_CONF_TARGET_DEFAULT_ADC_VREF);
float olcum1;
float olcum2;
//10k thermistorler için
float A = -0.9098880323e-3;
float B = 2.148732041e-4;
float C = 1.064658112e-7;

//100k thermistorler için
float a1 = 0.8268883559e-3;
float b1 = 2.088318209e-4 ;
float c1 = 0.8051409251e-7; 

float R1 = 1000;
float logR2,R2,T,Tc,Tf;

void port_b()
{
    io.setDirrection(MCP23017_PORTB, 0x00); //PORT B output ayarlanması
    char data[2];
    data[0] = 0x01;
    data[1] = 0x00;
    char dongu[16] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
    i2c.write(0x20<<1,data,2,0);
    data[0] = 0x13; //PORT B için 
    if (ENABLED) {
        for (int i = 0; i<=15; i++) {
            data[1] = dongu[i];
            i2c.write(0x20<<1,data,2,0);
            olcum1 = ain1.read();
            R2 = R1 * (1023.0/olcum1 -1.0);
            logR2 = log(R2);
            T = (1.0/(A+B*logR2+C*logR2*logR2*logR2));
            Tc = T-273.15;
            printf("%d.Sıcaklık = %f \n",i,Tc);
            printf("--------\n");
    //olcum2 = ain2.read();
    }
    }
}
void port_a()
{
    io.setDirrection(MCP23017_PORTA, 0x00); //PORT A output ayarlanması
    char data[2];
    data[0] = 0x01;
    data[1] = 0x00;
    char dongu[16] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
    i2c.write(0x20<<1,data,2,0);
    data[0] = 0x12; //PORT A için 
    if (ENABLED) {
        for (int i = 0; i<=15; i++) {
            data[1] = dongu[i];
            i2c.write(0x20<<1,data,2,0);
            olcum1 = ain1.read();
    //olcum2 = ain2.read();
    }
    }

}

int main()
{   //port_b();
    /*
    char data[2];
    data[0] = 0x01;
    data[1] = 0xfe;
    i2c.start();
    io.init();
    io.setDirrection(1, 0x00);
    i2c.stop();
    while (true) {
        i2c.start();
        //io.write(1, 0x01);
        i2c.write(0x40,data,2);
        
        printf("SA \n");
        HAL_Delay(1000);
    }    
    
    */
    /*
    devices = 0;
    for (address = 1; address < 127; address++) {
    i2c.start();
    hata = i2c.write(address<<1);
    i2c.stop();
    if (hata == 1) {
    printf("I2C device found address 0x%X \n",address);
    devices++; 
    }
    if (devices == 0) {
    printf("no devices found\n");
    }
    else {
    printf("done /n");
    wait_us(2000000);
    }
    }
    */
    
    io.setDirrection(MCP23017_PORTB, 0x00);
    char data[2];
    data[0] = 0x01;
    data[1] = 0x00;
    char dongu[8] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
    i2c.write(0x20<<1,data,2,0);
    data[0] = 0x13;
    while (true) {
        for (int i = 0; i<=8; i++) {
            data[1] = dongu[i]; 
            i2c.write(0x20<<1,data,2,0);
            printf("%#X \n",dongu[i]);
            HAL_Delay(1000);
        }
    }

}
