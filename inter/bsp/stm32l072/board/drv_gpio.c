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
#include "drv_gpio.h"

static void GPIOA_CLK_ENABLE(void)
{
#ifdef __HAL_RCC_GPIOA_CLK_ENABLE
    __HAL_RCC_GPIOA_CLK_ENABLE();
#endif
}
static void GPIOB_CLK_ENABLE(void)
{
#ifdef __HAL_RCC_GPIOB_CLK_ENABLE
    __HAL_RCC_GPIOB_CLK_ENABLE();
#endif
}

static void GPIOC_CLK_ENABLE(void)
{
#ifdef __HAL_RCC_GPIOC_CLK_ENABLE
    __HAL_RCC_GPIOC_CLK_ENABLE();
#endif
}

static void GPIOD_CLK_ENABLE(void)
{
#ifdef __HAL_RCC_GPIOD_CLK_ENABLE
    __HAL_RCC_GPIOD_CLK_ENABLE();
#endif
}

static void GPIOE_CLK_ENABLE(void)
{
#ifdef __HAL_RCC_GPIOE_CLK_ENABLE
    __HAL_RCC_GPIOE_CLK_ENABLE();
#endif
}

#ifdef __HAL_RCC_GPIOF_CLK_ENABLE
static void GPIOF_CLK_ENABLE(void)
{
    __HAL_RCC_GPIOF_CLK_ENABLE();
}
#endif
#ifdef __HAL_RCC_GPIOG_CLK_ENABLE
static void GPIOG_CLK_ENABLE(void)
{
    __HAL_RCC_GPIOG_CLK_ENABLE();
}
#endif
static void GPIOH_CLK_ENABLE(void)
{
#ifdef __HAL_RCC_GPIOH_CLK_ENABLE
    __HAL_RCC_GPIOH_CLK_ENABLE();
#endif
}



/* STM32 GPIO driver */
struct pin_index
{
    int index;
    void (*rcc)(void);
    GPIO_TypeDef *gpio;
    uint32_t pin;
};



