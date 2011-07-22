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

/*
struct goldfish_device {
    struct goldfish_device *next;
    struct goldfish_device *prev;
    uint32_t reported_state;
    void *cookie;
    const char *name;
    uint32_t id;
    uint32_t base; // filled in by goldfish_device_add if 0
    uint32_t size;
    uint32_t irq; // filled in by goldfish_device_add if 0
    uint32_t irq_count;
};
*/

typedef struct GoldfishDevice {
    DeviceState qdev;
    struct GoldfishDevice *next;
    uint32_t reported_state;
    const char *name;
    uint32_t base;
    uint32_t id;
    uint32_t size;
    uint32_t irq;
    uint32_t irq_count;
} GoldfishDevice;

typedef struct {
    DeviceInfo qdev;
//    const char *dt_name, *dt_type, *dt_compatible;
//    target_ulong signal_mask;
    CPUReadMemoryFunc **readfn;
    CPUWriteMemoryFunc **writefn;
    int (*init)(GoldfishDevice *dev);
//    void (*hcalls)(GoldfishBus *bus);
//    int (*devnode)(GoldfishDevice *dev, void *fdt, int node_off);
} GoldfishDeviceInfo;

typedef struct GoldfishBus {
    BusState bus;
    GoldfishDevice dev;
    GoldfishDevice *current;
} GoldfishBus;

void goldfish_bus_register_withprop(GoldfishDeviceInfo *info);
GoldfishBus *goldfish_bus_init(void);

static inline DeviceState *goldfish_bus_create(GoldfishBus *gbus, uint32_t base, int irq)
{
    DeviceState *dev;

    dev = qdev_create(&gbus->bus, "goldfish-timer");
    qdev_prop_set_uint32(dev, "base", base);
    qdev_prop_set_uint32(dev, "irq", irq);
    qdev_init_nofail(dev);

    return dev;
}

DeviceState *goldfish_device_bus_create(GoldfishBus *gbus, uint32_t base, uint32_t irq);
DeviceState *goldfish_timer_create(GoldfishBus *gbus, uint32_t base, int irq);
DeviceState *goldfish_rtc_create(GoldfishBus *gbus);
DeviceState *goldfish_int_create(GoldfishBus *gbus, uint32_t base, qemu_irq parent_irq, qemu_irq parent_fiq);
DeviceState *goldfish_tty_create(GoldfishBus *gbus, CharDriverState *cs, int id, uint32_t base, int irq);
void goldfish_device_init(qemu_irq *pic, uint32_t base, uint32_t irq);

void goldfish_device_set_irq(GoldfishDevice *dev, int irq, int level);
/*int goldfish_device_add(GoldfishDevice *dev,
                       CPUReadMemoryFunc **mem_read,
                       CPUWriteMemoryFunc **mem_write,
                       void *opaque);
*/
//int goldfish_add_device_no_io(GoldfishDevice *dev);

//void goldfish_device_init(qemu_irq *pic, uint32_t base, uint32_t size, uint32_t irq, uint32_t irq_count);
//int goldfish_device_bus_init(uint32_t base, uint32_t irq);

// device init functions:
qemu_irq *goldfish_interrupt_init(uint32_t base, qemu_irq parent_irq, qemu_irq parent_fiq);
void goldfish_timer_and_rtc_init(uint32_t timerbase, int timerirq);
int goldfish_tty_add(CharDriverState *cs, int id, uint32_t base, int irq);

#endif
