#ifndef RACE_PLAYER_UPDATE_H
#define RACE_PLAYER_UPDATE_H

#include "common.h"
#include "game/race/player/race_player_input.h"

#define RACE_PLAYER_BOARD_TUNING_COUNT 15
#define RACE_PLAYER_CHARACTER_TUNING_COUNT 7
#define RACE_PLAYER_DEMO_CHARACTER_TUNING_COUNT 6

typedef struct RacePlayerTuning {
    s16 maxSpeed;
    s16 gravity;
    s16 aerialGravity;
    s16 turnStrength;
    s16 lateralDeceleration;
    s16 turnRadiusAtFullLean;
    s16 turnRadiusAtHalfLean;
    s16 forwardDeceleration;
    s16 reverseDeceleration;
} RacePlayerTuning;

typedef char RacePlayerTuningSizeCheck[(sizeof(RacePlayerTuning) == 0x12) ? 1 : -1];

extern RacePlayerTuning gRacePlayerBoardTuning[RACE_PLAYER_BOARD_TUNING_COUNT];
extern RacePlayerTuning gRacePlayerCharacterTuning[RACE_PLAYER_CHARACTER_TUNING_COUNT];
extern RacePlayerTuning gRacePlayerDemoBoardTuning[RACE_PLAYER_BOARD_TUNING_COUNT];
extern RacePlayerTuning gRacePlayerDemoCharacterTuning[RACE_PLAYER_DEMO_CHARACTER_TUNING_COUNT];

