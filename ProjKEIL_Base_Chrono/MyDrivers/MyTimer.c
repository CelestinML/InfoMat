// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des p�riph�riques.
 Rem : OBLIGATION d'utiliser les d�finitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilit� du code.

 Pour les masques, utiliser �galement les d�finitions propos�e
 Rappel : pour mettre � 1  , reg = reg | Mask (ou Mask est le repr�sente le ou les bits � positionner � 1)
				  pour mettre � 0  , reg = reg&~ Mask (ou Mask est le repr�sente le ou les bits � positionner � 0)
 
*/ 
#include "stm32f103xb.h" 

//D�finition du pointeur sur la fonciton voulue
void (*pfonction)(void);

//Red�finition des handlers
void TIM2_IRQHandler (void) {
	(*pfonction)();
	TIM2->DIER &= ~TIM_SR_UIF;
}

void TIM1_UP_IRQHandler (void) {
	(*pfonction)();
	TIM1->DIER &= ~TIM_SR_UIF;
}

void TIM3_IRQHandler (void) {
	(*pfonction)();
	TIM3->DIER &= ~TIM_SR_UIF;
}

void TIM4_IRQHandler (void) {
	(*pfonction)();
	TIM4->DIER &= ~TIM_SR_UIF;
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
	//NVIC->IP
}

void MyTimer_IT_Enable(TIM_TypeDef * Timer) {
	Timer->DIER |= TIM_DIER_UIE;
	NVIC->ISER[0] = 1; //A REGLER !!!!!
}