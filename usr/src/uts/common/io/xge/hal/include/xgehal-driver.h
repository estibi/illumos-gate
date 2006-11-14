/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 *
 * Copyright (c) 2002-2006 Neterion, Inc.
 */

#ifndef XGE_HAL_DRIVER_H
#define XGE_HAL_DRIVER_H

#include "xge-os-pal.h"
#include "xge-list.h"
#include "xge-queue.h"
#include "xgehal-types.h"
#include "xgehal-config.h"
#include "xgehal-event.h"

__EXTERN_BEGIN_DECLS

/* maximum number of events consumed in a syncle poll() cycle */
#define XGE_HAL_DRIVER_QUEUE_CONSUME_MAX	5


/**
 * function xge_uld_sched_timer_cb_f - Per-device periodic timer
 * callback.
 * @devh: HAL device handle.
 * @userdata: Per-device user data (a.k.a. context) specified via
 * xge_hal_device_initialize().
 *
 * Periodic or one-shot timer callback. If specified (that is, not NULL)
 * HAL invokes this callback periodically. The call is performed in the
 * interrupt context, or more exactly, in the context of HAL's ISR
 * xge_hal_device_continue_irq().
 *
 * See also: xge_hal_device_initialize{}
 */
typedef void (*xge_uld_sched_timer_cb_f)(xge_hal_device_h devh, void *userdata);

/**
 * function xge_uld_link_up_f - Link-Up callback provided by upper-layer
 * driver.
 * @userdata: Opaque context set by the ULD via
 * xge_hal_device_private_set()
 * (typically - at HAL device iinitialization time).
 *
 * Link-up notification callback provided by the ULD.
 * This is one of the per-driver callbacks, see xge_hal_uld_cbs_t{}.
 *
 * See also: xge_hal_uld_cbs_t{}, xge_uld_link_down_f{},
 * xge_hal_driver_initialize(), xge_hal_device_private_set().
 */
typedef void (*xge_uld_link_up_f) (void *userdata);

/**
 * function xge_uld_link_down_f - Link-Down callback provided by
 * upper-layer driver.
 * @userdata: Opaque context set by the ULD via
 * xge_hal_device_private_set()
 * (typically - at HAL device iinitialization time).
 *
 * Link-Down notification callback provided by the upper-layer driver.
 * This is one of the per-driver callbacks, see xge_hal_uld_cbs_t{}.
 *
 * See also: xge_hal_uld_cbs_t{}, xge_uld_link_up_f{},
 * xge_hal_driver_initialize(), xge_hal_device_private_set().
 */
typedef void (*xge_uld_link_down_f) (void *userdata);

/**
 * function xge_uld_crit_err_f - Critical Error notification callback.
 * @userdata: Opaque context set by the ULD via
 * xge_hal_device_private_set()
 * (typically - at HAL device iinitialization time).
 * @type: Enumerated hw error, e.g.: double ECC.
 * @serr_data: Xframe status.
 * @ext_data: Extended data. The contents depends on the @type.
 *
 * Link-Down notification callback provided by the upper-layer driver.
 * This is one of the per-driver callbacks, see xge_hal_uld_cbs_t{}.
 *
 * See also: xge_hal_uld_cbs_t{}, xge_hal_event_e{},
 * xge_hal_device_private_set(), xge_hal_driver_initialize().
 */
typedef void (*xge_uld_crit_err_f) (void *userdata, xge_hal_event_e type,
		u64 ext_data);

/**
 * function xge_uld_event_queued_f - Event-enqueued notification
 * callback.
 * @devh: HAL device handle.
 * @event_type: HAL- or ULD-defined event type. Note that HAL
 *              events are enumerated by xge_hal_event_e{}.
 *
 * "Event-was-enqueued" notification callback provided by the upper-layer
 * driver. The callback is invoked (if defined, i.e., not NULL in the
 * xge_hal_uld_cbs_t{} structure) each time immediately after an event
 * is enqueued.
 *
 * See also: xge_hal_uld_cbs_t{}, xge_hal_device_private_set(),
 * xge_hal_driver_initialize().
 */
typedef void (*xge_uld_event_queued_f) (xge_hal_device_h devh, int event_type);

