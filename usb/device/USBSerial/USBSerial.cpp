/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stdint.h"
#include "USBSerial.h"

int USBSerial::_putc(int c)
{
    return send((uint8_t *)&c, 1) ? 1 : 0;
}

int USBSerial::_getc()
{
    uint8_t c = 0;
    if (receive(&c, sizeof(c))) {
        return c;
    } else {
        return -1;
    }
}

void USBSerial::data_rx()
{
    assert_locked();

    //call a potential handler
    if (rx) {
        rx.call();
    }
}

uint8_t USBSerial::available()
{
    USBCDC::lock();

    uint8_t size = 0;
    if (!_rx_in_progress) {
        size = _rx_size > 0xFF ? 0xFF : _rx_size;
    }

    USBCDC::unlock();
    return size;
}

bool USBSerial::connected()
{
    return _terminal_connected;
}