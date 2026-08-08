#include "../../include/battle.h"
#include "../../include/battle_controller_player.h"
#include "../../include/cheats.h"
#include "../../include/constants/battle_message_constants.h"
#include "../../include/constants/battle_script_constants.h"
#include "../../include/constants/file.h"

/**
 *  @brief cheat: while gCheatConfig.fleeTrainer is set, holding L+R when a
 *         trainer-battle turn starts runs the genuine escape sub-sequence
 *         (SUB_SEQ_ESCAPE) -- the exact script a successful wild flee runs,
 *         from the same quiescent state (every selection menu already closed).
 *         The vanilla block only exists in the selection UI, which this
 *         bypasses entirely.
 */
static void overrideTrainerEscape(struct BattleSystem *bsys, struct BattleStruct *ctx)
{
    u32 fight_type;

    if (!gCheatConfig.fleeTrainer)
        return;

    if (ctx->server_seq_no != CONTROLLER_COMMAND_CALC_EXECUTION_ORDER)
        return;

    if ((PAD_Read() & (PAD_BUTTON_L | PAD_BUTTON_R)) != (PAD_BUTTON_L | PAD_BUTTON_R))
        return;

    fight_type = BattleTypeGet(bsys);
    if (!(fight_type & BATTLE_TYPE_TRAINER) || (fight_type & BATTLE_TYPE_WIRELESS))
        return;

    if (BattleSystem_GetBattleOutcomeFlags(bsys))
        return;

    LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_ESCAPE);
    ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
    ctx->next_server_seq_no = CONTROLLER_COMMAND_44;
}

#if defined (DISABLE_ITEMS_IN_TRAINER_BATTLE)
void overrideItemUsage(struct BattleSystem *bsys, struct BattleStruct *ctx)
{
    MESSAGE_PARAM mp;
    int battlerId;
    u32 fight_type = BattleTypeGet(bsys);

    for (battlerId = 0; battlerId < bsys->maxBattlers; battlerId++)
    {
        if (ctx->playerActions[battlerId][0] == CONTROLLER_COMMAND_ITEM_INPUT && ctx->com_seq_no[battlerId] == 7)
        {
            if (fight_type & BATTLE_TYPE_TRAINER)
            {
                mp.msg_id = BATTLE_MSG_ITEMS_CANT_BE_USED_HERE; //msg.id  = msg_0197_00593; // Items can't be used here
                mp.msg_tag = TAG_NONE;
                ov12_022639B8(bsys, battlerId, mp);
                ctx->com_seq_no[battlerId] = SSI_STATE_15;
                ctx->ret_seq_no[battlerId] = SSI_STATE_SELECT_COMMAND_INIT;
            }
        }
    }
}
#endif

BOOL LONG_CALL BattleContext_Main(struct BattleSystem *bsys, struct BattleStruct *ctx)
{
    overrideTrainerEscape(bsys, ctx);

    if (!ctx->fight_end_flag)
    {
        if (BattleSystem_GetBattleOutcomeFlags(bsys) && !(BattleSystem_GetBattleOutcomeFlags(bsys) & 0x40))
        {
            ctx->server_seq_no = CONTROLLER_COMMAND_42;
        }
    }

    sPlayerBattleCommands[ctx->server_seq_no](bsys, ctx);
#if defined (DISABLE_ITEMS_IN_TRAINER_BATTLE)
    overrideItemUsage(bsys, ctx);
#endif

    if (ctx->server_seq_no == CONTROLLER_COMMAND_45)
    {
        return TRUE;
    }
    return FALSE;
}
