// AUTO-GENERIERT von build_hooks.py
#pragma once
#include <stdint.h>
#define KM_NHOOKS 22
#define KM_SLOT_RVA 0x1224000ULL
enum { KM_GameButtonCtor, KM_TouchBegan, KM_TouchEnded, KM_TouchMoved, KM_ButtonPressed, KM_SetTextGradient, KM_RefreshHeader, KM_ChatSender, KM_AddInput, KM_FrameHook, KM_HomeScreenInit, KM_CalcScale, KM_Predict, KM_SetXY, KM_SpriteAddChild, KM_SetText, KM_LoginScreenBuild, KM_LoginScreenPress, KM_ScreenManagerUpdate, KM_SettingsPopupCtor, KM_LoadAsset, KM_GetMovieClip };
static const uint32_t KM_RVAS[KM_NHOOKS] = {0x09b900, 0x09bde8, 0x09be50, 0x09be64, 0x09be7c, 0x095dec, 0x0c8c40, 0x0d246c, 0x1db228, 0x223ea8, 0x231ba4, 0x2343ac, 0x432508, 0xbbfc7c, 0xbd2bec, 0xbef364, 0x31bacc, 0x31c1d4, 0x315cc0, 0x307494, 0x008ab4, 0xb88b54};
extern void km_tramp_0(void); extern void km_leave_0(void);
extern void km_tramp_1(void); extern void km_leave_1(void);
extern void km_tramp_2(void); extern void km_leave_2(void);
extern void km_tramp_3(void); extern void km_leave_3(void);
extern void km_tramp_4(void); extern void km_leave_4(void);
extern void km_tramp_5(void); extern void km_leave_5(void);
extern void km_tramp_6(void); extern void km_leave_6(void);
extern void km_tramp_7(void); extern void km_leave_7(void);
extern void km_tramp_8(void); extern void km_leave_8(void);
extern void km_tramp_9(void); extern void km_leave_9(void);
extern void km_tramp_10(void); extern void km_leave_10(void);
extern void km_tramp_11(void); extern void km_leave_11(void);
extern void km_tramp_12(void); extern void km_leave_12(void);
extern void km_tramp_13(void); extern void km_leave_13(void);
extern void km_tramp_14(void); extern void km_leave_14(void);
extern void km_tramp_15(void); extern void km_leave_15(void);
extern void km_tramp_16(void); extern void km_leave_16(void);
extern void km_tramp_17(void); extern void km_leave_17(void);
extern void km_tramp_18(void); extern void km_leave_18(void);
extern void km_tramp_19(void); extern void km_leave_19(void);
extern void km_tramp_20(void); extern void km_leave_20(void);
extern void km_tramp_21(void); extern void km_leave_21(void);
static void *const KM_TRAMPS[KM_NHOOKS] = {km_tramp_0, km_tramp_1, km_tramp_2, km_tramp_3, km_tramp_4, km_tramp_5, km_tramp_6, km_tramp_7, km_tramp_8, km_tramp_9, km_tramp_10, km_tramp_11, km_tramp_12, km_tramp_13, km_tramp_14, km_tramp_15, km_tramp_16, km_tramp_17, km_tramp_18, km_tramp_19, km_tramp_20, km_tramp_21};
static void *const KM_LEAVES[KM_NHOOKS] = {km_leave_0, km_leave_1, km_leave_2, km_leave_3, km_leave_4, km_leave_5, km_leave_6, km_leave_7, km_leave_8, km_leave_9, km_leave_10, km_leave_11, km_leave_12, km_leave_13, km_leave_14, km_leave_15, km_leave_16, km_leave_17, km_leave_18, km_leave_19, km_leave_20, km_leave_21};
