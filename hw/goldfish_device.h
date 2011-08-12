/* Copyright (C) 2007-2008 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#ifndef GOLDFISH_DEVICE_H
#define GOLDFISH_DEVICE_H
#include "hw/hw.h"
#include "hw/qdev.h"

typedef struct GoldfishDevice {
    DeviceState qdev;
    struct GoldfishDevice *next;
    uint32_t reported_state;
    char *name;
    uint32_t base;
    uint32_t id;
    uint32_t size;
    uint32_t irq;
    uint32_t irq_count;
} GoldfishDevice;

typedef struct {
    DeviceInfo qdev;
    CPUReadMemoryFunc **readfn;
    CPUWriteMemoryFunc **writefn;
    int (*init)(GoldfishDevice *dev);
} GoldfishDeviceInfo;

typedef struct GoldfishBus {
    BusState bus;
    GoldfishDevice dev;
    GoldfishDevice *current;
} GoldfishBus;

void goldfish_bus_register_withprop(GoldfishDeviceInfo *info);
GoldfishBus *goldfish_bus_init(uint32_t base, uint32_t irq);
int goldfish_add_device_no_io(GoldfishDevice *dev);

DeviceState *goldfish_timer_create(GoldfishBus *gbus, uint32_t base, int irq);
DeviceState *goldfish_rtc_create(GoldfishBus *gbus);
DeviceState *goldfish_int_create(GoldfishBus *gbus, uint32_t base, qemu_irq parent_irq, qemu_irq parent_fiq);
DeviceState *goldfish_tty_create(GoldfishBus *gbus, CharDriverState *cs, int id, uint32_t base, int irq);
DeviceState *goldfish_fb_create(GoldfishBus *gbus, int id);
DeviceState *goldfish_nand_create(GoldfishBus *gbus);
DeviceState *goldfish_events_create(GoldfishBus *gbus, DeviceState *goldfish_int_dev);
DeviceState *goldfish_switch_create(GoldfishBus *gbus, const char *name_dev, uint32_t (*writefn)(void *opaque, uint32_t state), void *writeopaque, int id);
DeviceState *goldfish_battery_create(GoldfishBus *gbus);
DeviceState *goldfish_memlog_create(GoldfishBus *gbus, uint32_t base);
DeviceState *goldfish_mmc_create(GoldfishBus *gbus, uint32_t base, int id);
void goldfish_switch_set_state(void *opaque, uint32_t state);
void goldfish_device_init(DeviceState *dev, uint32_t base, uint32_t irq);
void goldfish_device_set_irq(GoldfishDevice *dev, int irq, int level);
void goldfish_battery_set_prop(void *opaque, int ac, int property, int value);
void goldfish_battery_display(void *opaque, void (* callback)(void *data, const char* string), void *data);
#endif
