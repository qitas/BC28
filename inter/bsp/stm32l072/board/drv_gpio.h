/*
 * File      : gpio.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2016-08-29     Aubr.Cool      the first version
 */
#include <rthw.h>
#include <rtdevice.h>
#include <board.h>

#ifdef RT_USING_PIN

#include "stm32l0xx.h"

#define __RCC_GPIO_CLK_ENABLE(BIT)   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->IOPENR, BIT);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOCEN);\
                                        UNUSED(tmpreg); \
                                      } while(0)
#define __RCC_GPIO_CLK_DISABLE(BIT)  CLEAR_BIT(RCC->IOPENR, BIT)

#define __STM32_PIN(index, gpio, gpio_index) {index, GPIO##gpio##_CLK_ENABLE, GPIO##gpio, GPIO_PIN_##gpio_index}

#define __STM32_PIN_DEFAULT {-1, 0, 0}

#define DRV_POWER_5V_PORT    GPIOB
#define DRV_POWER_5V_PIN     GPIO_PIN_13
#define DRV_POWER_24_PORT     GPIOB
#define DRV_POWER_24_PIN      GPIO_PIN_12


//使用中
//flash
#define PIN_CS_W      32    //W25Q8 CS片选
#define PIN_MISO_W    33    //W25Q8 MISO
#define PIN_WP_W      34    //W25Q8 WP
#define PIN_MOSI_W    35    //W25Q8 MOSI
#define PIN_POW_FLASH 38    //W25Q8 电源
#define PIN_HOLD_W    37    //W25Q8 读写保护 
#define PIN_SCK_W     36    //W25Q8 SCK

//水压
#define PIN_POW_WT    3     //满水监测电源
#define PIN_WT1       4     //满水1
#define PIN_WT2       5     //满水2
#define PIN_ADC_IN1   16    //4~20mA输入1
#define PIN_ADC_IN2   17    //4~20mA输入2

//硫化氢
#define PIN_POW_GAS   10    //硫化氢电源
#define PIN_GAS_IN    11    //硫化氢输入
#define PIN_AD_GAS    15    //硫化氢ad

//噪声
#define PIN_TX2_NOS   25    //串口2 噪声发送
#define PIN_RX2_NOS   26    //串口2 噪声接收
#define PIN_POW_NOS   29    //噪声电源

//电源电压检测
#define PIN_BAT_UP    30    //电源电压监测AD口
#define PIN_BAT_DO    31    //电源电压地

//LED
#define PIN_LED1      98    //LED1
#define PIN_LED2      97    //LED2

//温湿度
#define PIN_POW_TH    96    //温湿度电源
#define PIN_TH_SDA    95    //温湿度 I2C SDA
#define PIN_TH_SCL    93    //温湿度 I2C SCL

//GPS
#define PIN_POW_GPS   91    //GPS电源
#define PIN_TX5_GPS   90    //串口5 GPS发送
#define PIN_RX5_GPS   89    //串口5 GPS接收
#define PIN_RST_GPS   88    //GPS复位
#define PIN_T1S_GPS   87    //GPS 1秒脉冲
#define PIN_EXT_GPS   86    //GPS 低功耗唤醒

//蓝牙
#define PIN_RX5_BLE   83    //串口5 蓝牙接收
#define PIN_EN_BLE    82    //蓝牙电源
#define PIN_RST_BLE   81    //蓝牙复位
#define PIN_TX5_BLE   80    //串口5 蓝牙发送

//NB
#define PIN_RXL1_NB   79    //低功耗串口1 NB接收
#define PIN_TXL1_NB   78    //低功耗串口1 NB发送
#define PIN_RES_MCU   77    //NB 复位MCU
#define PIN_ON_OFF    71    //NB PWK

//三轴
#define PIN_VDD_ADXL  64    //三轴电源
#define PIN_SCLK_AD   63    //三轴 SCLK
#define PIN_MOSI_AD   62    //三轴 MOSI
#define PIN_MISO_AD   61    //三轴 MISO 
#define PIN_CS_AD     60    //三轴 CS
#define PIN_INT2_AD   59    //三轴 中断2
#define PIN_INT1_AD   58    //三轴 中断1

//灯带
#define PIN_LED_CTR   48    //灯带CTR

//5V电源
#define PIN_POW_5V    52    //5V电源

//24V电源
#define PIN_POW_24V   51    //24V电源

//调试
#define PIN_RX4_T     40    //串口4 调试接收
#define PIN_TX4_T     39    //串口4 调试发送


//未使用
#define PIN_OSC32_IN  8     //外部32.768k时钟
#define PIN_OSC32_OUT 9
#define PIN_OSC_IN    12    //外部8M时钟
#define PIN_OSC_OUT   13

#define PIN_SWDIO     72    //烧写器 SWDIO脚
#define PIN_SWCLK     76    //烧写器 SWCLK脚

#define PIN_TX4_SP    23    //串口4 备用发送
#define PIN_RX4_SP    24    //串口4 备用接收

#define PIN_RX1_485   69    //串口1 485接收
#define PIN_TX1_485   68    //串口1 485发送
#define PIN_485_C     67    //485 片选
#define PIN_BUZ       65    //蜂鸣器

#define PIN_RXL1_SP   56    //低功耗串口1 备用接收
#define PIN_TXL1_SP   55    //低功耗串口1 备用发送

#define PIN_RFID_MISO 46    //RFID MISO
#define PIN_RFID_MOSI 45    //RFID MOSI
#define PIN_RFID_CLK  44    //RFID CLK
#define PIN_RFID_SS   43    //RFID SS
#define PIN_RFIS_SSI  42    //RFID SSi
#define PIN_POW_RFID  41    //RFID 电源

#define PIN_RESERVE1  1
#define PIN_RESERVE2  2
#define PIN_RESERVE3  7
#define PIN_RESERVE4  18
#define PIN_RESERVE5  92
#define PIN_RESERVE6  85
#define PIN_RESERVE7  84
#define PIN_RESERVE8  70
#define PIN_RESERVE9  66
#define PIN_RESERVE10 57
#define PIN_RESERVE11 54
#define PIN_RESERVE12 53
#define PIN_RESERVE13 47

#define ITEM_NUM(items) sizeof(items)/sizeof(items[0])


#endif