static const struct pin_index pins[] =
{
      __STM32_PIN(PIN_TX4_SP , A, 0),//串口4 备用发送
      __STM32_PIN(PIN_RX4_SP , A, 1),//串口4 备用接收
      __STM32_PIN(PIN_TX2_NOS , A, 2),//串口2 噪声发送
      __STM32_PIN(PIN_RX2_NOS , A, 3),//串口2 噪声接收
      __STM32_PIN(PIN_POW_NOS , A, 4),//噪声电源
      __STM32_PIN(PIN_BAT_UP , A, 5),//电源电压监测AD口
      __STM32_PIN(PIN_BAT_DO , A, 6),//电源电压地
      __STM32_PIN(PIN_CS_W , A, 7),//W25Q8 CS片选
      __STM32_PIN(PIN_485_C , A, 8),//485 片选
      __STM32_PIN(PIN_TX1_485 , A, 9),//串口1 485发送
      __STM32_PIN(PIN_RX1_485 , A, 10),//串口1 485接收
      __STM32_PIN(PIN_RESERVE8 , A, 11),//预留
      __STM32_PIN(PIN_ON_OFF , A, 12),//NB PWK
      __STM32_PIN(PIN_SWDIO , A, 13),//烧写器 SWDIO脚
      __STM32_PIN(PIN_SWCLK , A, 14),//烧写器 SWCLK脚
      __STM32_PIN(PIN_RES_MCU , A, 15),//NB 复位MCU
      
      
      __STM32_PIN(PIN_MOSI_W , B, 0),//W25Q8 MOSI
      __STM32_PIN(PIN_SCK_W , B, 1),//W25Q8 SCK
      __STM32_PIN(PIN_HOLD_W , B, 2),//W25Q8 读写保护 
      __STM32_PIN(PIN_RX5_GPS , B, 3),//串口5 GPS接收
      __STM32_PIN(PIN_TX5_GPS , B, 4),//串口5 GPS发送
      __STM32_PIN(PIN_POW_GPS , B, 5),//GPS电源
      __STM32_PIN(PIN_RESERVE5 , B, 6),//预留
      __STM32_PIN(PIN_TH_SCL , B, 7),//温湿度 I2C SCL
      __STM32_PIN(PIN_TH_SDA , B, 8),//温湿度 I2C SDA
      __STM32_PIN(PIN_POW_TH , B, 9),//温湿度电源
      __STM32_PIN(PIN_RESERVE13 , B, 10),//预留
      __STM32_PIN(PIN_LED_CTR , B, 11),//灯带CTR
      __STM32_PIN(PIN_POW_24V , B, 12),//24V电源
      __STM32_PIN(PIN_POW_5V , B, 13),//5V电源
      __STM32_PIN(PIN_RESERVE12 , B, 14),//预留
      __STM32_PIN(PIN_RESERVE11 , B, 15),//预留
      
      
      __STM32_PIN(PIN_AD_GAS , C, 0),//硫化氢ad
      __STM32_PIN(PIN_ADC_IN1 , C, 1),//4~20mA输入1
      __STM32_PIN(PIN_ADC_IN2 , C, 2),//4~20mA输入2
      __STM32_PIN(PIN_RESERVE4 , C, 3),//预留
      __STM32_PIN(PIN_MISO_W , C, 4),//W25Q8 MISO
      __STM32_PIN(PIN_WP_W , C, 5),//W25Q8 WP
      __STM32_PIN(PIN_SCLK_AD , C, 6),//三轴 SCLK
      __STM32_PIN(PIN_VDD_ADXL , C, 7),//三轴电源
      __STM32_PIN(PIN_BUZ , C, 8),//蜂鸣器
      __STM32_PIN(PIN_RESERVE9 , C, 9),//预留
      __STM32_PIN(PIN_TXL1_NB , C, 10),//低功耗串口1 NB发送
      __STM32_PIN(PIN_RXL1_NB , C, 11),//低功耗串口1 NB接收
      __STM32_PIN(PIN_TX5_BLE , C, 12),//串口5 蓝牙发送
      __STM32_PIN(PIN_RESERVE3  , C, 13),//预留
      __STM32_PIN(PIN_OSC32_IN  , C, 14),//外部32.768k时钟
      __STM32_PIN(PIN_OSC32_OUT  , C, 15),
      
      
      __STM32_PIN(PIN_RST_BLE , D, 0),//蓝牙复位
      __STM32_PIN(PIN_EN_BLE , D, 1),//蓝牙电源
      __STM32_PIN(PIN_RX5_BLE , D, 2),//串口5 蓝牙接收
      __STM32_PIN(PIN_RESERVE7 , D, 3),//预留
      __STM32_PIN(PIN_RESERVE6 , D, 4),//预留
      __STM32_PIN(PIN_EXT_GPS , D, 5),//GPS 低功耗唤醒
      __STM32_PIN(PIN_T1S_GPS , D, 6),//GPS 1秒脉冲
      __STM32_PIN(PIN_RST_GPS , D, 7),//GPS复位
      __STM32_PIN(PIN_TXL1_SP , D, 8),//串口5 GPS发送
      __STM32_PIN(PIN_RXL1_SP , D, 9),//串口5 GPS接收
      __STM32_PIN(PIN_RESERVE10 , D, 10),//预留
      __STM32_PIN(PIN_INT1_AD , D, 11),//三轴 中断1
      __STM32_PIN(PIN_INT2_AD , D, 12),//三轴 中断2
      __STM32_PIN(PIN_CS_AD , D, 13),//三轴 CS
      __STM32_PIN(PIN_MISO_AD , D, 14),//三轴 MISO 
      __STM32_PIN(PIN_MOSI_AD , D, 15),//三轴 MOSI
      
      
      __STM32_PIN(PIN_LED2 , E, 0),//LED2
      __STM32_PIN(PIN_LED1 , E, 1),//LED1
      __STM32_PIN(PIN_RESERVE1  , E, 2),//预留
      __STM32_PIN(PIN_RESERVE2  , E, 3),//预留
      __STM32_PIN(PIN_POW_WT  , E, 4),//满水监测电源
      __STM32_PIN(PIN_WT1  , E, 5),//满水1
      __STM32_PIN(PIN_WT2  , E, 6),//满水2
      __STM32_PIN(PIN_POW_FLASH , E, 7),//W25Q8 电源
      __STM32_PIN(PIN_TX4_T , E, 8),//串口4 调试发送
      __STM32_PIN(PIN_RX4_T , E, 9),//串口4 调试接收
      __STM32_PIN(PIN_POW_RFID , E, 10),//RFID 电源
      __STM32_PIN(PIN_RFIS_SSI , E, 11),//RFID SSi
      __STM32_PIN(PIN_RFID_SS , E, 12),//RFID SS
      __STM32_PIN(PIN_RFID_CLK , E, 13),//RFID CLK
      __STM32_PIN(PIN_RFID_MOSI , E, 14),//RFID MOSI
      __STM32_PIN(PIN_RFID_MISO , E, 15),//RFID MISO
      
      
      __STM32_PIN(PIN_OSC_IN , H, 0),//外部8M时钟
      __STM32_PIN(PIN_OSC_OUT , H, 1),
      __STM32_PIN(PIN_POW_GAS , H, 9),//硫化氢电源
      __STM32_PIN(PIN_GAS_IN , H, 10),//硫化氢输入

};

