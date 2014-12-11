#include "pwm.h"
#include "stm32f10x_tim.h"

#define PWM_USETIMER2

#ifdef PWM_USETIMER2
	#define PWM_TIMER				(TIM2)
	#define PWM_TIMER_RCC   (RCC_APB1Periph_TIM2)
#endif


#ifdef RT_USING_FINSH
#include <finsh.h>
void set_pwm(uint16_t arr,uint16_t psc,uint16_t pulse)
{
	/*http://www.xuebuyuan.com/2038717.html
	f = 72M/[(psc+1)*(arr+1)]   
	*/
    /* 初始化结构体定义 */
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef 	TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOA,GPIO_Pin_0);
	

	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  		//选择定时器模式:TIM PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	//TIM_OCInitStructure.TIM_Pulse = (arr+1)/2;	  		//占空比 50%
	TIM_OCInitStructure.TIM_Pulse = (arr+1)/pulse;	  			//占空比1:3
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	/* 使能TIM2在CCR1上的预装载寄存器 */
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	/* 使能定时器 */
	TIM_Cmd(TIM2, ENABLE); 			
}
FINSH_FUNCTION_EXPORT(set_pwm, set pwm on PA0 arr psc 1/pulse.)
#endif

