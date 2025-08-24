#include "mbed.h"
BufferedSerial pc(USBTX, USBRX, 115200);
BufferedSerial esp(PA_9, PA_10, 115200);
int16_t pwm[2] = {0, 0};
int LeftY;
int RightY;

int main(){
    CAN can(PA_11, PA_12, (int)1e6); // canを出力するピンを指定
    CANMessage msg; // 変数「msg」の作成
    char buf[64];
    int buf_index = 0;

    while(1){
        if(esp.readable()){
            char c;
            int len = esp.read(&c, 1);
            if (len > 0){
                if (c == ','){
                    if(buf_index > 0){
                        buf[buf_index] = '\0';
                        LeftY = atoi(buf);
                        buf_index = 0;
                    }
                }else if (c == '\n'){
                    if(buf_index > 0){
                        buf[buf_index] = '\0';
                        RightY = atoi(buf);
                        buf_index = 0;
                    }
                }else{
                    if (buf_index < sizeof(buf) - 1){
                        buf[buf_index++] = c;
                    }
                }       
            }
        }
        pwm[0] = LeftY;
        pwm[1] = RightY;
        CANMessage msg(2, (const uint8_t *)pwm, 8); //特に理由がない限りwhile直下
        can.write(msg); //特に理由がない限りwhile直下
    }
}