/**
 * function xge_uld_event_f - ULD event callback.
 * @item: ULD-defined event, item of the xge_queue_t.
 *
 * ULD event callback.
 * Upper-layer driver can use HAL queue to serialize certain slow-path
 * events. HAL periodically polls the queue as part of the
 * xge_hal_device_poll() processing. When/if HAL discovers in the queue
 * an unkown event type it simply invokes the event callback
 * (which must be non-NULL and supplied by the ULD in this case).
 *
 * See also: xge_hal_uld_cbs_t{}, xge_hal_device_poll(), xge_queue_t{},
 * xge_hal_driver_initialize(), xge_queue_item_t{}.
 */
typedef void (*xge_uld_event_f) (xge_queue_item_t *item);

/**
 * function xge_uld_before_device_poll_f - ULD "before-poll" callback.
 * @devh: HAL device handle.
 *
 * HAL invokes the callback from inside its xge_hal_device_poll()
 * implementation %prior to accessing the @devh device. This allows ULD to
 * perform per-device locking and/or context mapping, if required..
 * The interface is currently used by AIX driver only.
 * To avoid using/implementing the callback set the corresponding field
 * in the xge_hal_uld_cbs_t{} structure to NULL.
 *
 * Returns: 0 on success, non-zero on failure.
 *
 * See also: xge_hal_driver_initialize(), xge_hal_uld_cbs_t{},
 * xge_hal_device_poll().
 */
typedef int (*xge_uld_before_device_poll_f) (xge_hal_device_h devh);

/**
 * function xge_uld_after_device_poll_f - ULD "after-poll" callback.
 * @devh: HAL device handle.
 *
 * Unless NULL is specified,
 * HAL invokes the callback from inside its xge_hal_device_poll()
 * implementation immediately %after it has completed polling the @devh
 * device. This allows ULD to undo the affects of
 * xge_uld_before_device_poll_f{}.
 * The interface is currently used by AIX driver only.
 *
 * See also: xge_hal_driver_initialize(), xge_hal_uld_cbs_t{},
 * xge_hal_device_poll().
 */
typedef void (*xge_uld_after_device_poll_f) (xge_hal_device_h devh);

/**
 * function xge_uld_xpak_alarm_log_f - ULD "XPAK alarm log" callback.
 * @devh: HAL device handle.
 *
 * Unless NULL is specified,
 * HAL invokes the callback from inside __hal_chk_xpak_counter()
 */
typedef void (*xge_uld_xpak_alarm_log_f) (xge_hal_device_h devh, xge_hal_xpak_alarm_type_e type);

/**
 * struct xge_hal_uld_cbs_t - Upper-layer driver "slow-path" callbacks.
 * @link_up: See xge_uld_link_up_f{}.
 * @link_down: See xge_uld_link_down_f{}.
 * @crit_err: See xge_uld_crit_err_f{}.
 * @event: See xge_uld_event_f{}.
 * @event_queued: See xge_uld_event_queued_f{}.
 * @before_device_poll: See xge_uld_before_device_poll_f{}.
 * @after_device_poll: See xge_uld_after_device_poll_f{}.
 * @sched_timer: See xge_uld_sched_timer_cb_f{}.
 *
 * Upper layer driver slow-path (per-driver) callbacks.
 * Implemented by ULD and provided to HAL via
 * xge_hal_driver_initialize().
 * Note that these callbacks are not mandatory: HAL will not invoke
 * a callback if NULL is specified.
 *
 * Note that in addition to those, there are curently 2 per-channel callbacks
 * (completion and abort) specified at channel open time
 * via xge_hal_channel_open().
 *
 * See also: xge_hal_driver_initialize().
 */
typedef struct xge_hal_uld_cbs_t {
	xge_uld_link_up_f		link_up;
	xge_uld_link_down_f		link_down;
	xge_uld_crit_err_f		crit_err;
	xge_uld_event_f			event;
	xge_uld_event_queued_f		event_queued;
	xge_uld_before_device_poll_f    before_device_poll;
	xge_uld_after_device_poll_f	after_device_poll;
	xge_uld_sched_timer_cb_f	sched_timer;
	xge_uld_xpak_alarm_log_f	xpak_alarm_log;
} xge_hal_uld_cbs_t;