void initRacePlayers(void);
void applyRacePlayerTuning(RacePlayer *player);
void initRacePlayer(RacePlayer *player);
void updateRacePlayers(void);
void updateRacePlayer(RacePlayer *player);
void updateRacePlayerMotionFeedback(RacePlayer *player);
void updateRacePlayerMode00Grounded(RacePlayer *player);
void updateRacePlayerMode29Crash(RacePlayer *player);
void updateRacePlayerMode01JumpStart(RacePlayer *player);
void updateRacePlayerMode22Airborne(RacePlayer *player);
void dispatchRacePlayerAirborneMode(RacePlayer *player);
void updateRacePlayerAirborneLaunch(RacePlayer *player);
void updateRacePlayerAirborneCruise(RacePlayer *player);
void resetRacePlayerTrickSubstate(RacePlayer *player);
void updateRacePlayerTrickSubstate(RacePlayer *player);
void updateRacePlayerTrickSubstateStart(RacePlayer *player);
void updateRacePlayerTrickSubstateHold(RacePlayer *player);
void updateRacePlayerTrickSubstateFinish(RacePlayer *player);
void updateRacePlayerTrickSubstateNoop(RacePlayer *player);
void updateRacePlayerMode13AerialTrick(RacePlayer *player);
void updateRacePlayerMode15AerialTrick(RacePlayer *player);
void updateRacePlayerMode16AerialTrick(RacePlayer *player);
void updateRacePlayerMode17AerialTrick(RacePlayer *player);
void updateRacePlayerMode18AerialTrick(RacePlayer *player);
void updateRacePlayerMode19AerialTrick(RacePlayer *player);
void updateRacePlayerMode20AerialTrick(RacePlayer *player);
void updateRacePlayerMode21AerialTrick(RacePlayer *player);
void updateRacePlayerMode31AerialTrick(RacePlayer *player);
void updateRacePlayerMode41AerialTrick(RacePlayer *player);
void updateRacePlayerMode42AerialTrick(RacePlayer *player);
void updateRacePlayerMode43AerialTrick(RacePlayer *player);
void updateRacePlayerMode32AerialTrick(RacePlayer *player);
void updateRacePlayerMode33AerialTrick(RacePlayer *player);
void updateRacePlayerMode34AerialTrick(RacePlayer *player);
void updateRacePlayerMode44AerialTrick(RacePlayer *player);
void updateRacePlayerMode45AerialTrick(RacePlayer *player);
void updateRacePlayerMode46AerialTrick(RacePlayer *player);
void updateRacePlayerMode35AerialTrick(RacePlayer *player);
void updateRacePlayerMode36AerialTrick(RacePlayer *player);
void updateRacePlayerMode47AerialTrick(RacePlayer *player);
void updateRacePlayerMode48AerialTrick(RacePlayer *player);
void updateRacePlayerMode49AerialTrick(RacePlayer *player);
void updateRacePlayerMode37AerialTrick(RacePlayer *player);
void updateRacePlayerMode50AerialTrick(RacePlayer *player);
void updateRacePlayerMode51AerialTrick(RacePlayer *player);
void updateRacePlayerMode52AerialTrick(RacePlayer *player);
void updateRacePlayerMode38AerialTrick(RacePlayer *player);
void updateRacePlayerMode53AerialTrick(RacePlayer *player);
void updateRacePlayerMode54AerialTrick(RacePlayer *player);
void updateRacePlayerMode39AerialTrick(RacePlayer *player);
void updateRacePlayerMode55AerialTrick(RacePlayer *player);
void updateRacePlayerMode56AerialTrick(RacePlayer *player);
void updateRacePlayerMode57AerialTrick(RacePlayer *player);
void updateRacePlayerMode03Nudge(RacePlayer *player);
void updateRacePlayerMode04Spinout(RacePlayer *player);
void updateRacePlayerMode05SpinoutStun(RacePlayer *player);
void updateRacePlayerMode08SpinoutRecover(RacePlayer *player);
void updateRacePlayerMode06TerrainFall(RacePlayer *player);
void updateRacePlayerMode28TerrainFallWithItemEffect(RacePlayer *player);
void updateRacePlayerMode09TerrainCrash(RacePlayer *player);
void updateRacePlayerMode10TerrainCrashSlide(RacePlayer *player);
void updateRacePlayerMode12LaunchRecover(RacePlayer *player);
void updateRacePlayerMode11LaunchHit(RacePlayer *player);
void updateRacePlayerMode26Tumble(RacePlayer *player);
void updateRacePlayerMode14PushHit(RacePlayer *player);
void updateRacePlayerMode23ItemSteal(RacePlayer *player);
void updateRacePlayerMode24HeavyKnockdown(RacePlayer *player);
void updateRacePlayerMode25SpinHit(RacePlayer *player);
void updateRacePlayerMode27Slide(RacePlayer *player);
void updateRacePlayerMode40Stun(RacePlayer *player);
void dispatchRacePlayerMode07CourseObject(RacePlayer *player);
void updateRacePlayerMode07AlignToLaunchRamp(RacePlayer *player);
void updateRacePlayerMode07StartLaunchRamp(RacePlayer *player);
void updateRacePlayerMode07LaunchRampTakeoff(RacePlayer *player);
void updateRacePlayerMode07LaunchRampSpin(RacePlayer *player);
void updateRacePlayerMode07LaunchRampPose(RacePlayer *player);
void updateRacePlayerMode07LaunchRampDrift(RacePlayer *player);
void updateRacePlayerMode07LaunchRampClimb(RacePlayer *player);
void updateRacePlayerMode07SpiralExit(RacePlayer *player);
s32 tryStartRacePlayerCourseObjectMode(s16 arg0);
void dispatchRacePlayerMode30Attack(RacePlayer *player);
void updateRacePlayerMode30AttackApproach(RacePlayer *player);
void updateRacePlayerMode30AttackResolve(RacePlayer *player);
void dispatchRacePlayerMode32Character(RacePlayer *player);
void updateRacePlayerMode32Character0(RacePlayer *player);
void updateRacePlayerMode32Character1(RacePlayer *player);
void updateRacePlayerMode32Character2(RacePlayer *player);
void updateRacePlayerMode32Character3(RacePlayer *player);
void updateRacePlayerMode32Character4(RacePlayer *player);
void updateRacePlayerMode32Character5(RacePlayer *player);
void dispatchRacePlayerMode35Character(RacePlayer *player);
void updateRacePlayerMode35Character0(RacePlayer *player);
void updateRacePlayerMode35Character1(RacePlayer *player);
void updateRacePlayerMode35Character2(RacePlayer *player);
void updateRacePlayerMode35Character3(RacePlayer *player);
void updateRacePlayerMode35Character4(RacePlayer *player);
void updateRacePlayerMode35Character5(RacePlayer *player);
void updateRacePlayersPostUpdate(void);
void updateRacePlayerPostUpdate(RacePlayer *player);
void updateRacePlayerPostUpdateNoop(RacePlayer *player);
void updateRacePlayerPostUpdateCourseObject(RacePlayer *player);
void updateRacePlayerPostUpdateMode00(RacePlayer *player);
void updateRacePlayerVoiceSounds(RacePlayer *player);
void updateRacePlayerLoopingSound(RacePlayer *player);
void updateRacePlayerPostUpdateMode22(RacePlayer *player);
void updateRacePlayerPostUpdateAirborneTrick(RacePlayer *player);
void updateRacePlayerPostUpdateAirborneLaunch(RacePlayer *player);
void updateRacePlayerPostUpdateNudge(RacePlayer *player);
void updateRacePlayerPostUpdateMode29(RacePlayer *player);
void updateRacePlayerPostUpdateAttack(RacePlayer *player);

#endif
