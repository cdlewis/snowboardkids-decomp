#include "assets.h"
#include "game/demo/main_menu_demo_race_intro_data.h"
#include "game/race/race_state.h"
#include "game/race/player/character_model.h"

/* Keep zero-initialized data in the original ROM section. */
u8 gMainMenuDemoRaceIntroIndex __attribute__((section(".data"))) = 0;
MainMenuDemoRaceIntroEntry gMainMenuDemoRaceIntroEntries[] = {
    {
        .courseIndex = RACE_COURSE_ROOKIE_MOUNTAIN,
        .splitscreen = 0,
        .raceType = 0,
        .loadCourseAssets = 0,
        .romStart = (void *)&MAIN_MENU_DEMO_ROOKIE_MOUNTAIN_REPLAY_ROM_START,
        .romEnd = (void *)&MAIN_MENU_DEMO_ROOKIE_MOUNTAIN_REPLAY_ROM_END,
        .player0Character = RACE_CHARACTER_SLASH,
        .player1Character = RACE_CHARACTER_NANCY,
        .player2Character = RACE_CHARACTER_JAM,
        .player3Character = RACE_CHARACTER_LINDA,
    },
    {
        .courseIndex = RACE_COURSE_BIG_SNOWMAN,
        .splitscreen = 1,
        .raceType = 0,
        .loadCourseAssets = 0,
        .romStart = (void *)&MAIN_MENU_DEMO_BIG_SNOWMAN_REPLAY_ROM_START,
        .romEnd = (void *)&MAIN_MENU_DEMO_BIG_SNOWMAN_REPLAY_ROM_END,
        .player0Character = RACE_CHARACTER_TOMMY,
        .player1Character = RACE_CHARACTER_NANCY,
        .player2Character = RACE_CHARACTER_JAM,
        .player3Character = RACE_CHARACTER_LINDA,
    },
    {
        .courseIndex = RACE_COURSE_SUNSET_ROCK,
        .splitscreen = 1,
        .raceType = 1,
        .loadCourseAssets = 0,
        .romStart = (void *)&MAIN_MENU_DEMO_SUNSET_ROCK_REPLAY_ROM_START,
        .romEnd = (void *)&MAIN_MENU_DEMO_SUNSET_ROCK_REPLAY_ROM_END,
        .player0Character = RACE_CHARACTER_JAM,
        .player1Character = RACE_CHARACTER_SLASH,
        .player2Character = RACE_CHARACTER_NANCY,
        .player3Character = RACE_CHARACTER_LINDA,
    },
    {
        .courseIndex = RACE_COURSE_ANIMAL_LAND,
        .splitscreen = 1,
        .raceType = 2,
        .loadCourseAssets = 0,
        .romStart = (void *)&MAIN_MENU_DEMO_ANIMAL_LAND_REPLAY_ROM_START,
        .romEnd = (void *)&MAIN_MENU_DEMO_ANIMAL_LAND_REPLAY_ROM_END,
        .player0Character = RACE_CHARACTER_NANCY,
        .player1Character = RACE_CHARACTER_SLASH,
        .player2Character = RACE_CHARACTER_NANCY,
        .player3Character = RACE_CHARACTER_LINDA,
    },
    {
        .courseIndex = RACE_COURSE_GRASS_VALLEY,
        .splitscreen = 0,
        .raceType = 0,
        .loadCourseAssets = 1,
        .romStart = (void *)&MAIN_MENU_DEMO_GRASS_VALLEY_REPLAY_ROM_START,
        .romEnd = (void *)&MAIN_MENU_DEMO_GRASS_VALLEY_REPLAY_ROM_END,
        .player0Character = RACE_CHARACTER_LINDA,
        .player1Character = RACE_CHARACTER_NANCY,
        .player2Character = RACE_CHARACTER_TOMMY,
        .player3Character = RACE_CHARACTER_JAM,
    },
};
