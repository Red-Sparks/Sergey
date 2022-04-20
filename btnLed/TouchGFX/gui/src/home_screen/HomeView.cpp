#include <gui/home_screen/HomeView.hpp>
#include "stm32746g_discovery.h"

HomeView::HomeView()
{

}

void HomeView::setupScreen()
{
    HomeViewBase::setupScreen();
}

void HomeView::tearDownScreen()
{
    HomeViewBase::tearDownScreen();
}

void HomeView:: switchLed()
{
	BSP_LED_On(LED1);
}
