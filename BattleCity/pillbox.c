#include "pillbox.h"
#include "draw_tool.h"
#include "map.h"

void ShowPillbox(Pillbox *pPillbox)
{
	if (pPillbox == NULL)
		return;

	int nTX = pPillbox->nX;
	int nTY = pPillbox->nY;
	WORD wAttr = FG_GRAY;

	if (pPillbox->bDead)
		wAttr = FG_RED;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			UpdateMapPoint(SIGN_PILLBOX, nTX + j, nTY + i);
			if (g_PboxStatus[i][j] == 1) {
				WriteChar(nTX + j, nTY + i, "бя", wAttr);
			}
		}
	}
}

Pillbox *InitPillbox(void)
{
	Pillbox *pPillbox = (Pillbox *)calloc(1, sizeof(Pillbox));
	pPillbox->bDead = 0;
	pPillbox->nX = 17;
	pPillbox->nY = 36;
	pPillbox->eType = SIGN_PILLBOX;

	return pPillbox;
}
