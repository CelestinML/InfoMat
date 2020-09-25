// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 
#include "stm32f103xb.h" 

//Définition du pointeur sur la fonciton voulue
void (*pfonction)(void);

//Redéfinition des handlers
void TIM2_IRQHandler (void) {
	(*pfonction)();
	TIM2->SR &= ~TIM_SR_UIF;
}

void TIM1_UP_IRQHandler (void) {
	(*pfonction)();
	TIM1->SR &= ~TIM_SR_UIF;
}

void TIM3_IRQHandler (void) {
	(*pfonction)();
	TIM3->SR &= ~TIM_SR_UIF;
}

void TIM4_IRQHandler (void) {
	(*pfonction)();
	TIM4->SR &= ~TIM_SR_UIF;
}

void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc) {
	//Activation de l'horloge
	if (Timer == TIM1) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	}
	else if (Timer == TIM2) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if (Timer == TIM3) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if (Timer == TIM4) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	Timer->PSC = Psc;
	Timer->ARR = Arr;
}

void MyTimer_Start(TIM_TypeDef * Timer) {
	Timer->CR1 |= TIM_CR1_CEN;
}

void MyTimer_Stop(TIM_TypeDef * Timer) {
	Timer->CR1 &= ~TIM_CR1_CEN;
}

void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio) {
	pfonction = IT_function;
	if (Timer == TIM1) {
		NVIC->IP[25] = Prio << 4;
	}
	else if (Timer == TIM2) {
		NVIC->IP[28] = Prio << 4;
	}
	else if (Timer == TIM3) {
		NVIC->IP[29] = Prio << 4;
	}
	else if (Timer == TIM4) {
		NVIC->IP[30] = Prio << 4;
	}
}

void MyTimer_IT_Enable(TIM_TypeDef * Timer) {
	Timer->DIER |= TIM_DIER_UIE;
	if (Timer == TIM1) {
		NVIC->ISER[0] |= (1 << 25);
	}
	else if (Timer == TIM2) {
		NVIC->ISER[0] |= (1 << 28);
	}
	else if (Timer == TIM3) {
		NVIC->ISER[0] |= (1 << 29);
	}
	else if (Timer == TIM4) {
		NVIC->ISER[0] |= (1 << 30);
	}
}
	
void MyTimer_IT_Disable(TIM_TypeDef * Timer) {
		Timer->DIER &= ~TIM_DIER_UIE;
	if (Timer == TIM1) {
		NVIC->ISER[0] &= ~(1 << 25);
	}
	else if (Timer == TIM2) {
		NVIC->ISER[0] &= ~(1 << 28);
	}
	else if (Timer == TIM3) {
		NVIC->ISER[0] &= ~(1 << 29);
	}
	else if (Timer == TIM4) {
		NVIC->ISER[0] &= ~(1 << 30);
	}
}