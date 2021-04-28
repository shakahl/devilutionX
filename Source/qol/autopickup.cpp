/**
* @file autopickup.cpp
*
* QoL feature for automatically picking up gold
*/

#include "options.h"
#include "player.h"

namespace devilution {
namespace {

bool HasRoomForGold()
{
	for (int idx : plr[myplr].InvGrid) {
		// Secondary item cell. No need to check those as we'll go through the main item cells anyway.
		if (idx < 0)
			continue;

		// Empty cell. 1x1 space available.
		if (idx == 0)
			return true;

		// Main item cell. Potentially a gold pile so check it.
		auto item = plr[myplr].InvList[idx - 1];
		if (item._itype == ITYPE_GOLD && item._ivalue < MaxGold)
			return true;
	}

	return false;
}

} // namespace

void AutoGoldPickup(int pnum)
{
	if (!sgOptions.Gameplay.bAutoGoldPickup)
		return;

	if (pnum != myplr)
		return;
	if (leveltype == DTYPE_TOWN)
		return;
	if (!HasRoomForGold())
		return;

	for (int dir = 0; dir < 8; dir++) {
		int x = plr[pnum].position.tile.x + pathxdir[dir];
		int y = plr[pnum].position.tile.y + pathydir[dir];
		if (dItem[x][y] != 0) {
			int itemIndex = dItem[x][y] - 1;
			if (items[itemIndex]._itype == ITYPE_GOLD) {
				NetSendCmdGItem(true, CMD_REQUESTAGITEM, pnum, pnum, itemIndex);
				items[itemIndex]._iRequest = true;
				PlaySFX(IS_IGRAB);
			}
		}
	}
}

} // namespace devilution