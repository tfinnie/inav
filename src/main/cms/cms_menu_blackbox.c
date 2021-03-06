/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

//
// CMS things for blackbox and flashfs.
//

#include <stdbool.h>
#include <stdint.h>

#include "platform.h"

#ifdef CMS

#include "blackbox/blackbox.h"

#include "cms/cms.h"
#include "cms/cms_types.h"
#include "cms/cms_menu_blackbox.h"

#include "common/utils.h"

#include "config/feature.h"

#include "drivers/time.h"

#include "fc/config.h"

#include "io/flashfs.h"

#ifdef USE_FLASHFS
static long cmsx_EraseFlash(displayPort_t *pDisplay, const void *ptr)
{
    UNUSED(ptr);

    displayClearScreen(pDisplay);
    displayWrite(pDisplay, 5, 3, "ERASING FLASH...");
    displayResync(pDisplay); // Was max7456RefreshAll(); Why at this timing?

    flashfsEraseCompletely();
    while (!flashfsIsReady()) {
        delay(100);
    }

    displayClearScreen(pDisplay);
    displayResync(pDisplay); // Was max7456RefreshAll(); wedges during heavy SPI?

    return 0;
}
#endif // USE_FLASHFS

static bool cmsx_Blackbox_Enabled(bool *enabled)
{
    if (enabled) {
        if (*enabled) {
            featureSet(FEATURE_BLACKBOX);
        } else {
            featureClear(FEATURE_BLACKBOX);
        }
    }
    return featureConfigured(FEATURE_BLACKBOX);
}

static OSD_Entry cmsx_menuBlackboxEntries[] =
{
    { "-- BLACKBOX --", OME_Label,      NULL, NULL, 0},
    { "ENABLED",        OME_BoolFunc,   NULL, cmsx_Blackbox_Enabled, 0 },
    OSD_SETTING_ENTRY("RATE DENOM", SETTING_BLACKBOX_RATE_DENOM),
#ifdef USE_FLASHFS
    { "ERASE FLASH",OME_Funcall, cmsx_EraseFlash, NULL, 0 },
#endif // USE_FLASHFS

    { "BACK", OME_Back, NULL, NULL, 0 },
    { NULL, OME_END, NULL, NULL, 0 }
};

CMS_Menu cmsx_menuBlackbox = {
    .GUARD_text = "MENUBB",
    .GUARD_type = OME_MENU,
    .onEnter = NULL,
    .onExit = NULL,
    .onGlobalExit = NULL,
    .entries = cmsx_menuBlackboxEntries
};
#endif
