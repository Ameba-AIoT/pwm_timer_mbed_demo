#include "pwm_timer_mbed_RGB.h"

// 定时器对象
gtimer_t gtimer1;
gtimer_t gtimer2;
gtimer_t gtimer3;

// GPIO 对象，用于控制 3 路 LED
gpio_t led1_gpio;
gpio_t led2_gpio;
gpio_t led3_gpio;

// 中断回调与主循环之间的通信标志
volatile uint32_t time1_expired = 0;
volatile uint32_t time2_expired = 0;
volatile uint32_t time3_expired = 0;

/**
 * @brief Timer1 超时回调
 *
 */
void timer1_timeout_handler(uint32_t id)
{
    (void)id;
    time1_expired = 1;
}

/**
 * @brief Timer2 超时回调
 */
void timer2_timeout_handler(uint32_t id)
{
    (void)id;
    time2_expired = 1;
}

/**
 * @brief Timer3 超时回调
 */
void timer3_timeout_handler(uint32_t id)
{
    (void)id;
    time3_expired = 1;
}

/**
 * @brief PWM Timer RGB 示例入口函数
 *
 * 说明：
 * - LED1 使用 TIMER1 周期定时中断，周期性闪烁
 * - LED2 使用 TIMER2  定时器，到期后在任务中翻转并重新启动
 * - LED3 使用 TIMER3  定时器，到期后在任务中翻转并重新启动
 *
 */
void pwm_timer_mbed_RGB(void)
{
    // 1. 初始化 GPIO
    gpio_init(&led1_gpio, LED1_PIN);
    gpio_init(&led2_gpio, LED2_PIN);
    gpio_init(&led3_gpio, LED3_PIN);

    // 设置为输出模式
    gpio_dir(&led1_gpio, PIN_OUTPUT);
    gpio_dir(&led2_gpio, PIN_OUTPUT);
    gpio_dir(&led3_gpio, PIN_OUTPUT);

    // 禁用上下拉
    gpio_mode(&led1_gpio, PullNone);
    gpio_mode(&led2_gpio, PullNone);
    gpio_mode(&led3_gpio, PullNone);

    // 初始熄灭
    gpio_write(&led1_gpio, 0);
    gpio_write(&led2_gpio, 0);
    gpio_write(&led3_gpio, 0);

    // 2. 初始化通用定时器
    /**
     * void gtimer_init(gtimer_t *obj, uint32_t tid)
     * @param tid:
     *     TIMER0 / TIMER1 / ... / TIMER11
     */
    gtimer_init(&gtimer1, TIMER1);
    gtimer_init(&gtimer2, TIMER2);
    gtimer_init(&gtimer3, TIMER3);

    // 清零标志
    time1_expired = 0;
    time2_expired = 0;
    time3_expired = 0;

    // 3. 启动定时器
    // 3.1 TIMER1 周期定时器，用于周期性闪烁 LED1
    gtimer_start_periodical(&gtimer1,
                            (uint32_t)PWM_TIME_US,          // 周期（us）
                            (void *)timer1_timeout_handler, // 回调函数
                            (uint32_t)&led1_gpio);          // 参数（这里没有用到，只是保持原型）

    // 3.2 TIMER2 / TIMER3  定时器
    // 到期后在任务中翻转 LED2 / LED3 并重新启动 
    gtimer_start_one_shout(&gtimer2,
                           (uint32_t)LED2_TIME_US,
                           (void *)timer2_timeout_handler,
                           NULL);

    gtimer_start_one_shout(&gtimer3,
                           (uint32_t)LED3_TIME_US,
                           (void *)timer3_timeout_handler,
                           NULL);

    RTK_LOGI(TAG, "pwm_timer_mbed_RGB demo start!\r\n");

    // 4. 主循环：在任务上下文中处理定时事件
    while (1) {

        // 4.1 来自 TIMER1 的周期事件：LED1 闪烁
        if (time1_expired) {
            // 翻转 LED1
            gpio_write(&led1_gpio, !gpio_read(&led1_gpio));
            // 打印日志（在任务上下文中进行是安全的）
            RTK_LOGI(TAG,"timer1 timeout: led_%lu, period:%lu ms\r\n",
                   (uint32_t)led1_gpio.pin,
                   (uint32_t)PWM_TIME_US / 1000);

            // 清标志
            time1_expired = 0;
        }

        // 4.2 来自 TIMER2  事件：LED2 翻转并重启 
        if (time2_expired) {
            gpio_write(&led2_gpio, !gpio_read(&led2_gpio));
            RTK_LOGI(TAG,"timer2 timeout: led_%lu, period:%lu ms\r\n",
                   (uint32_t)led2_gpio.pin,
                   (uint32_t)LED2_TIME_US / 1000);

            // 重新启动 TIMER2 
            gtimer_start_one_shout(&gtimer2,
                                   (uint32_t)LED2_TIME_US,
                                   (void *)timer2_timeout_handler,
                                   NULL);

            time2_expired = 0;
        }

        // 4.3 来自 TIMER3  事件：LED3 翻转并重启 
        if (time3_expired) {
            gpio_write(&led3_gpio, !gpio_read(&led3_gpio));
            RTK_LOGI(TAG,"timer3 timeout: led_%lu, period:%lu ms\r\n",
                   (uint32_t)led3_gpio.pin,
                   (uint32_t)LED3_TIME_US / 1000);

            // 重新启动 TIMER3 
            gtimer_start_one_shout(&gtimer3,
                                   (uint32_t)LED3_TIME_US,
                                   (void *)timer3_timeout_handler,
                                   NULL);

            time3_expired = 0;
        }       
        rtos_time_delay_ms(1);
    }

    // 理论上这个 demo 不会返回到这里
    rtos_task_delete(NULL);
}