/**
 * struct xge_hal_driver_t - Represents HAL object.
 * @config: HAL configuration.
 * @devices: List of all PCI-enumerated Xframe devices in the system.
 * A single xge_hal_driver_t instance contains zero or more
 * Xframe devices.
 * @devices_lock: Lock to protect %devices when inserting/removing.
 * @is_initialized: True if HAL is initialized; false otherwise.
 * @uld_callbacks: Upper-layer driver callbacks. See xge_hal_uld_cbs_t{}.
 * @debug_module_mask: 32bit mask that defines which components of the
 * driver are to be traced. The trace-able components are:
 *	XGE_COMPONENT_HAL_CONFIG		0x1
 *	XGE_COMPONENT_HAL_FIFO			0x2
 *	XGE_COMPONENT_HAL_RING			0x4
 *	XGE_COMPONENT_HAL_CHANNEL		0x8
 *	XGE_COMPONENT_HAL_DEVICE		0x10
 *	XGE_COMPONENT_HAL_MM			0x20
 *	XGE_COMPONENT_HAL_QUEUE			0x40
 *	XGE_COMPONENT_HAL_STATS			0x100
 *	XGE_COMPONENT_OSDEP			0x1000
 *	XGE_COMPONENT_LL			0x2000
 *	XGE_COMPONENT_TOE			0x4000
 *	XGE_COMPONENT_RDMA			0x8000
 *	XGE_COMPONENT_ALL			0xffffffff
 * The @debug_module_mask allows to switch off and on tracing at runtime.
 * In addition, the traces for the same trace-able components can be
 * compiled out, based on the same mask provided via Makefile.
 * @debug_level: See xge_debug_level_e{}.
 *
 * HAL (driver) object. There is a single instance of this structure per HAL.
 */
typedef struct xge_hal_driver_t {
	xge_hal_driver_config_t		config;
	int                             is_initialized;
	xge_hal_uld_cbs_t               uld_callbacks;
	u32				debug_module_mask;
	int				debug_level;
} xge_hal_driver_t;

extern xge_hal_driver_t *g_xge_hal_driver;

static inline int
xge_hal_driver_is_initialized(void) {
        return g_xge_hal_driver->is_initialized;
}

static inline int
xge_hal_driver_debug_module_mask(void)
{
	return g_xge_hal_driver->debug_module_mask;
}

static inline void
xge_hal_driver_debug_module_mask_set(u32 new_mask)
{
#if (defined(XGE_DEBUG_TRACE_MASK) && XGE_DEBUG_TRACE_MASK > 0) || \
    (defined(XGE_DEBUG_ERR_MASK) && XGE_DEBUG_ERR_MASK > 0)
	g_xge_hal_driver->debug_module_mask = new_mask;
	g_module_mask = &g_xge_hal_driver->debug_module_mask;
#endif
}

static inline int
xge_hal_driver_debug_level(void) { return g_xge_hal_driver->debug_level; }

static inline void
xge_hal_driver_debug_level_set(int new_level)
{
#if (defined(XGE_DEBUG_TRACE_MASK) && XGE_DEBUG_TRACE_MASK > 0) || \
    (defined(XGE_DEBUG_ERR_MASK) && XGE_DEBUG_ERR_MASK > 0)
	g_xge_hal_driver->debug_level = new_level;
	g_level = &g_xge_hal_driver->debug_level;
#endif
}

static inline void
xge_hal_driver_module_mask_set(int new_mask) {
	g_xge_hal_driver->debug_module_mask = new_mask; }

xge_hal_status_e xge_hal_driver_initialize(xge_hal_driver_config_t *config,
		xge_hal_uld_cbs_t *uld_callbacks);

void xge_hal_driver_terminate(void);

#ifdef XGE_TRACE_INTO_CIRCULAR_ARR
static inline void
xge_hal_driver_tracebuf_timestamp(int on)
{
	g_xge_os_tracebuf->timestamp = on;
}

void xge_hal_driver_tracebuf_dump(void);
#else
#define xge_hal_driver_tracebuf_timestamp(a)
#define xge_hal_driver_tracebuf_dump()
#endif

__EXTERN_END_DECLS

#endif /* XGE_HAL_DRIVER_H */
