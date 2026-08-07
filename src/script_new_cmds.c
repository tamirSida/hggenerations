#include "../include/types.h"
#include "../include/script.h"
#include "../include/repel.h"
#include "../include/save.h"
#include "../include/pokemon.h"
#include "../include/cheats.h"
#include "../include/constants/file.h"
#include "../include/constants/species.h"

#define SCRIPT_NEW_CMD_REPEL_USE            0
#define SCRIPT_NEW_CMD_TUNER_PREP           1
#define SCRIPT_NEW_CMD_TUNER_PARSE_SPECIES  2
#define SCRIPT_NEW_CMD_TUNER_PARSE_LEVEL    3
#define SCRIPT_NEW_CMD_TUNER_PARSE_GENDER   4
#define SCRIPT_NEW_CMD_TUNER_COMMIT         5

#define SCRIPT_NEW_CMD_MAX          256

// gen 4 text encoding: '0'..'9' are 0x0121..0x012A, EOS is 0xFFFF
#define CHAR_DIGIT_FIRST 0x0121
#define CHAR_DIGIT_LAST  0x012A
#define CHAR_EOS         0xFFFF

#define TUNER_PARSE_INVALID 0xFFFF

// scratch state for the Encounter Tuner input flow (valid only within script 2073)
static u16 ALIGN4 sTunerNickBackup[12];
static u32 ALIGN4 sTunerHasNickBackup = 0;
static u32 ALIGN4 sTunerSlot = 0;
static u16 ALIGN4 sTunerStagedSpecies = 0;
static u16 ALIGN4 sTunerStagedLevel = 0;
static u16 ALIGN4 sTunerStagedGender = 0;

/**
 *  @brief find the first non-egg party member, back its nickname up, and
 *         return the slot so the script can hand it to the naming screen
 */
static u16 TunerPrep(FieldSystem *fsys)
{
    struct Party *party = SaveData_GetPlayerPartyPtr(fsys->savedata);

    for (int i = 0; i < 6; i++)
    {
        struct PartyPokemon *mon = Party_GetMonByIndex(party, i);
        if (mon == NULL)
            continue;
        if (GetMonData(mon, MON_DATA_SPECIES, NULL) == 0)
            continue;
        if (GetMonData(mon, MON_DATA_IS_EGG, NULL))
            continue;

        GetMonData(mon, MON_DATA_NICKNAME, sTunerNickBackup);
        sTunerHasNickBackup = GetMonData(mon, MON_DATA_HAS_NICKNAME, NULL);
        sTunerSlot = i;
        return (u16)i;
    }

    return 0xFF; // no usable mon (should be impossible in the field)
}

/**
 *  @brief read the digits the player typed on the naming screen out of the
 *         backed-up mon's nickname, then restore its real nickname.
 *  @return the parsed number, or TUNER_PARSE_INVALID if the entry was empty
 *          or contained anything that is not a digit
 */
static u16 TunerParse(FieldSystem *fsys)
{
    u16 typed[12];
    u32 value = 0;
    u32 digits = 0;
    BOOL valid = TRUE;

    struct Party *party = SaveData_GetPlayerPartyPtr(fsys->savedata);
    struct PartyPokemon *mon = Party_GetMonByIndex(party, sTunerSlot);

    GetMonData(mon, MON_DATA_NICKNAME, typed);

    // put the real nickname back before anything else can observe the mon
    SetMonData(mon, MON_DATA_NICKNAME, sTunerNickBackup);
    SetMonData(mon, MON_DATA_HAS_NICKNAME, &sTunerHasNickBackup);

    for (int i = 0; i < 11 && typed[i] != CHAR_EOS; i++)
    {
        if (typed[i] < CHAR_DIGIT_FIRST || typed[i] > CHAR_DIGIT_LAST)
        {
            valid = FALSE;
            break;
        }
        value = value * 10 + (typed[i] - CHAR_DIGIT_FIRST);
        digits++;
        if (value > 0xFFFE) // clamp before it can wrap the u16 return
        {
            value = 0xFFFE;
        }
    }

    if (!valid || digits == 0)
        return TUNER_PARSE_INVALID;

    return (u16)value;
}

BOOL Script_RunNewCmd(SCRIPTCONTEXT *ctx) {
    u8 sw = ScriptReadByte(ctx);
    u16 UNUSED arg0 = ScriptReadHalfword(ctx);

    switch (sw) {
        case SCRIPT_NEW_CMD_REPEL_USE:;
#ifdef IMPLEMENT_REUSABLE_REPELS
            u16 most_recent_repel = Repel_GetMostRecent();
            SetScriptVar(arg0, most_recent_repel);
            Repel_Use(most_recent_repel, HEAPID_MAIN_HEAP);
#endif
            break;

        case SCRIPT_NEW_CMD_TUNER_PREP:
            sTunerStagedSpecies = 0;
            sTunerStagedLevel = 0;
            sTunerStagedGender = 0;
            SetScriptVar(arg0, TunerPrep(ctx->fsys));
            break;

        case SCRIPT_NEW_CMD_TUNER_PARSE_SPECIES:;
            u16 species = TunerParse(ctx->fsys);
            if (species == TUNER_PARSE_INVALID || species == 0 || species > MAX_MON_NUM)
            {
                species = 0; // anything unreadable turns the tuner off
            }
            sTunerStagedSpecies = species;
            SetScriptVar(arg0, species);
            break;

        case SCRIPT_NEW_CMD_TUNER_PARSE_LEVEL:;
            u16 level = TunerParse(ctx->fsys);
            if (level == TUNER_PARSE_INVALID || level > 100)
            {
                level = 0; // unreadable/out of range = keep natural levels
            }
            sTunerStagedLevel = level;
            SetScriptVar(arg0, level);
            break;

        case SCRIPT_NEW_CMD_TUNER_PARSE_GENDER:;
            u16 gender = TunerParse(ctx->fsys);
            if (gender == TUNER_PARSE_INVALID || gender > 2)
            {
                gender = 0; // unreadable = random gender
            }
            sTunerStagedGender = gender;
            SetScriptVar(arg0, gender);
            break;

        case SCRIPT_NEW_CMD_TUNER_COMMIT:
            gCheatConfig.tunerSpecies = sTunerStagedSpecies;
            gCheatConfig.tunerLevel = (u8)sTunerStagedLevel;
            gCheatConfig.tunerGender = (u8)sTunerStagedGender;
            break;

        default: break;
    }

    return FALSE;
}