struct pin_irq_map
{
    rt_uint16_t pinbit;
    IRQn_Type irqno;
};
static const struct pin_irq_map pin_irq_map[] =
{
    {GPIO_PIN_0, EXTI0_1_IRQn},
    {GPIO_PIN_1, EXTI0_1_IRQn},
    {GPIO_PIN_2, EXTI2_3_IRQn},
    {GPIO_PIN_3, EXTI2_3_IRQn},
    {GPIO_PIN_4, EXTI4_15_IRQn},
    {GPIO_PIN_5, EXTI4_15_IRQn},
    {GPIO_PIN_6, EXTI4_15_IRQn},
    {GPIO_PIN_7, EXTI4_15_IRQn},
    {GPIO_PIN_8, EXTI4_15_IRQn},
    {GPIO_PIN_9, EXTI4_15_IRQn},
    {GPIO_PIN_10, EXTI4_15_IRQn},
    {GPIO_PIN_11, EXTI4_15_IRQn},
    {GPIO_PIN_12, EXTI4_15_IRQn},
    {GPIO_PIN_13, EXTI4_15_IRQn},
    {GPIO_PIN_14, EXTI4_15_IRQn},
    {GPIO_PIN_15, EXTI4_15_IRQn},
};
struct rt_pin_irq_hdr pin_irq_hdr_tab[] =
{
    { -1, 0, RT_NULL, RT_NULL},
//    { -1, 0, RT_NULL, RT_NULL},
//    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
////    { -1, 0, RT_NULL, RT_NULL},
};

const struct pin_index *get_pin(uint8_t pin)
{
    const struct pin_index *index;
    if (pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
        if (index->index == -1)
            index = RT_NULL;
    }
    else
    {
        index = RT_NULL;
    }
    return index;
};

void stm32_pin_write(rt_device_t dev, rt_base_t pin, rt_base_t value)
{
    const struct pin_index *index;
    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }
    HAL_GPIO_WritePin(index->gpio, index->pin, (GPIO_PinState)value);
}

int stm32_pin_read(rt_device_t dev, rt_base_t pin)
{
    int value;
    const struct pin_index *index;
    value = PIN_LOW;
    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return value;
    }
    value = HAL_GPIO_ReadPin(index->gpio, index->pin);
    return value;
}

void stm32_pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    const struct pin_index *index;
    GPIO_InitTypeDef GPIO_InitStruct;
    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }
    /* GPIO Periph clock enable */
    index->rcc();
    /* Configure GPIO_InitStructure */
    GPIO_InitStruct.Pin = index->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull down. */
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* output setting: od. */
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    rt_kprintf("%x\n",GPIO_InitStruct.Mode);
    HAL_GPIO_Init(index->gpio, &GPIO_InitStruct);
}

rt_inline rt_int32_t bit2bitno(rt_uint32_t bit)
{
    int i;
    for (i = 0; i < 32; i++)
    {
        if ((0x01 << i) == bit)
        {
            return i;
        }
    }
    return -1;
}

rt_inline const struct pin_irq_map *get_pin_irq_map(uint32_t pinbit)
{
    rt_int32_t mapindex = bit2bitno(pinbit);
    if (mapindex < 0 || mapindex >= ITEM_NUM(pin_irq_map))
    {
        return RT_NULL;
    }
    return &pin_irq_map[mapindex];
};
rt_err_t stm32_pin_attach_irq(struct rt_device *device, rt_int32_t pin,
                              rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    const struct pin_index *index;
    rt_base_t level;
    rt_int32_t irqindex = -1;
    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return RT_ENOSYS;
    }
    irqindex = bit2bitno(index->pin);
    if (irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
    {
        return RT_ENOSYS;
    }
    level = rt_hw_interrupt_disable();
    if (pin_irq_hdr_tab[irqindex].pin == pin &&
            pin_irq_hdr_tab[irqindex].hdr == hdr &&
            pin_irq_hdr_tab[irqindex].mode == mode &&
            pin_irq_hdr_tab[irqindex].args == args)
    {
        rt_hw_interrupt_enable(level);
        return RT_EOK;
    }
    if (pin_irq_hdr_tab[irqindex].pin != -1)
    {
        rt_hw_interrupt_enable(level);
        return RT_EBUSY;
    }
    pin_irq_hdr_tab[irqindex].pin = pin;
    pin_irq_hdr_tab[irqindex].hdr = hdr;
    pin_irq_hdr_tab[irqindex].mode = mode;
    pin_irq_hdr_tab[irqindex].args = args;
    rt_hw_interrupt_enable(level);
    return RT_EOK;
}

