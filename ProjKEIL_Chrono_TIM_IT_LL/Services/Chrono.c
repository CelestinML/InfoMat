// A COMPLETER

/*
Service permettant de chornom�trer jusqu'� 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 � TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "Math.h"

// variable priv�e de type Time qui m�morise la dur�e mesur�e
static Time Chrono_Time; // rem : static rend la visibilit� de la variable Chrono_Time limit�e � ce fichier 

// variable priv�e qui m�morise pour le module le timer utilis� par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par d�faut au cas o� l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// d�claration callback appel� toute les 10ms
void Chrono_Task_10ms(void);

void CalculArrPsc(float periode_int, int* psc, int* arr) {
	float arr_temp = 0xFFFF;
	float psc_temp = ceil((72.0*pow(10, 6) *  periode_int) / (arr_temp + 1.0) - 1.0);
	*arr = (int)(periode_int * 72.0*pow(10, 6)) / (psc_temp + 1.0) - 1.0;
	*psc = (int)psc_temp;
}

/**
	* @brief  Configure le chronom�tre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer)
{
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	// Fixation du Timer
	Chrono_Timer=Timer;

	int psc, arr;
	CalculArrPsc(1, &psc, &arr);
	
	// R�glage Timer pour un d�bordement � 10ms
	MyTimer_Conf(Chrono_Timer,arr, psc);
	
	// R�glage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
	
	//Activation des IO
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //Activation de la clock des IO
	GPIOC->CRH |= GPIO_CRH_MODE10; //Config en output de la pin10
	GPIOC->CRH &= ~GPIO_CRH_CNF10; //Config en push pull de la pin10
}


/**
	* @brief  D�marre le chronom�tre. 
  * @note   si la dur�e d�passe 59mn 59sec 99 Hund, elle est remise � z�ro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
}


/**
	* @brief  Arr�te le chronom�tre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronom�tre � 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arr�t Chrono
	MyTimer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time priv�e g�r�e dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}

/**
	* @brief  incr�mente la variable priv�e Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ 
	if ((GPIOC->ODR & (0X01 << 10 )) != 0 )
		GPIOC->ODR &= ~GPIO_ODR_ODR10;
	else
		GPIOC->ODR |= GPIO_ODR_ODR10;
}