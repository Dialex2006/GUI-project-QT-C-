#include "wirpi.h"
#include <wiringPi.h>
#include "wiringPiI2C.h"
#include <QtDebug>



WirPi::WirPi()
{

       // setup
       I2C_addr= wiringPiI2CSetup(0x48);

       qDebug() << "Read I2C addr";

       wiringPiI2CWriteReg16(I2C_addr,1, 0xc3e3);

}


int WirPi::Read_sensor0(void)
{
    return Read_ad(0);  // read ad ch=0
}

int WirPi::Read_sensor1(void)
{
    return Read_ad(1);  // read ad ch=1
}



int WirPi::Read_ad(unsigned char ch)
{
    unsigned char byt_h, byt_l;

    if (ch==0) { wiringPiI2CWriteReg16(I2C_addr,1, 0xc3c3);};
    if (ch==1) { wiringPiI2CWriteReg16(I2C_addr,1, 0xc3d3);};
    if (ch==2) { wiringPiI2CWriteReg16(I2C_addr,1, 0xc3e3);};
    if (ch==3) { wiringPiI2CWriteReg16(I2C_addr,1, 0xc3f3);};

       uint16_t res;

       do {
                res= wiringPiI2CReadReg16(I2C_addr,1) & 0x8000;

            } while (res==0);


       unsigned int ad_w =uint(wiringPiI2CReadReg16(I2C_addr,0));

       unsigned int ap, yp;

       yp=ad_w & 0x00ff;
       ap=ad_w & 0xff00;

       ap=ap >> 8;

       unsigned int wd;

  //qDebug() << " reg read ";
  //qDebug() << ad_w << " - " <<yp << " - " << ap;

        byt_h= uchar (yp);
        byt_l= uchar (ap);

       int ker;
       if(byt_h & 0x80)
       {   qDebug() << " Negat";


           wd=256*byt_h+byt_l;
           wd=~ wd;
           wd++;
           wd=0x7FFF & wd;
           ker=-1;
       }

       else
       {
           wd=256*byt_h+byt_l;
           ker=1;
       }

        long l_res = long(ker * int(wd));

         //qDebug() << int(l_res) ;

       return int (l_res) ;

}