rt_err_t stm32_pin_dettach_irq(struct rt_device *device, rt_int32_t pin)
{
    const struct pin_index *index;
    rt_base_t level;
    rt_int32_t irqindex = -1;
    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return RT_ENOSYS;
    }
    irqindex = bit2bitno(index->pin);
    if (irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
    {
        return RT_ENOSYS;
    }
    level = rt_hw_interrupt_disable();
    if (pin_irq_hdr_tab[irqindex].pin == -1)
    {
        rt_hw_interrupt_enable(level);
        return RT_EOK;
    }
    pin_irq_hdr_tab[irqindex].pin = -1;
    pin_irq_hdr_tab[irqindex].hdr = RT_NULL;
    pin_irq_hdr_tab[irqindex].mode = 0;
    pin_irq_hdr_tab[irqindex].args = RT_NULL;
    rt_hw_interrupt_enable(level);
    return RT_EOK;
}

rt_err_t stm32_pin_irq_enable(struct rt_device *device, rt_base_t pin,
                              rt_uint32_t enabled)
{
    const struct pin_index *index;
    const struct pin_irq_map *irqmap;
    rt_base_t level;
    rt_int32_t irqindex = -1;
    GPIO_InitTypeDef GPIO_InitStruct;
    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return RT_ENOSYS;
    }
    if (enabled == PIN_IRQ_ENABLE)
    {
        irqindex = bit2bitno(index->pin);
        if (irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
        {
            return RT_ENOSYS;
        }
        level = rt_hw_interrupt_disable();
        if (pin_irq_hdr_tab[irqindex].pin == -1)
        {
            rt_hw_interrupt_enable(level);
            return RT_ENOSYS;
        }
        irqmap = &pin_irq_map[irqindex];
        /* GPIO Periph clock enable */
        index->rcc();
        /* Configure GPIO_InitStructure */
        GPIO_InitStruct.Pin = index->pin;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        switch (pin_irq_hdr_tab[irqindex].mode)
        {
        case PIN_IRQ_MODE_RISING:
            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            break;
        case PIN_IRQ_MODE_FALLING:
            GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            break;
        case PIN_IRQ_MODE_RISING_FALLING:
            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
        }
        HAL_GPIO_Init(index->gpio, &GPIO_InitStruct);
        HAL_NVIC_SetPriority(irqmap->irqno, 5, 0);
        HAL_NVIC_EnableIRQ(irqmap->irqno);
        rt_hw_interrupt_enable(level);
    }
    else if (enabled == PIN_IRQ_DISABLE)
    {
        irqmap = get_pin_irq_map(index->pin);
        if (irqmap == RT_NULL)
        {
            return RT_ENOSYS;
        }
        HAL_NVIC_DisableIRQ(irqmap->irqno);
    }
    else
    {
        return RT_ENOSYS;
    }
    return RT_EOK;
}

const static struct rt_pin_ops _stm32_pin_ops =
{
    stm32_pin_mode,
    stm32_pin_write,
    stm32_pin_read,
    stm32_pin_attach_irq,
    stm32_pin_dettach_irq,
    stm32_pin_irq_enable,
};

int rt_hw_pin_init(void)
{
    int result;
    result = rt_device_pin_register("pin", &_stm32_pin_ops, RT_NULL);
    return result;
}
INIT_BOARD_EXPORT(rt_hw_pin_init);
/*
rt_inline void pin_irq_hdr(int irqno)
{
    if (pin_irq_hdr_tab[irqno].hdr)
    {
        pin_irq_hdr_tab[irqno].hdr(pin_irq_hdr_tab[irqno].args);
    }
}*/

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    pin_irq_hdr(bit2bitno(GPIO_Pin));
//}




