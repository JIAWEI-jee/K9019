#ifndef _PRODUCT_H
#define _PRODUCT_H
#include "HC89F303.h"

#define PRODUCT_CHECK_IN   P2_0
#define PRODUCT_OK         P2_2

enum producte_test_list
{
    CHECK_NULL,
    CHECK_SUCCESS,
    CHECK_FAIL,
    RES_OK,
};

enum producte_process_state_list
{
    PRODUCTE_INIT,    //初始化需要驱动的设备
    PRODUCTE_TEST,
    PRODUCTE_END,     //测试结束
    PRODUCTE_ERROR,   //测试错误
    PRODUCTE_PASS     //测试通过
};

void product_check_process ( void );

#endif 


