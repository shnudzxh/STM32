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
    /* ��ʼ���ṹ�嶨�� */
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
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  		//ѡ��ʱ��ģʽ:TIM PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
	//TIM_OCInitStructure.TIM_Pulse = (arr+1)/2;	  		//ռ�ձ� 50%
	TIM_OCInitStructure.TIM_Pulse = (arr+1)/pulse;	  			//ռ�ձ�1:3
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	/* ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	/* ʹ�ܶ�ʱ�� */
	TIM_Cmd(TIM2, ENABLE); 			
}
FINSH_FUNCTION_EXPORT(set_pwm, set pwm on PA0 arr psc 1/pulse.)
#endif

