/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include <assert.h>
#include "applet.h"
#include "board.h"
#include "board_support.h"
#include "trace.h"
#include "extram/ddram.h"
#include "peripherals/pmc.h"

/*----------------------------------------------------------------------------
 *         Private functions
 *----------------------------------------------------------------------------*/

static bool check_extram(void)
{
	volatile uint32_t *extram = (volatile uint32_t *)DDR_CS_ADDR;
	int i;

	if (!pmc_is_system_clock_enabled(PMC_SYSTEM_CLOCK_DDR))
		return false;

	for (i = 0; i < 1024; i++)
		extram[i] = (i & 0xff) * 0x01010101;

	for (i = 0; i < 1024; i++)
		if (extram[i] != (i & 0xff) * 0x01010101)
			return false;

	return true;
}

static bool init_extram_from_preset(uint32_t preset)
{
	enum _ddram_devices device;
	struct _mpddrc_desc desc;

	switch (preset) {
#ifdef CONFIG_HAVE_MPDDRC_DDR2
  #ifdef CONFIG_HAVE_DDR2_MT47H128M8
	case 0:
		trace_info_wp("Preset 0 (4 x MT47H128M8)\r\n");
		device = MT47H128M8;
		break;
  #endif
  #ifdef CONFIG_HAVE_DDR2_MT47H64M16
	case 1:
		trace_info_wp("Preset 1 (MT47H64M16)\r\n");
		device = MT47H64M16;
		break;
  #endif
  #ifdef CONFIG_HAVE_DDR2_MT47H128M16
	case 2:
		trace_info_wp("Preset 2 (2 x MT47H128M16)\r\n");
		device = MT47H128M16;
		break;
  #endif
#endif
#ifdef CONFIG_HAVE_MPDDRC_LPDDR2
  #ifdef CONFIG_HAVE_LPDDR2_MT42L128M16
	case 3:
		trace_info_wp("Preset 3 (2 x MT42L128M16)\r\n");
		device = MT42L128M16;
		break;
  #endif
#endif
#ifdef CONFIG_HAVE_MPDDRC_DDR3
  #ifdef CONFIG_HAVE_DDR3_MT41K128M16
	case 4:
		trace_info_wp("Preset 4 (2 x MT41K128M16)\r\n");
		device = MT41K128M16;
		break;
  #endif
#endif
#ifdef CONFIG_HAVE_MPDDRC_LPDDR3
  #ifdef CONFIG_HAVE_LPDDR3_EDF8164A3MA
	case 5:
		trace_info_wp("Preset 5 (EDF8164A3MA)\r\n");
		device = EDF8164A3MA;
		break;
  #endif
#endif
	default:
		trace_error("Unsupported DDRAM preset (%u).\r\n",
				(unsigned)preset);
		return false;
	}

	board_cfg_matrix_for_ddr();
	ddram_init_descriptor(&desc, device);
	ddram_configure(&desc);
	return true;
}

static uint32_t handle_cmd_initialize(uint32_t cmd, uint32_t *mailbox)
{
	union initialize_mailbox *mbx = (union initialize_mailbox*)mailbox;
	uint32_t mode = mbx->in.parameters[0];

	assert(cmd == APPLET_CMD_INITIALIZE);

	applet_set_init_params(mbx->in.comm_type, mbx->in.trace_level);

	trace_info_wp("\r\nApplet 'External RAM' from softpack " SOFTPACK_VERSION ".\r\n");

	if (check_extram()) {
		trace_info_wp("External RAM already configured.\r\n");
	} else {
		switch (mode) {
		case 0:
			if (!init_extram_from_preset(mbx->in.parameters[1]))
				return APPLET_FAIL;
			break;
		default:
			trace_error("Invalid external RAM mode: only mode 0 (preset) is supported.\r\n");
			return APPLET_FAIL;
		}

		if (!check_extram()) {
			trace_error("External RAM test failed.\r\n");
			return APPLET_FAIL;
		}

		trace_info_wp("External RAM initialization complete.\r\n");
	}

	mbx->out.buf_addr = 0;
	mbx->out.buf_size = 0;
	mbx->out.page_size = 0;
	mbx->out.mem_size = 0;
	mbx->out.erase_support = 0;
	mbx->out.nand_header = 0;

	return APPLET_SUCCESS;
}

/*----------------------------------------------------------------------------
 *         Commands list
 *----------------------------------------------------------------------------*/

const struct applet_command applet_commands[] = {
	{ APPLET_CMD_INITIALIZE, handle_cmd_initialize },
	{ 0, NULL }
};
