#include "gd32f4xx_it.h"
#include "systick.h"

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void) {
    /* if NMI exception occurs, go to infinite loop */
    while (1) {
    }
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void) {
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1) {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void) {
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1) {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void) {
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1) {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void) {
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1) {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void) {
    /* if SVC exception occurs, go to infinite loop */
    while (1) {
    }
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void) {
    /* if DebugMon exception occurs, go to infinite loop */
    while (1) {
    }
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void) {
    /* if PendSV exception occurs, go to infinite loop */
    while (1) {
    }
}

/*!
    \brief    this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void) {
    delay_decrement();
}

