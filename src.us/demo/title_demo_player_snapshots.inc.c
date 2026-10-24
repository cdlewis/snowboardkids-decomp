/* Complete RacePlayer state at each demo segment start, grouped by player.
 * Named fields follow RacePlayer declaration order. Captured pointer values
 * and nonzero padding are retained verbatim. Unspecified fields are zero.
 * DEMO_TRANSFORM takes three rotation rows followed by the translation.
 * Render pointers use byte offsets within gRelocatableHeapStart; model matrices
 * form the consecutive allocation runs made by drawRacePlayerModel. */
// clang-format off
{
    /* Player 0. */
    /* Segment 0, frame 0. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(0xB4733F, -0x1755DBB, -0x8FC7FBD),
        .renderPosition = DEMO_VEC3(0xB4733F, -0x1780120, -0x8FC7FBD),
        .previousPosition = DEMO_VEC3(0xB92267, -0x172C6E7, -0x8F12357),
        .velocity = DEMO_VEC3(-0x4AF28, -0x34D63, -0xB5C66),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x17C144A,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0xBC9B,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x285CF,
        .modeValue = 0x40000,
        .stickX = -24,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -381, -210, -926,  -232, 988, -128,  920, 162, -416,
                0xB4733F, -0x1780120, -0x8FC7FBD
            ),
            DEMO_TRANSFORM(
                -669, -252, -731,  -232, 988, -128,  738, 81, -703,
                0xB13FC5, -0x1705A9B, -0x8FCEABF
            ),
            DEMO_TRANSFORM(
                -576, -557, -636,  -640, 789, -112,  551, 333, -793,
                0xB257C5, -0x1707763, -0x8FBC83F
            ),
            DEMO_TRANSFORM(
                -576, -557, -636,  -235, 844, -527,  811, -151, -603,
                0xB437C5, -0x172C7B3, -0x8FB740F
            ),
            DEMO_TRANSFORM(
                -717, 32, -729,  -566, 619, 585,  460, 813, -415,
                0xB061D5, -0x17134F3, -0x8FDED0F
            ),
            DEMO_TRANSFORM(
                -835, -78, -586,  -306, 924, 313,  505, 430, -777,
                0xB20A55, -0x1730593, -0x8FFA3CF
            ),
            DEMO_TRANSFORM(
                -553, -171, -843,  230, 936, -340,  828, -374, -467,
                0xB13FC5, -0x1705A9B, -0x8FCEABF
            ),
            DEMO_TRANSFORM(
                -1000, 132, -171,  206, 821, -573,  62, -595, -829,
                0xB22605, -0x16CB21B, -0x8FE3F3F
            ),
            DEMO_TRANSFORM(
                -217, -872, -489,  -279, 534, -826,  959, -41, -352,
                0xB362DD, -0x16CBC4B, -0x8FC0557
            ),
            DEMO_TRANSFORM(
                -86, -984, -268,  -727, 247, -675,  714, 133, -720,
                0xB4D601, -0x16CFCEB, -0x8F8099F
            ),
            DEMO_TRANSFORM(
                -399, 659, -672,  472, 772, 476,  814, -124, -606,
                0xB0AF9D, -0x16D920B, -0x9002407
            ),
            DEMO_TRANSFORM(
                -236, 808, -580,  330, 627, 738,  939, -17, -406,
                0xAE2CE5, -0x16F1CC3, -0x903335B
            ),
            DEMO_TRANSFORM(
                -381, -210, -926,  -232, 988, -128,  920, 162, -416,
                0xB5301F, -0x174DEE8, -0x8F9A5F5
            ),
            DEMO_TRANSFORM(
                -381, -210, -926,  -232, 988, -128,  920, 162, -416,
                0xB2940F, -0x1764FB8, -0x8FFFA75
            ),
        },
        .localVelocityZ = -0xC8FA1,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3961,
        .modelBaseYaw = 0x30FF,
        .modelBaseRoll = 106,
        .projectedPos = DEMO_VEC3(0xB3827F, -0x1699FBB, -0x8FEC7FD),
        .movementSpeed = 0xC49E9,
        .grabInputFrames = 26,
        .collisionVelocityX = -0x4AF28,
        .collisionVelocityZ = -0xB5C66,
        .pitchAngle = 3961,
        .facingAngle = 0x30FF,
        .leanAngle = 106,
        .surfacePitchAngle = -135,
        .surfaceRollAngle = -21,
        .leanInput = 384,
        .leanScale = 32,
        .modeAngle = 109,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 101, 3196, 230, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 203, 3763, 284, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 1051, 3602, 3191, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 759, 3780, 3162, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3293, 3670, 1039, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3087, 3801, 1132, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 1526,
        .animationFrameDataOffset = 1526,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(0xACA603, -0x17B7C74, -0x9041A47),
            DEMO_VEC3(0xB46257, -0x1773057, -0x8F156A7),
            DEMO_VEC3(0xB48427, -0x17BD53E, -0x907A8D3),
            DEMO_VEC3(0xBC407B, -0x1778921, -0x8F4E533),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141392),
        .shadowMtx = DEMO_HEAP_MATRIX(141392 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(0xACA603, -0x17AD4F2, -0x9041A47),
            DEMO_VEC3(0xB46257, -0x1768FAE, -0x8F156A7),
            DEMO_VEC3(0xB48427, -0x1797292, -0x907A8D3),
            DEMO_VEC3(0xBC407B, -0x1752D4E, -0x8F4E533),
            DEMO_VEC3(0xB17039, -0x1783C9B, -0x8F8D92B),
            DEMO_VEC3(0xB8EA69, -0x173F757, -0x8E5FA4F),
            DEMO_VEC3(0xB95A65, -0x176DA3B, -0x8FC4C5F),
            DEMO_VEC3(0xC0D495, -0x17294F7, -0x8E96D83),
        },
        .groundContactMask = 15,
        .coursePathIndex = 8,
        .coursePathOffset = 0x6D64BE,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135376),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 190,
    } },
    /* Segment 1, frame 57. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x23B3CB5, -0x2B2599A, -0xD4513DE),
        .renderPosition = DEMO_VEC3(-0x23B3CB5, -0x2B251FB, -0xD4513DE),
        .previousPosition = DEMO_VEC3(-0x232B8BD, -0x2B09FFB, -0xD3C0664),
        .velocity = DEMO_VEC3(-0x883F8, -0x27ADF, -0x90D7A),
        .groundOffset = 0xE63C,
        .collisionBottomY = -0x2B91ADA,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x4DDB,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x1BE72,
        .modeValue = 0x40000,
        .stickX = -14,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -694, -147, -737,  -168, 1008, -42,  733, 93, -708,
                -0x23B3CB5, -0x2B251FB, -0xD4513DE
            ),
            DEMO_TRANSFORM(
                -900, -169, -456,  -168, 1008, -42,  456, 37, -915,
                -0x23DA848, -0x2AC17D7, -0xD440AB3
            ),
            DEMO_TRANSFORM(
                -726, -618, -370,  -718, 580, 439,  -55, 571, -846,
                -0x23C4198, -0x2AC5647, -0xD435A53
            ),
            DEMO_TRANSFORM(
                -726, -618, -370,  -215, 687, -726,  687, -437, -618,
                -0x23A26B8, -0x2AE09D7, -0xD44A3C3
            ),
            DEMO_TRANSFORM(
                -906, 207, -427,  -417, 93, 929,  227, 997, 2,
                -0x23EE4C8, -0x2ACD597, -0xD44B083
            ),
            DEMO_TRANSFORM(
                -988, 13, -262,  -92, 938, 398,  246, 408, -905,
                -0x23DABD8, -0x2AD1B57, -0xD476973
            ),
            DEMO_TRANSFORM(
                -906, -97, -464,  94, 944, -380,  464, -380, -827,
                -0x23DA848, -0x2AC17D7, -0xD440AB3
            ),
            DEMO_TRANSFORM(
                -970, 176, 272,  -6, 847, -572,  -324, -544, -802,
                -0x23D49C8, -0x2A86717, -0xD458773
            ),
            DEMO_TRANSFORM(
                -565, -779, -347,  -633, 662, -454,  570, -36, -848,
                -0x23B1F08, -0x2A8A08F, -0xD443563
            ),
            DEMO_TRANSFORM(
                -335, -942, -213,  -939, 370, -161,  226, 142, -987,
                -0x237B260, -0x2A9878F, -0xD41DDCB
            ),
            DEMO_TRANSFORM(
                -808, 558, -286,  558, 852, 85,  285, -88, -978,
                -0x23F8C28, -0x2A919CF, -0xD467A53
            ),
            DEMO_TRANSFORM(
                -480, 835, -343,  723, 588, 420,  541, -44, -867,
                -0x2432BFC, -0x2AB2207, -0xD47597F
            ),
            DEMO_TRANSFORM(
                -694, -147, -737,  -168, 1008, -42,  733, 93, -708,
                -0x23946ED, -0x2AF5AAB, -0xD42A8BE
            ),
            DEMO_TRANSFORM(
                -694, -147, -737,  -168, 1008, -42,  733, 93, -708,
                -0x23E056D, -0x2B05C5B, -0xD47B3CE
            ),
        },
        .localVelocityZ = -0xC8FED,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 4002,
        .modelBaseYaw = 0x31EC,
        .modelBaseRoll = 60,
        .projectedPos = DEMO_VEC3(-0x23C6AF5, -0x2A679DA, -0xD4654DE),
        .movementSpeed = 0xC6DA9,
        .grabInputFrames = 26,
        .collisionVelocityX = -0x883F8,
        .collisionVelocityZ = -0x90D7A,
        .pitchAngle = 4002,
        .facingAngle = 0x31EC,
        .leanAngle = 60,
        .surfacePitchAngle = -94,
        .surfaceRollAngle = -4088,
        .leanInput = 224,
        .leanScale = 32,
        .modeAngle = 63,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x67EC0, -0x1E2C0),
            DEMO_MODEL_PART(1, 4, 100, 3459, 3459, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 976, 3459, 3459, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2653, 3405, 547, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 143, 3296, 140, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 212, 3809, 232, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 806, 3564, 3296, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 597, 3751, 3205, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3561, 3627, 735, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3263, 3797, 1002, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 1526,
        .animationFrameDataOffset = 1526,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x245667D, -0x2B4DA11, -0xD498708),
            DEMO_VEC3(-0x237565D, -0x2B1DA29, -0xD3A93F4),
            DEMO_VEC3(-0x23F230D, -0x2B4C203, -0xD4F93C8),
            DEMO_VEC3(-0x23112ED, -0x2B1C21A, -0xD40A0B4),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(108624),
        .shadowMtx = DEMO_HEAP_MATRIX(108624 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x245667D, -0x2B435A1, -0xD498708),
            DEMO_VEC3(-0x237565D, -0x2B139C5, -0xD3A93F4),
            DEMO_VEC3(-0x23F230D, -0x2B36A31, -0xD4F93C8),
            DEMO_VEC3(-0x23112ED, -0x2B06E55, -0xD40A0B4),
            DEMO_VEC3(-0x23CD9E5, -0x2B26631, -0xD408DD6),
            DEMO_VEC3(-0x22EE799, -0x2AF6A55, -0xD317E3A),
            DEMO_VEC3(-0x23689E1, -0x2B19AC1, -0xD468E8E),
            DEMO_VEC3(-0x2289795, -0x2AE9EE5, -0xD377EF2),
        },
        .groundContactMask = 15,
        .coursePathIndex = 12,
        .coursePathOffset = 0x517FEC,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(102608),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = 0.0f,
        .replayFrame = 299,
    } },
    /* Segment 2, frame 108. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xAF6A5A9, -0x85F7C45, -0x17A6B734),
        .renderPosition = DEMO_VEC3(-0xAF6A5A9, -0x862D92F, -0x17A6B734),
        .previousPosition = DEMO_VEC3(-0xAEFB782, -0x85B2F2D, -0x179D28BB),
        .velocity = DEMO_VEC3(-0x6EE27, -0x50259, -0x98E79),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x8663186,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 790,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x439A6,
        .modeValue = 0x40000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -564, -349, -778,  -205, 962, -282,  828, 0, -601,
                -0xAF6A5A9, -0x862D92F, -0x17A6B734
            ),
            DEMO_TRANSFORM(
                -810, -329, -530,  -205, 962, -282,  590, -117, -828,
                -0xAF97EFF, -0x85B2B10, -0x17A8178E
            ),
            DEMO_TRANSFORM(
                -715, -617, -393,  -611, 806, -152,  402, 128, -931,
                -0xAF83557, -0x85B2800, -0x17A72D2E
            ),
            DEMO_TRANSFORM(
                -715, -617, -393,  -292, 745, -636,  670, -332, -697,
                -0xAF66B17, -0x85D84B0, -0x17A6BACE
            ),
            DEMO_TRANSFORM(
                -843, -45, -577,  -410, 766, 539,  409, 677, -649,
                -0xAFA9567, -0x85C1EC0, -0x17A8BB3E
            ),
            DEMO_TRANSFORM(
                -936, -116, -396,  -194, 990, 168,  364, 230, -928,
                -0xAF96157, -0x85E5D40, -0x17AA500E
            ),
            DEMO_TRANSFORM(
                -920, -341, -288,  -7, 672, -771,  446, -691, -607,
                -0xAF97EFF, -0x85B2B10, -0x17A8178E
            ),
            DEMO_TRANSFORM(
                -987, 133, 233,  -87, 680, -758,  -255, -752, -644,
                -0xAF9867F, -0x8588A90, -0x17AB1A8E
            ),
            DEMO_TRANSFORM(
                -608, -804, -171,  -738, 628, -325,  361, -69, -954,
                -0xAF74617, -0x8580930, -0x17AA0606
            ),
            DEMO_TRANSFORM(
                -334, -967, 13,  -966, 334, -12,  8, -17, -1022,
                -0xAF3615F, -0x858C3CC, -0x17A87F76
            ),
            DEMO_TRANSFORM(
                -971, -9, -320,  74, 987, -256,  310, -266, -937,
                -0xAFBC507, -0x859B410, -0x17AB6E56
            ),
            DEMO_TRANSFORM(
                -697, 626, -409,  616, 798, 173,  425, -128, -921,
                -0xAFDF0FB, -0x85D56D4, -0x17AB1E56
            ),
            DEMO_TRANSFORM(
                -564, -349, -778,  -205, 962, -282,  828, 0, -601,
                -0xAF537A1, -0x85F4DCF, -0x17A4BE84
            ),
            DEMO_TRANSFORM(
                -564, -349, -778,  -205, 962, -282,  828, 0, -601,
                -0xAF913F1, -0x861B16F, -0x17AA1154
            ),
        },
        .localVelocityZ = -0xC9000,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = 0x2199,
        .projectedPos = DEMO_VEC3(-0xAF90CA9, -0x85434C5, -0x17AA0774),
        .movementSpeed = 0xBCE0F,
        .grabInputFrames = 20,
        .collisionVelocityX = -0x6EE27,
        .collisionVelocityZ = -0x98E79,
        .pitchAngle = 3869,
        .facingAngle = 0x2199,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -2,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 440, 3464, 4032, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 272, 3888, 176, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 440, 3568, 3456, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 328, 3760, 3280, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3912, 3528, 280, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3464, 3776, 784, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 922,
        .animationFrameCount = 20,
        .animationFramesRemaining = 9,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xAFFE8ED, -0x8670960, -0x17AC09BE),
            DEMO_VEC3(-0xAF47749, -0x85FEF2B, -0x179C41FA),
            DEMO_VEC3(-0xAF8D409, -0x8671304, -0x17B12C6E),
            DEMO_VEC3(-0xAED6265, -0x85FF8CE, -0x17A164AA),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141328),
        .shadowMtx = DEMO_HEAP_MATRIX(141328 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xAFFE8ED, -0x86663B3, -0x17AC09BE),
            DEMO_VEC3(-0xAF47749, -0x85F4EAB, -0x179C41FA),
            DEMO_VEC3(-0xAF8D409, -0x86663B3, -0x17B12C6E),
            DEMO_VEC3(-0xAED6265, -0x85F4EAB, -0x17A164AA),
            DEMO_VEC3(-0xAF8FAC6, -0x862169B, -0x17A27B45),
            DEMO_VEC3(-0xAED8922, -0x85B0193, -0x1792B381),
            DEMO_VEC3(-0xAF1E5E2, -0x862169B, -0x17A79DF5),
            DEMO_VEC3(-0xAE6743E, -0x85B0193, -0x1797D631),
        },
        .groundContactMask = 15,
        .coursePathIndex = 27,
        .coursePathOffset = 0x17E42CC,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135376),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 604,
    } },
    /* Segment 3, frame 161. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xC3570AB, -0x92556B7, -0x195E50F2),
        .renderPosition = DEMO_VEC3(-0xC3570AB, -0x928B3A1, -0x195E50F2),
        .previousPosition = DEMO_VEC3(-0xC2E8284, -0x921099E, -0x1954C279),
        .velocity = DEMO_VEC3(-0x6EE27, -0x50259, -0x98E79),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x92C0BF7,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 790,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x439A6,
        .modeValue = 0x40000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -564, -349, -778,  -205, 962, -282,  828, 0, -601,
                -0xC3570AB, -0x928B3A1, -0x195E50F2
            ),
            DEMO_TRANSFORM(
                -810, -329, -530,  -205, 962, -282,  590, -117, -828,
                -0xC384A01, -0x9210582, -0x195FB14C
            ),
            DEMO_TRANSFORM(
                -715, -617, -393,  -611, 806, -152,  402, 128, -931,
                -0xC370059, -0x9210272, -0x195EC6EC
            ),
            DEMO_TRANSFORM(
                -715, -617, -393,  -292, 745, -636,  670, -332, -697,
                -0xC353619, -0x9235F22, -0x195E548C
            ),
            DEMO_TRANSFORM(
                -843, -45, -577,  -410, 766, 539,  409, 677, -649,
                -0xC396069, -0x921F932, -0x196054FC
            ),
            DEMO_TRANSFORM(
                -936, -116, -396,  -194, 990, 168,  364, 230, -928,
                -0xC382C59, -0x92437B2, -0x1961E9CC
            ),
            DEMO_TRANSFORM(
                -918, -301, -334,  -41, 814, -617,  448, -541, -743,
                -0xC384A01, -0x9210582, -0x195FB14C
            ),
            DEMO_TRANSFORM(
                -990, 107, 232,  -90, 724, -716,  -240, -714, -691,
                -0xC3873C1, -0x91DD742, -0x19621ACC
            ),
            DEMO_TRANSFORM(
                -612, -786, -232,  -708, 653, -344,  413, -45, -934,
                -0xC363051, -0x91D809A, -0x1960FC64
            ),
            DEMO_TRANSFORM(
                -339, -965, -40,  -963, 341, -49,  61, 21, -1021,
                -0xC326671, -0x91E5BA6, -0x195F44F0
            ),
            DEMO_TRANSFORM(
                -975, 38, -306,  129, 972, -289,  280, -314, -932,
                -0xC3AACC1, -0x91EF97A, -0x19629ED4
            ),
            DEMO_TRANSFORM(
                -659, 658, -421,  676, 757, 125,  393, -197, -923,
                -0xC3D0DE9, -0x922762E, -0x196228C8
            ),
            DEMO_TRANSFORM(
                -564, -349, -778,  -205, 962, -282,  828, 0, -601,
                -0xC3402A3, -0x9252841, -0x195C5842
            ),
            DEMO_TRANSFORM(
                -564, -349, -778,  -205, 962, -282,  828, 0, -601,
                -0xC37DEF3, -0x9278BE1, -0x1961AB12
            ),
        },
        .localVelocityZ = -0xC9000,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = 0x2199,
        .projectedPos = DEMO_VEC3(-0xC37D7AB, -0x91A0F37, -0x1961A132),
        .movementSpeed = 0xBCE0F,
        .grabInputFrames = 20,
        .collisionVelocityX = -0x6EE27,
        .collisionVelocityZ = -0x98E79,
        .pitchAngle = 3869,
        .facingAngle = 0x2199,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -2,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 264, 3440, 4088, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 240, 3880, 160, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 464, 3528, 3448, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 360, 3728, 3264, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3920, 3552, 312, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3496, 3792, 816, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 1282,
        .animationFrameCount = 20,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xC3EB3EF, -0x92CE3D2, -0x1963A37C),
            DEMO_VEC3(-0xC33424B, -0x925C99C, -0x1953DBB8),
            DEMO_VEC3(-0xC379F0B, -0x92CED75, -0x1968C62C),
            DEMO_VEC3(-0xC2C2D67, -0x925D340, -0x1958FE68),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141328),
        .shadowMtx = DEMO_HEAP_MATRIX(141328 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xC3EB3EF, -0x92C3E25, -0x1963A37C),
            DEMO_VEC3(-0xC33424B, -0x925291D, -0x1953DBB8),
            DEMO_VEC3(-0xC379F0B, -0x92C3E25, -0x1968C62C),
            DEMO_VEC3(-0xC2C2D67, -0x925291D, -0x1958FE68),
            DEMO_VEC3(-0xC37C5C8, -0x927F10C, -0x195A1503),
            DEMO_VEC3(-0xC2C5424, -0x920DC04, -0x194A4D3F),
            DEMO_VEC3(-0xC30B0E4, -0x927F10C, -0x195F37B3),
            DEMO_VEC3(-0xC253F40, -0x920DC04, -0x194F6FEF),
        },
        .groundContactMask = 15,
        .coursePathIndex = 27,
        .coursePathOffset = 0x39D43CF,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135376),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 650,
    } },
    /* Segment 4, frame 210. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xCD4CECC, -0x9886C4A, -0x1A3A13A5),
        .renderPosition = DEMO_VEC3(-0xCD4CECC, -0x98BC969, -0x1A3A13A5),
        .previousPosition = DEMO_VEC3(-0xCCDE3DF, -0x9841171, -0x1A30899F),
        .velocity = DEMO_VEC3(-0x6EAED, -0x51018, -0x98A06),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x98F2189,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 737,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x447C8,
        .modeValue = 0x40000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -563, -354, -777,  -207, 960, -286,  828, 0, -601,
                -0xCD4CECC, -0x98BC969, -0x1A3A13A5
            ),
            DEMO_TRANSFORM(
                -809, -333, -529,  -207, 960, -286,  590, -119, -827,
                -0xCD7ADC0, -0x9841EDD, -0x1A3B7BA8
            ),
            DEMO_TRANSFORM(
                -713, -620, -391,  -613, 804, -155,  401, 125, -932,
                -0xCD66420, -0x98419FD, -0x1A3A9130
            ),
            DEMO_TRANSFORM(
                -713, -620, -391,  -294, 742, -639,  671, -333, -696,
                -0xCD49830, -0x98674FD, -0x1A3A1C90
            ),
            DEMO_TRANSFORM(
                -843, -49, -577,  -412, 766, 537,  407, 675, -652,
                -0xCD8C370, -0x98513ED, -0x1A3C1E80
            ),
            DEMO_TRANSFORM(
                -936, -120, -395,  -197, 990, 165,  363, 227, -929,
                -0xCD78DE0, -0x98752FD, -0x1A3DB140
            ),
            DEMO_TRANSFORM(
                -916, -289, -350,  -53, 853, -562,  451, -485, -779,
                -0xCD7ADC0, -0x9841EDD, -0x1A3B7BA8
            ),
            DEMO_TRANSFORM(
                -994, 100, 221,  -82, 738, -703,  -228, -701, -709,
                -0xCD7E380, -0x980C99D, -0x1A3DADE8
            ),
            DEMO_TRANSFORM(
                -607, -783, -255,  -702, 658, -347,  430, -31, -927,
                -0xCD59FD0, -0x9807F7D, -0x1A3C8C68
            ),
            DEMO_TRANSFORM(
                -347, -960, -59,  -958, 351, -75,  91, 29, -1018,
                -0xCD1DD98, -0x981609D, -0x1A3AC798
            ),
            DEMO_TRANSFORM(
                -979, 55, -290,  146, 964, -308,  257, -337, -931,
                -0xCDA19C0, -0x981E90D, -0x1A3E42D8
            ),
            DEMO_TRANSFORM(
                -653, 658, -431,  685, 751, 109,  387, -218, -921,
                -0xCDC8C9C, -0x9855585, -0x1A3DB300
            ),
            DEMO_TRANSFORM(
                -563, -354, -777,  -207, 960, -286,  828, 0, -601,
                -0xCD3635C, -0x9883B31, -0x1A381E65
            ),
            DEMO_TRANSFORM(
                -563, -354, -777,  -207, 960, -286,  828, 0, -601,
                -0xCD73DEC, -0x98AA6B1, -0x1A3D6F05
            ),
        },
        .localVelocityZ = -0xC9000,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3866,
        .modelBaseYaw = 0x2199,
        .projectedPos = DEMO_VEC3(-0xCD73E0C, -0x97D2A0A, -0x1A3D6F25),
        .movementSpeed = 0xBC890,
        .grabInputFrames = 20,
        .collisionVelocityX = -0x6EAED,
        .collisionVelocityZ = -0x98A06,
        .pitchAngle = 3866,
        .facingAngle = 0x2199,
        .surfacePitchAngle = -230,
        .surfaceRollAngle = -2,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 200, 3432, 16, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 224, 3872, 160, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 472, 3512, 3440, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 376, 3712, 3264, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3928, 3568, 320, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3504, 3792, 824, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 262,
        .animationFrameCount = 20,
        .animationFramesRemaining = 20,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xCDE0F78, -0x99004E5, -0x1A3F62F1),
            DEMO_VEC3(-0xCD2A304, -0x988D3B0, -0x1A2FA1A9),
            DEMO_VEC3(-0xCD6FA94, -0x9900EAC, -0x1A4485A1),
            DEMO_VEC3(-0xCCB8E20, -0x988DD77, -0x1A34C459),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(108560),
        .shadowMtx = DEMO_HEAP_MATRIX(108560 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xCDE0F78, -0x98F5F99, -0x1A3F62F1),
            DEMO_VEC3(-0xCD2A304, -0x9883339, -0x1A2FA1A9),
            DEMO_VEC3(-0xCD6FA94, -0x98F5F99, -0x1A4485A1),
            DEMO_VEC3(-0xCCB8E20, -0x9883339, -0x1A34C459),
            DEMO_VEC3(-0xCD7248B, -0x98B04C0, -0x1A35D8EB),
            DEMO_VEC3(-0xCCBB817, -0x983D860, -0x1A2617A3),
            DEMO_VEC3(-0xCD00FA7, -0x98B04C0, -0x1A3AFB9B),
            DEMO_VEC3(-0xCC4A333, -0x983D860, -0x1A2B3A53),
        },
        .groundContactMask = 15,
        .coursePathIndex = 28,
        .coursePathOffset = 0x181904,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(102608),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 673,
    } },
    /* Segment 5, frame 263. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x527E987, -0x3635D8C, -0x1039454E),
        .renderPosition = DEMO_VEC3(-0x527E987, -0x366B5C1, -0x1039454E),
        .previousPosition = DEMO_VEC3(-0x51F9144, -0x3612414, -0x103025A3),
        .velocity = DEMO_VEC3(-0x85843, -0x2DB88, -0x91FAB),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x369FF9C,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1995,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x22487,
        .modeValue = 0x40000,
        .stickX = -1,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -680, -179, -743,  -118, 1007, -134,  755, -3, -690,
                -0x527E987, -0x366B5C1, -0x1039454E
            ),
            DEMO_TRANSFORM(
                -894, -168, -467,  -118, 1007, -134,  482, -63, -900,
                -0x529F6B7, -0x35EAA13, -0x10395560
            ),
            DEMO_TRANSFORM(
                -820, -480, -379,  -524, 878, 22,  314, 212, -949,
                -0x5289877, -0x35EE8F3, -0x10389540
            ),
            DEMO_TRANSFORM(
                -820, -480, -379,  -267, 851, -499,  549, -301, -808,
                -0x5270F77, -0x3617B73, -0x1038A620
            ),
            DEMO_TRANSFORM(
                -898, 121, -474,  -258, 723, 675,  415, 713, -604,
                -0x52B3757, -0x35F6723, -0x1039F3E0
            ),
            DEMO_TRANSFORM(
                -979, 35, -293,  -57, 972, 312,  290, 315, -928,
                -0x52A7547, -0x3618563, -0x103BEEB0
            ),
            DEMO_TRANSFORM(
                -973, -134, -285,  1, 924, -439,  315, -419, -878,
                -0x529F6B7, -0x35EAA13, -0x10395560
            ),
            DEMO_TRANSFORM(
                -948, 176, 341,  -58, 832, -592,  -380, -568, -760,
                -0x529F4F7, -0x35B0E13, -0x103B0D20
            ),
            DEMO_TRANSFORM(
                -713, -678, -279,  -649, 764, -197,  339, 39, -964,
                -0x52794E7, -0x35B2DAB, -0x103A2390
            ),
            DEMO_TRANSFORM(
                -461, -903, -139,  -912, 455, 64,  5, 153, -1011,
                -0x523CF7B, -0x35CA77F, -0x1038DEC8
            ),
            DEMO_TRANSFORM(
                -983, 225, -171,  256, 972, -186,  120, -222, -990,
                -0x52C5577, -0x35BD57B, -0x103B88C0
            ),
            DEMO_TRANSFORM(
                -599, 782, -273,  780, 647, 139,  279, -126, -975,
                -0x52F30F7, -0x35EF4EF, -0x103B2F2C
            ),
            DEMO_TRANSFORM(
                -680, -179, -743,  -118, 1007, -134,  755, -3, -690,
                -0x525E097, -0x363A2A1, -0x10370ECE
            ),
            DEMO_TRANSFORM(
                -680, -179, -743,  -118, 1007, -134,  755, -3, -690,
                -0x52A8697, -0x364DD31, -0x103C23EE
            ),
        },
        .localVelocityZ = -0xC9000,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3981,
        .modelBaseYaw = 483,
        .modelBaseRoll = -2,
        .projectedPos = DEMO_VEC3(-0x5295547, -0x3578D8C, -0x103AD2CE),
        .movementSpeed = 0xC5D46,
        .grabInputFrames = 26,
        .collisionVelocityX = -0x85843,
        .collisionVelocityZ = -0x91FAB,
        .pitchAngle = 3981,
        .facingAngle = 483,
        .leanAngle = -2,
        .surfacePitchAngle = -115,
        .surfaceRollAngle = -4063,
        .leanInput = 16,
        .leanScale = 32,
        .modeAngle = 4,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 197, 3424, 23, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 224, 3868, 164, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 493, 3515, 3431, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 390, 3714, 3261, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3905, 3571, 346, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3489, 3792, 835, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 1526,
        .animationFrameDataOffset = 1526,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x532079B, -0x369A74A, -0x103DDACC),
            DEMO_VEC3(-0x524401B, -0x365FB68, -0x102EC9E4),
            DEMO_VEC3(-0x52B92F3, -0x3693369, -0x1043C0B8),
            DEMO_VEC3(-0x51DCB73, -0x3658787, -0x1034AFD0),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(108560),
        .shadowMtx = DEMO_HEAP_MATRIX(108560 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x532079B, -0x36884B3, -0x103DDACC),
            DEMO_VEC3(-0x524401B, -0x364E03F, -0x102EC9E4),
            DEMO_VEC3(-0x52B92F3, -0x3688B43, -0x1043C0B8),
            DEMO_VEC3(-0x51DCB73, -0x364E6CF, -0x1034AFD0),
            DEMO_VEC3(-0x529AE1E, -0x3664819, -0x1034BED1),
            DEMO_VEC3(-0x51BEBCE, -0x362A3A5, -0x1025A8B9),
            DEMO_VEC3(-0x52336BA, -0x36651F1, -0x103AA28D),
            DEMO_VEC3(-0x515746A, -0x362AD7D, -0x102B8C75),
        },
        .groundContactMask = 15,
        .coursePathIndex = 18,
        .coursePathOffset = 0x56F1C6,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(102608),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 385,
    } },
    /* Segment 6, frame 312. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x249AA43B, -0x17B1625A, -0x186B2DAE),
        .renderPosition = DEMO_VEC3(-0x249AA43B, -0x17B4BBE5, -0x186B2DAE),
        .previousPosition = DEMO_VEC3(-0x24901686, -0x17AF3546, -0x1871A358),
        .velocity = DEMO_VEC3(-0xA8DB5, -0x2E253, 0x675AA),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x17B81799,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1653,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x21665,
        .modeValue = 1024,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -860, -175, 526,  -149, 1008, 91,  -534, 0, -873,
                -0x249AA43B, -0x17B4BBE5, -0x186B2DAE
            ),
            DEMO_TRANSFORM(
                -629, -165, 789,  -149, 1008, 91,  -792, -59, -645,
                -0x249B0CCC, -0x17ACAF4C, -0x18692C07
            ),
            DEMO_TRANSFORM(
                -530, -477, 733,  -72, 878, 519,  -871, 217, -489,
                -0x249A0DFC, -0x17ACEF84, -0x186A5F97
            ),
            DEMO_TRANSFORM(
                -530, -477, 733,  -536, 854, 168,  -690, -297, -693,
                -0x2499D79C, -0x17AF8264, -0x186BE507
            ),
            DEMO_TRANSFORM(
                -635, 124, 791,  618, 719, 383,  -510, 716, -523,
                -0x249BE64C, -0x17AD6B44, -0x18680F47
            ),
            DEMO_TRANSFORM(
                -559, 222, 827,  438, 923, 48,  -735, 380, -600,
                -0x249DB5FC, -0x17AF8714, -0x18692E87
            ),
            DEMO_TRANSFORM(
                -400, -200, 920,  -660, 772, -119,  -671, -640, -431,
                -0x249B0CCC, -0x17ACAF4C, -0x18692C07
            ),
            DEMO_TRANSFORM(
                181, 200, 987,  -652, 786, -39,  -766, -622, 267,
                -0x249DA10C, -0x17A9AB0C, -0x1869A347
            ),
            DEMO_TRANSFORM(
                -332, -705, 663,  -278, 740, 647,  -926, 29, -432,
                -0x249C5484, -0x17A98E6C, -0x186BD3AF
            ),
            DEMO_TRANSFORM(
                -155, -919, 422,  -30, 430, 927,  -1010, 127, -93,
                -0x249AA90C, -0x17AAE460, -0x186F7E93
            ),
            DEMO_TRANSFORM(
                -385, 136, 938,  -99, 1000, -186,  -942, -161, -363,
                -0x249E4884, -0x17AA88BC, -0x1867550F
            ),
            DEMO_TRANSFORM(
                -358, 747, 600,  333, 698, -669,  -898, -38, -488,
                -0x249EAC38, -0x17ADEEF0, -0x1864D137
            ),
            DEMO_TRANSFORM(
                -860, -175, 526,  -149, 1008, 91,  -534, 0, -873,
                -0x24981103, -0x17B1AC15, -0x186CC156
            ),
            DEMO_TRANSFORM(
                -860, -175, 526,  -149, 1008, 91,  -534, 0, -873,
                -0x249DF203, -0x17B2DEC5, -0x186927F6
            ),
        },
        .localVelocityZ = -0xC9000,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3984,
        .modelBaseYaw = -0x5A9A,
        .projectedPos = DEMO_VEC3(-0x249C63FB, -0x17A5901A, -0x186A1BEE),
        .movementSpeed = 0xC5F9F,
        .grabInputFrames = 2,
        .grabTrickId = 4,
        .grabHoldFrames = 40,
        .aerialTrickId = 8,
        .trickAttackCompletedTrickMask = 0x100,
        .collisionVelocityX = -0xA8DB5,
        .collisionVelocityZ = 0x675AA,
        .pitchAngle = 3984,
        .facingAngle = -0x5A9A,
        .surfacePitchAngle = -112,
        .surfaceRollAngle = -2,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3368, 3448, 480, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 440, 3464, 4032, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 272, 3888, 176, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 440, 3568, 3456, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 328, 3760, 3280, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3912, 3528, 280, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3464, 3776, 784, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 802,
        .animationFrameCount = 20,
        .animationFramesRemaining = 11,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x24A1123F, -0x17B72E51, -0x18621CF4),
            DEMO_VEC3(-0x248FA4FF, -0x17B39662, -0x186CC82C),
            DEMO_VEC3(-0x24A5A377, -0x17B73818, -0x18699330),
            DEMO_VEC3(-0x24943637, -0x17B3A02A, -0x18743E68),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141328),
        .shadowMtx = DEMO_HEAP_MATRIX(141328 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x24A1123F, -0x17B68273, -0x18621CF4),
            DEMO_VEC3(-0x248FA4FF, -0x17B2F557, -0x186CC82C),
            DEMO_VEC3(-0x24A5A377, -0x17B68273, -0x18699330),
            DEMO_VEC3(-0x24943637, -0x17B2F557, -0x18743E68),
            DEMO_VEC3(-0x2496848A, -0x17B45560, -0x1868929E),
            DEMO_VEC3(-0x2485174A, -0x17B0C844, -0x18733DD6),
            DEMO_VEC3(-0x249B15C2, -0x17B45560, -0x187008DA),
            DEMO_VEC3(-0x2489A882, -0x17B0C844, -0x187AB412),
        },
        .groundContactMask = 15,
        .coursePathIndex = 52,
        .coursePathOffset = 0x1D8B672,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135632),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 1330,
    } },
    /* Segment 7, frame 620. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x3008F9A4, -0x1C4DA282, -0x9CBFB40),
        .renderPosition = DEMO_VEC3(-0x3008F9A4, -0x1C4CAC63, -0x9CBFB40),
        .previousPosition = DEMO_VEC3(-0x2FFA367E, -0x1C4A1ADE, -0x9CF6A6E),
        .velocity = DEMO_VEC3(-0xEC326, -0x448E4, 0x36F2E),
        .groundOffset = 0xCD9A,
        .collisionBottomY = -0x1C5463C2,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x123B9,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x37DCD,
        .modeValue = 1024,
        .stickX = 23,
        .stickY = -20,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -970, -233, 225,  -279, 963, -205,  -165, -257, -977,
                -0x3008F9A4, -0x1C4CAC63, -0x9CBFB40
            ),
            DEMO_TRANSFORM(
                -858, -133, 541,  -279, 963, -205,  -482, -320, -843,
                -0x300A6D8C, -0x1C462301, -0x9CB7747
            ),
            DEMO_TRANSFORM(
                -621, -563, 587,  -136, 800, 623,  -801, 299, -560,
                -0x300908C4, -0x1C466951, -0x9CC289F
            ),
            DEMO_TRANSFORM(
                -621, -563, 587,  -813, 455, -422,  -28, -722, -723,
                -0x3008A294, -0x1C48C151, -0x9CDFC6F
            ),
            DEMO_TRANSFORM(
                -894, 232, 439,  497, 423, 788,  -2, 902, -483,
                -0x300B8C74, -0x1C46CD71, -0x9CA927F
            ),
            DEMO_TRANSFORM(
                -849, 260, 508,  286, 981, -23,  -493, 122, -887,
                -0x300D0134, -0x1C480B11, -0x9CCE17F
            ),
            DEMO_TRANSFORM(
                -330, -348, 903,  -434, 906, 190,  -865, -322, -440,
                -0x300A6D8C, -0x1C462301, -0x9CB7747
            ),
            DEMO_TRANSFORM(
                359, -7, 957,  -639, 760, 245,  -713, -684, 262,
                -0x300C1FCC, -0x1C4298C1, -0x9CAB887
            ),
            DEMO_TRANSFORM(
                266, -871, 466,  -4, 481, 903,  -987, -237, 121,
                -0x300B1AF4, -0x1C423061, -0x9CD050F
            ),
            DEMO_TRANSFORM(
                137, -1013, 35,  357, 82, 955,  -948, -116, 365,
                -0x300B9BC0, -0x1C424049, -0x9D13C9F
            ),
            DEMO_TRANSFORM(
                -242, 117, 987,  -249, 976, -178,  -962, -282, -202,
                -0x300CB814, -0x1C43E3B1, -0x9C89BAF
            ),
            DEMO_TRANSFORM(
                -266, 649, 743,  262, 789, -595,  -952, 35, -371,
                -0x300C47C4, -0x1C473C0D, -0x9C6072F
            ),
            DEMO_TRANSFORM(
                -970, -233, 225,  -279, 963, -205,  -165, -257, -977,
                -0x300656EC, -0x1C4985FB, -0x9CD4160
            ),
            DEMO_TRANSFORM(
                -970, -233, 225,  -279, 963, -205,  -165, -257, -977,
                -0x300CF9BC, -0x1C4B1F0B, -0x9CBB650
            ),
        },
        .localVelocityZ = -0xF919D,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3946,
        .modelBaseYaw = -0x2B6B,
        .modelBaseRoll = -170,
        .projectedPos = DEMO_VEC3(-0x300BA424, -0x1C41F382, -0x9CB5D00),
        .movementSpeed = 0xF2811,
        .grabInputFrames = 2,
        .grabTrickId = 4,
        .grabHoldFrames = 40,
        .aerialTrickId = 8,
        .trickAttackCompletedTrickMask = 0x100,
        .collisionVelocityX = -0xEC326,
        .collisionVelocityZ = 0x36F2E,
        .trailEffectTimer = 44,
        .pitchAngle = 3946,
        .facingAngle = -0x2B6B,
        .leanAngle = -170,
        .surfacePitchAngle = -150,
        .surfaceRollAngle = -1,
        .leanInput = -368,
        .leanScale = 52,
        .modeAngle = -170,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xF9630,
        .speedLimit = 0xF9630,
        .rankSpeedBonus = 560,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x671C0, -0x1E5C0),
            DEMO_MODEL_PART(1, 4, 96, 3461, 3455, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 987, 3461, 3455, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2638, 3406, 550, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3368, 3448, 480, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 196, 3694, -343, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 326, 0x1052, -194, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 60, 4065, 3246, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, -95, 4027, 2944, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 4078, 3820, -15, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3703, 3923, 373, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 2,
        .animationStartOffset = 1404,
        .animationFrameDataOffset = 1404,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x30121AC8, -0x1C50DEBE, -0x9C5851E),
            DEMO_VEC3(-0x2FFE6F04, -0x1C4C20B6, -0x9CA1832),
            DEMO_VEC3(-0x30138444, -0x1C50C55D, -0x9CDDE4E),
            DEMO_VEC3(-0x2FFFD880, -0x1C4C0755, -0x9D27162),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(139920),
        .shadowMtx = DEMO_HEAP_MATRIX(139920 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x30121AC8, -0x1C4DF195, -0x9C5851E),
            DEMO_VEC3(-0x2FFE6F04, -0x1C493501, -0x9CA1832),
            DEMO_VEC3(-0x30138444, -0x1C5023C5, -0x9CDDE4E),
            DEMO_VEC3(-0x2FFFD880, -0x1C4B6731, -0x9D27162),
            DEMO_VEC3(-0x300338A6, -0x1C4A676E, -0x9C8C92A),
            DEMO_VEC3(-0x2FEFA2EE, -0x1C45AADA, -0x9CDB99E),
            DEMO_VEC3(-0x3004CA0E, -0x1C4C999E, -0x9D11B3E),
            DEMO_VEC3(-0x2FF13456, -0x1C47DD0A, -0x9D60BB2),
        },
        .groundContactMask = 12,
        .coursePathIndex = 71,
        .coursePathOffset = 0xFB007A,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135504),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_SPEED_FAN_LOOP,
        .loopSoundPitch = 0.0f,
        .speedFan = {
            .state = 2,
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x2FFD9BA4, -0x1C49F163, -0x9CE9E40),
            .frontTransform = DEMO_TRANSFORM(
                970, 233, -225,  -279, 963, -205,  165, 257, 977,
                -0x2FFD9BA4, -0x1C49F163, -0x9CE9E40
            ),
            .backTransform = DEMO_TRANSFORM(
                970, 233, -225,  -14, -679, -765,  -323, 728, -640,
                -0x2FFC7094, -0x1C40DC63, -0x9D131A0
            ),
            .modelYaw = 2048,
            .spinAngleX = -0x2540,
            .frontMatrix = DEMO_HEAP_MATRIX(132688),
            .backMatrix = DEMO_HEAP_MATRIX(132688 + sizeof(Mtx)),
            .scaleStep = 16,
        },
        .replayFrame = 1689,
    } },
    /* Segment 8, frame 674. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x310410D9, -0x1C95142E, -0x9D01E6D),
        .renderPosition = DEMO_VEC3(-0x310410D9, -0x1C938E5F, -0x9D01E6D),
        .previousPosition = DEMO_VEC3(-0x30F58F2B, -0x1C91822B, -0x9CC1C1E),
        .velocity = DEMO_VEC3(-0xE81AE, -0x45343, -0x4024F),
        .groundOffset = 0xF715,
        .collisionBottomY = -0x1C9BD56E,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x1DCE4,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x39E4E,
        .modeValue = 1024,
        .heldInput = STICK_DOWN | STICK_RIGHT,
        .previousInput = STICK_DOWN | STICK_RIGHT,
        .stickX = 31,
        .stickY = -31,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -910, -236, -404,  -23, 906, -475,  468, -413, -811,
                -0x310410D9, -0x1C938E5F, -0x9D01E6D
            ),
            DEMO_TRANSFORM(
                -1014, -83, -108,  -23, 906, -475,  134, -468, -899,
                -0x3105135C, -0x1C8D0448, -0x9D1AAEA
            ),
            DEMO_TRANSFORM(
                -891, -484, 136,  -443, 887, 249,  -236, 158, -982,
                -0x310393F4, -0x1C8D5628, -0x9D146F2
            ),
            DEMO_TRANSFORM(
                -891, -484, 136,  -337, 368, -892,  372, -822, -480,
                -0x310247B4, -0x1C8FEFF8, -0x9D201E2
            ),
            DEMO_TRANSFORM(
                -956, 255, -260,  -69, 587, 834,  358, 798, -531,
                -0x31068CD4, -0x1C8D94F8, -0x9D197F2
            ),
            DEMO_TRANSFORM(
                -960, 300, -185,  302, 977, 17,  182, -38, -1005,
                -0x31065884, -0x1C8F4D68, -0x9D40A02
            ),
            DEMO_TRANSFORM(
                -582, -489, 684,  -579, 836, 106,  -610, -327, -752,
                -0x3105135C, -0x1C8D0448, -0x9D1AAEA
            ),
            DEMO_TRANSFORM(
                49, -98, 1017,  -799, 630, 99,  -636, -800, -46,
                -0x3107565C, -0x1C89BF88, -0x9D140AA
            ),
            DEMO_TRANSFORM(
                423, -653, 664,  -196, 650, 765,  -910, -444, 143,
                -0x3105A23C, -0x1C88F608, -0x9D2F9EA
            ),
            DEMO_TRANSFORM(
                430, -922, 99,  155, 179, 995,  -915, -403, 215,
                -0x3105BD84, -0x1C8A10A8, -0x9D71356
            ),
            DEMO_TRANSFORM(
                -582, 32, 840,  -281, 955, -232,  -792, -364, -535,
                -0x310879BC, -0x1C8B5A38, -0x9CFA1FA
            ),
            DEMO_TRANSFORM(
                -586, 487, 681,  365, 897, -328,  -754, 55, -688,
                -0x310894F8, -0x1C8ECA00, -0x9CD245A
            ),
            DEMO_TRANSFORM(
                -910, -236, -404,  -23, 906, -475,  468, -413, -811,
                -0x31010371, -0x1C9088CF, -0x9CFE59D
            ),
            DEMO_TRANSFORM(
                -910, -236, -404,  -23, 906, -475,  468, -413, -811,
                -0x31073BF1, -0x1C92271F, -0x9D2A9ED
            ),
        },
        .localVelocityZ = -0xF559E,
        .localVelocityX = -0x23CE7,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3944,
        .modelBaseYaw = -0x1D11,
        .modelBaseRoll = -279,
        .projectedPos = DEMO_VEC3(-0x31069999, -0x1C89676E, -0x9D13E6D),
        .movementSpeed = 0xF0CE2,
        .grabInputFrames = 2,
        .grabTrickId = 4,
        .grabHoldFrames = 40,
        .aerialTrickId = 8,
        .trickAttackCompletedTrickMask = 0x100,
        .collisionVelocityX = -0xE81AE,
        .collisionVelocityZ = -0x4024F,
        .trailEffectTimer = 27,
        .pitchAngle = 3944,
        .facingAngle = -0x1D11,
        .leanAngle = -279,
        .surfacePitchAngle = -152,
        .surfaceRollAngle = -4054,
        .leanInput = -496,
        .leanScale = 63,
        .modeAngle = -279,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xF9400,
        .speedLimit = 0xF9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x68880, -0x1E080),
            DEMO_MODEL_PART(1, 4, 104, 3457, 3462, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 968, 3457, 3462, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2663, 3404, 545, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3368, 3448, 480, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 193, 3862, -573, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 391, 0x1116, -421, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, -205, 0x1144, 3122, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, -397, 0x1086, 2738, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 0x104F, 3982, -230, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3830, 4007, 83, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 2,
        .animationStartOffset = 1404,
        .animationFrameDataOffset = 1404,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x310F494F, -0x1C991000, -0x9D0C137),
            DEMO_VEC3(-0x30FCD8AF, -0x1C943AFD, -0x9C88D93),
            DEMO_VEC3(-0x310B4903, -0x1C986229, -0x9D7AF47),
            DEMO_VEC3(-0x30F8D863, -0x1C938D26, -0x9CF7BA3),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107152),
        .shadowMtx = DEMO_HEAP_MATRIX(107152 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x310F494F, -0x1C943073, -0x9D0C137),
            DEMO_VEC3(-0x30FCD8AF, -0x1C8F644F, -0x9C88D93),
            DEMO_VEC3(-0x310B4903, -0x1C97B86F, -0x9D7AF47),
            DEMO_VEC3(-0x30F8D863, -0x1C92EC4B, -0x9CF7BA3),
            DEMO_VEC3(-0x3100CB1F, -0x1C908C6A, -0x9CC4294),
            DEMO_VEC3(-0x30EE077F, -0x1C8BB87E, -0x9C4DBD8),
            DEMO_VEC3(-0x30FD16D7, -0x1C9411AA, -0x9D35C64),
            DEMO_VEC3(-0x30EA5337, -0x1C8F3DBE, -0x9CBF5A8),
        },
        .groundContactMask = 12,
        .coursePathIndex = 72,
        .coursePathOffset = 0xDB6485,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(102736),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_SPEED_FAN_LOOP,
        .loopSoundPitch = 0.0f,
        .speedFan = {
            .state = 2,
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x30F966D9, -0x1C90CA5F, -0x9CB626D),
            .frontTransform = DEMO_TRANSFORM(
                910, 236, 404,  -23, 906, -475,  -468, 413, 811,
                -0x30F966D9, -0x1C90CA5F, -0x9CB626D
            ),
            .backTransform = DEMO_TRANSFORM(
                910, 236, 404,  -200, 994, -128,  -424, 35, 930,
                -0x30F64409, -0x1C882F3F, -0x9CDF6BD
            ),
            .modelYaw = 2048,
            .spinAngleX = 256,
            .frontMatrix = DEMO_HEAP_MATRIX(99920),
            .backMatrix = DEMO_HEAP_MATRIX(99920 + sizeof(Mtx)),
            .scaleStep = 16,
        },
        .replayFrame = 1706,
    } },
    /* Segment 9, frame 723. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x8BC17AF, -0x5FA30A7, -0x145872AD),
        .renderPosition = DEMO_VEC3(-0x8BC17AF, -0x5FD90A7, -0x145872AD),
        .previousPosition = DEMO_VEC3(-0x8B75190, -0x5F2217B, -0x14500D19),
        .velocity = DEMO_VEC3(-0x4C61F, -0x8C5F4, -0x86594),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x600E76F,
        .targetGroundOffset = 0x40000,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x7F786,
        .modeValue = 0x40000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -389, -654, -684,  -332, 787, -563,  886, 8, -511,
                -0x8BC17AF, -0x5FD90A7, -0x145872AD
            ),
            DEMO_TRANSFORM(
                -664, -619, -472,  -332, 787, -563,  703, -212, -712,
                -0x8C00B04, -0x5F74B9C, -0x145C318D
            ),
            DEMO_TRANSFORM(
                -538, -833, -250,  -718, 592, -424,  490, -47, -896,
                -0x8BEE844, -0x5F6C5E4, -0x145B3A25
            ),
            DEMO_TRANSFORM(
                -538, -833, -250,  -334, 470, -844,  802, -363, -519,
                -0x8BCCDC4, -0x5F881E4, -0x1459FC25
            ),
            DEMO_TRANSFORM(
                -740, -371, -601,  -605, 781, 263,  364, 546, -784,
                -0x8C0DAA4, -0x5F89624, -0x145C9C25
            ),
            DEMO_TRANSFORM(
                -841, -408, -414,  -385, 937, -139,  435, 41, -924,
                -0x8BF14E4, -0x5FAE074, -0x145D6165
            ),
            DEMO_TRANSFORM(
                -799, -551, -321,  -128, 643, -784,  624, -573, -571,
                -0x8C00B04, -0x5F74B9C, -0x145C318D
            ),
            DEMO_TRANSFORM(
                -1016, -48, 107,  -117, 492, -888,  -10, -895, -494,
                -0x8C08B04, -0x5F4C7DC, -0x145F420D
            ),
            DEMO_TRANSFORM(
                -424, -929, -66,  -755, 385, -572,  544, -189, -844,
                -0x8BE872C, -0x5F3BFA4, -0x145E16E5
            ),
            DEMO_TRANSFORM(
                -165, -991, 191,  -986, 116, -244,  215, -224, -974,
                -0x8BAEF5C, -0x5F3586C, -0x145BDD69
            ),
            DEMO_TRANSFORM(
                -928, -223, -367,  15, 856, -558,  429, -512, -773,
                -0x8C26EDC, -0x5F67104, -0x145FA915
            ),
            DEMO_TRANSFORM(
                -696, 373, -649,  524, 876, -58,  534, -372, -788,
                -0x8C44D68, -0x5FA03D8, -0x145E5501
            ),
            DEMO_TRANSFORM(
                -389, -654, -684,  -332, 787, -563,  886, 8, -511,
                -0x8BB931F, -0x5F967EF, -0x14577BFD
            ),
            DEMO_TRANSFORM(
                -389, -654, -684,  -332, 787, -563,  886, 8, -511,
                -0x8BE3BDF, -0x5FDE14F, -0x145C296D
            ),
        },
        .localVelocityZ = -0xC8FFB,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3644,
        .modelBaseYaw = 0x2151,
        .modelBaseRoll = 7,
        .projectedPos = DEMO_VEC3(-0x8BFE16F, -0x5F0F627, -0x145F1CED),
        .movementSpeed = 0x9A8B4,
        .grabInputFrames = 20,
        .collisionVelocityX = -0x4C61F,
        .collisionVelocityZ = -0x86594,
        .pitchAngle = 3644,
        .facingAngle = 0x2151,
        .leanAngle = 7,
        .surfacePitchAngle = -452,
        .surfaceRollAngle = -4044,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 200, 3432, 16, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 224, 3872, 160, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 472, 3512, 3440, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 376, 3712, 3264, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3928, 3568, 320, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3504, 3792, 824, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 262,
        .animationFrameCount = 20,
        .animationFramesRemaining = 20,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x8C3D1FD, -0x6058AE6, -0x145D31FF),
            DEMO_VEC3(-0x8BBF00D, -0x5F83C18, -0x144F5473),
            DEMO_VEC3(-0x8BC3F51, -0x604DE29, -0x146190E7),
            DEMO_VEC3(-0x8B45D61, -0x5F78F5A, -0x1453B35B),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140816),
        .shadowMtx = DEMO_HEAP_MATRIX(140816 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x8C3D1FD, -0x6043B49, -0x145D31FF),
            DEMO_VEC3(-0x8BBF00D, -0x5F6F811, -0x144F5473),
            DEMO_VEC3(-0x8BC3F51, -0x604293D, -0x146190E7),
            DEMO_VEC3(-0x8B45D61, -0x5F6E605, -0x1453B35B),
            DEMO_VEC3(-0x8BF0A3A, -0x5FC2FF1, -0x1454C969),
            DEMO_VEC3(-0x8B7284A, -0x5EEECB9, -0x1446EBDD),
            DEMO_VEC3(-0x8B77AD6, -0x5FC163D, -0x14592E55),
            DEMO_VEC3(-0x8AF98E6, -0x5EED305, -0x144B50C9),
        },
        .groundContactMask = 15,
        .coursePathIndex = 25,
        .coursePathOffset = 0x21972F1,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135376),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 508,
    } },
    /* Segment 10, frame 775. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x8A90973, -0x5DA0F58, -0x1436D6E6),
        .renderPosition = DEMO_VEC3(-0x8A90973, -0x5DD50AE, -0x1436D6E6),
        .previousPosition = DEMO_VEC3(-0x8A44D97, -0x5D223CF, -0x142E6B45),
        .velocity = DEMO_VEC3(-0x4BBDC, -0x8ACC9, -0x86BA1),
        .groundOffset = 0x3E156,
        .collisionBottomY = -0x5E0D098,
        .targetGroundOffset = 0x40000,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x7F789,
        .modeValue = 0x40000,
        .stickX = -4,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -385, -654, -686,  -340, 787, -558,  884, 18, -514,
                -0x8A90973, -0x5DD50AE, -0x1436D6E6
            ),
            DEMO_TRANSFORM(
                -661, -622, -472,  -340, 787, -558,  703, -203, -715,
                -0x8AD0ACF, -0x5D71C5C, -0x143A7FB1
            ),
            DEMO_TRANSFORM(
                -529, -839, -249,  -735, 584, -404,  474, -30, -905,
                -0x8ABE857, -0x5D69564, -0x143988A1
            ),
            DEMO_TRANSFORM(
                -529, -839, -249,  -332, 462, -849,  809, -358, -512,
                -0x8A9C117, -0x5D84BC4, -0x14385971
            ),
            DEMO_TRANSFORM(
                -740, -371, -601,  -616, 763, 289,  344, 571, -776,
                -0x8ADD817, -0x5D86814, -0x143AEB31
            ),
            DEMO_TRANSFORM(
                -839, -413, -413,  -393, 935, -133,  431, 49, -926,
                -0x8AC09B7, -0x5DAA4E4, -0x143BC421
            ),
            DEMO_TRANSFORM(
                -769, -556, -383,  -88, 657, -777,  668, -552, -542,
                -0x8AD0ACF, -0x5D71C5C, -0x143A7FB1
            ),
            DEMO_TRANSFORM(
                -1018, -53, 70,  -87, 491, -892,  12, -895, -494,
                -0x8AD62CF, -0x5D48A9C, -0x143D8971
            ),
            DEMO_TRANSFORM(
                -376, -950, -54,  -720, 322, -650,  620, -201, -787,
                -0x8AB772F, -0x5D3812C, -0x143C38D9
            ),
            DEMO_TRANSFORM(
                -130, -995, 195,  -970, 65, -316,  295, -226, -952,
                -0x8A81757, -0x5D2D428, -0x1439BC79
            ),
            DEMO_TRANSFORM(
                -912, -126, -446,  110, 896, -479,  450, -476, -785,
                -0x8AF386F, -0x5D6387C, -0x143E1799
            ),
            DEMO_TRANSFORM(
                -649, 387, -688,  504, 889, 25,  608, -323, -756,
                -0x8B16843, -0x5D9C034, -0x143D356D
            ),
            DEMO_TRANSFORM(
                -385, -654, -686,  -340, 787, -558,  884, 18, -514,
                -0x8A88CE3, -0x5D9281E, -0x1435DB56
            ),
            DEMO_TRANSFORM(
                -385, -654, -686,  -340, 787, -558,  884, 18, -514,
                -0x8AB2FF3, -0x5DDA17E, -0x143A8C46
            ),
        },
        .localVelocityZ = -0xC8FFF,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3644,
        .modelBaseYaw = 0x214E,
        .modelBaseRoll = 15,
        .projectedPos = DEMO_VEC3(-0x8ACCAF3, -0x5D0D4D8, -0x143D85A6),
        .movementSpeed = 0x9A8EB,
        .grabInputFrames = 20,
        .collisionVelocityX = -0x4BBDC,
        .collisionVelocityZ = -0x86BA1,
        .pitchAngle = 3644,
        .facingAngle = 0x214E,
        .leanAngle = 15,
        .surfacePitchAngle = -452,
        .surfaceRollAngle = -4041,
        .leanInput = 64,
        .leanScale = 32,
        .modeAngle = 18,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x81B80, -0x18380),
            DEMO_MODEL_PART(1, 4, 243, 3391, 3598, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 639, 3391, 3598, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3109, 3369, 456, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 184, 3393, 51, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 221, 3854, 180, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 567, 3526, 3399, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 439, 3723, 3248, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3824, 3584, 438, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3436, 3793, 874, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 1526,
        .animationFrameDataOffset = 1526,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x8B0B969, -0x5E54BBB, -0x143B9820),
            DEMO_VEC3(-0x8A8E855, -0x5D7FDFC, -0x142DB034),
            DEMO_VEC3(-0x8A92A91, -0x5E4A556, -0x143FFD98),
            DEMO_VEC3(-0x8A1597D, -0x5D75797, -0x143215AC),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141328),
        .shadowMtx = DEMO_HEAP_MATRIX(141328 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x8B0B969, -0x5E405FA, -0x143B9820),
            DEMO_VEC3(-0x8A8E855, -0x5D6C2C2, -0x142DB034),
            DEMO_VEC3(-0x8A92A91, -0x5E3DE9A, -0x143FFD98),
            DEMO_VEC3(-0x8A1597D, -0x5D69B62, -0x143215AC),
            DEMO_VEC3(-0x8ABFCDF, -0x5DBF4A8, -0x14333041),
            DEMO_VEC3(-0x8A430FB, -0x5CEB170, -0x142545BD),
            DEMO_VEC3(-0x8A46A33, -0x5DBD090, -0x143790CD),
            DEMO_VEC3(-0x89C9E4F, -0x5CE8D58, -0x1429A649),
        },
        .groundContactMask = 15,
        .coursePathIndex = 25,
        .coursePathOffset = 0x1F2FCE2,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135376),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 504,
    } },
    /* Segment 11, frame 825. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1100F7D5, -0xBCB22F2, -0x2057017D),
        .renderPosition = DEMO_VEC3(-0x1100F7D5, -0xBCDECFA, -0x2057017D),
        .previousPosition = DEMO_VEC3(-0x10F67620, -0xBC72A1D, -0x2051644C),
        .velocity = DEMO_VEC3(-0xA81B5, -0x4B100, -0x59D31),
        .groundOffset = 0x40000,
        .collisionBottomY = -0xBD1DB1D,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x95F8,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3EA47,
        .modeValue = 0x40000,
        .stickX = -19,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -856, -324, -457,  -343, 963, -38,  442, 121, -914,
                -0x1100F7D5, -0xBCDECFA, -0x2057017D
            ),
            DEMO_TRANSFORM(
                -955, -346, -122,  -343, 963, -38,  128, 5, -1015,
                -0x11045BF1, -0xBC66ADB, -0x2055F8D1
            ),
            DEMO_TRANSFORM(
                -797, -636, -81,  -634, 763, 247,  -93, 242, -989,
                -0x1102CAC1, -0xBC66183, -0x2055C601
            ),
            DEMO_TRANSFORM(
                -797, -636, -81,  -582, 772, -332,  267, -213, -964,
                -0x1100EF41, -0xBC89DF3, -0x20567FD1
            ),
            DEMO_TRANSFORM(
                -1014, -63, -115,  -127, 678, 755,  30, 763, -680,
                -0x11059731, -0xBC76503, -0x20562211
            ),
            DEMO_TRANSFORM(
                -1008, -157, 78,  -120, 952, 355,  -127, 340, -956,
                -0x110537C1, -0xBC961E3, -0x20585851
            ),
            DEMO_TRANSFORM(
                -963, -272, -212,  -150, 898, -465,  310, -406, -885,
                -0x11045BF1, -0xBC66ADB, -0x2055F8D1
            ),
            DEMO_TRANSFORM(
                -872, 81, 527,  -299, 761, -613,  -441, -678, -626,
                -0x1104F2B1, -0xBC2E89B, -0x2057C9D1
            ),
            DEMO_TRANSFORM(
                -453, -908, -129,  -824, 466, -385,  401, -66, -938,
                -0x110285D1, -0xBC2AEBB, -0x20570AB9
            ),
            DEMO_TRANSFORM(
                -184, -1005, -53,  -1006, 182, 27,  -17, 57, -1021,
                -0x10FE9E91, -0xBC29DB7, -0x20556071
            ),
            DEMO_TRANSFORM(
                -873, 503, -171,  521, 876, -81,  106, -157, -1005,
                -0x110739E1, -0xBC4030B, -0x205814A9
            ),
            DEMO_TRANSFORM(
                -658, 727, -289,  695, 717, 219,  359, -55, -956,
                -0x110AD789, -0xBC63CA7, -0x20581E1D
            ),
            DEMO_TRANSFORM(
                -856, -324, -457,  -343, 963, -38,  442, 121, -914,
                -0x10FEE175, -0xBCA773A, -0x20558915
            ),
            DEMO_TRANSFORM(
                -856, -324, -457,  -343, 963, -38,  442, 121, -914,
                -0x1104BBE5, -0xBCCAEAA, -0x2058A9B5
            ),
        },
        .localVelocityZ = -0xC8FC9,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3886,
        .modelBaseYaw = 0x12C0,
        .modelBaseRoll = 82,
        .projectedPos = DEMO_VEC3(-0x11045115, -0xBBFC132, -0x2058CBBD),
        .movementSpeed = 0xBE998,
        .grabInputFrames = 20,
        .collisionVelocityX = -0xA81B5,
        .collisionVelocityZ = -0x59D31,
        .pitchAngle = 3886,
        .facingAngle = 0x12C0,
        .leanAngle = 82,
        .surfacePitchAngle = -210,
        .surfaceRollAngle = -7,
        .leanInput = 304,
        .leanScale = 32,
        .modeAngle = 86,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 122, 3246, 185, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 207, 3786, 258, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 929, 3583, 3244, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 678, 3766, 3183, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3427, 3649, 887, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3175, 3799, 1067, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 1526,
        .animationFrameDataOffset = 1526,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x110B88FD, -0xBD25F17, -0x2057BB9F),
            DEMO_VEC3(-0x10FA2F31, -0xBCBC936, -0x204E7657),
            DEMO_VEC3(-0x1107C079, -0xBD28D91, -0x205F8CA3),
            DEMO_VEC3(-0x10F666AD, -0xBCBF7DB, -0x2056475B),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141328),
        .shadowMtx = DEMO_HEAP_MATRIX(141328 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x110B88FD, -0xBD1BAE4, -0x2057BB9F),
            DEMO_VEC3(-0x10FA2F31, -0xBCB28D8, -0x204E7657),
            DEMO_VEC3(-0x1107C079, -0xBD0B11C, -0x205F8CA3),
            DEMO_VEC3(-0x10F666AD, -0xBCA1F10, -0x2056475B),
            DEMO_VEC3(-0x11010672, -0xBCDC1D6, -0x20523BE4),
            DEMO_VEC3(-0x10EFC54A, -0xBC72FCA, -0x2048C6A0),
            DEMO_VEC3(-0x10FD26F6, -0xBCCBB56, -0x205A01F8),
            DEMO_VEC3(-0x10EBE5CE, -0xBC6294A, -0x20508CB4),
        },
        .groundContactMask = 15,
        .coursePathIndex = 35,
        .coursePathOffset = 0xF35716,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135376),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 834,
    } },
    /* Segment 12, frame 878. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x16B9B3B3, -0xDEEDBAE, -0x204AB59E),
        .renderPosition = DEMO_VEC3(-0x16B9B3B3, -0xDF0E0B3, -0x204AB59E),
        .previousPosition = DEMO_VEC3(-0x16AEEE98, -0xDE9CE45, -0x204E99F4),
        .velocity = DEMO_VEC3(-0xAC51B, -0x5CEA9, 0x3E456),
        .groundOffset = 0x2AA0A,
        .collisionBottomY = -0xDF59CEE,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1285,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x51968,
        .modeValue = 0x40000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -877, -420, 317,  -395, 933, 142,  -348, 0, -963,
                -0x16B9B3B3, -0xDF0E0B3, -0x204AB59E
            ),
            DEMO_TRANSFORM(
                -709, -396, 622,  -395, 933, 142,  -623, -141, -800,
                -0x16BC031A, -0xDEA16B2, -0x20481AC0
            ),
            DEMO_TRANSFORM(
                -489, -718, 540,  -250, 699, 703,  -863, 203, -510,
                -0x16BAC7B2, -0xDE9F6E2, -0x20491608
            ),
            DEMO_TRANSFORM(
                -489, -718, 540,  -834, 591, 30,  -333, -426, -868,
                -0x16BA0C32, -0xDEC0382, -0x204B2578
            ),
            DEMO_TRANSFORM(
                -788, -79, 647,  495, 587, 675,  -424, 833, -415,
                -0x16BCDBA2, -0xDEB1FE2, -0x20474328
            ),
            DEMO_TRANSFORM(
                -629, -181, 786,  38, 989, 258,  -805, 188, -602,
                -0x16BE4F72, -0xDECD8B2, -0x20493D98
            ),
            DEMO_TRANSFORM(
                -567, -352, 775,  -643, 787, -113,  -558, -550, -658,
                -0x16BC031A, -0xDEA16B2, -0x20481AC0
            ),
            DEMO_TRANSFORM(
                -68, 71, 1018,  -754, 684, -98,  -687, -758, 7,
                -0x16BE865A, -0xDE702F2, -0x20488C00
            ),
            DEMO_TRANSFORM(
                -348, -825, 495,  -563, 601, 605,  -779, -66, -659,
                -0x16BCD342, -0xDE68942, -0x204A6288
            ),
            DEMO_TRANSFORM(
                -76, -978, 292,  -348, 300, 913,  -958, -31, -356,
                -0x16BA14BA, -0xDE720A6, -0x204D91EC
            ),
            DEMO_TRANSFORM(
                -556, -15, 858,  -318, 953, -189,  -797, -369, -523,
                -0x16BF98A2, -0xDE84192, -0x20469928
            ),
            DEMO_TRANSFORM(
                -632, 602, 533,  253, 793, -595,  -763, -235, -639,
                -0x16BF84A6, -0xDEBC796, -0x20443A9C
            ),
            DEMO_TRANSFORM(
                -877, -420, 317,  -395, 933, 142,  -348, 0, -963,
                -0x16B7AADB, -0xDED291B, -0x204B71C6
            ),
            DEMO_TRANSFORM(
                -877, -420, 317,  -395, 933, 142,  -348, 0, -963,
                -0x16BDAAEB, -0xDF0096B, -0x20494706
            ),
        },
        .localVelocityZ = -0xC8FFB,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3820,
        .modelBaseYaw = 0x14E2,
        .projectedPos = DEMO_VEC3(-0x16BE5633, -0xDE3EB2E, -0x2049095E),
        .movementSpeed = 0xB739B,
        .grabInputFrames = 20,
        .collisionVelocityX = -0xAC51B,
        .collisionVelocityZ = 0x3E456,
        .pitchAngle = 3820,
        .facingAngle = 0x14E2,
        .surfacePitchAngle = -276,
        .surfaceRollAngle = -4045,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x77280, -0x1AA80),
            DEMO_MODEL_PART(1, 4, 185, 3419, 3541, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 777, 3419, 3541, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2922, 3383, 494, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 240, 3432, 8, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 232, 3872, 160, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 464, 3520, 3440, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 368, 3720, 3264, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3928, 3560, 312, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3496, 3792, 816, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 1342,
        .animationFrameCount = 20,
        .animationFramesRemaining = 2,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x16C11BBD, -0xDF67FD6, -0x2043621E),
            DEMO_VEC3(-0x16AF5269, -0xDEDF0C7, -0x2049CE8E),
            DEMO_VEC3(-0x16C414FD, -0xDF5CD40, -0x204B9CAE),
            DEMO_VEC3(-0x16B24BA9, -0xDED3E31, -0x2052091E),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141328),
        .shadowMtx = DEMO_HEAP_MATRIX(141328 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x16C11BBD, -0xDF52405, -0x2043621E),
            DEMO_VEC3(-0x16AF5269, -0xDEC9D61, -0x2049CE8E),
            DEMO_VEC3(-0x16C414FD, -0xDF52405, -0x204B9CAE),
            DEMO_VEC3(-0x16B24BA9, -0xDEC9D61, -0x2052091E),
            DEMO_VEC3(-0x16B656A2, -0xDEFF52A, -0x20474674),
            DEMO_VEC3(-0x16A48D4E, -0xDE76E86, -0x204DB2E4),
            DEMO_VEC3(-0x16B94FE2, -0xDEFF52A, -0x204F8104),
            DEMO_VEC3(-0x16A7868E, -0xDE76E86, -0x2055ED74),
        },
        .groundContactMask = 15,
        .coursePathIndex = 43,
        .coursePathOffset = 0x67787B,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135376),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 962,
    } },
    /* Segment 13, frame 928. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x11D3D2E2, -0xC12A4C7, -0x20AC0C56),
        .renderPosition = DEMO_VEC3(-0x11D3D2E2, -0xC14F576, -0x20AC0C56),
        .previousPosition = DEMO_VEC3(-0x11C85B55, -0xC0EE779, -0x20A8AAEB),
        .velocity = DEMO_VEC3(-0xB778D, -0x47E8E, -0x3616B),
        .groundOffset = 0x392DA,
        .collisionBottomY = -0xC196607,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0xA22B,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3C94D,
        .modeValue = 0x40000,
        .stickX = -20,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -934, -313, -275,  -336, 965, 44,  246, 131, -984,
                -0x11D3D2E2, -0xC14F576, -0x20AC0C56
            ),
            DEMO_TRANSFORM(
                -963, -339, 72,  -336, 965, 44,  -82, 17, -1020,
                -0x11D6CF28, -0xC0DAB31, -0x20AA36F3
            ),
            DEMO_TRANSFORM(
                -789, -647, 76,  -560, 735, 436,  -331, 294, -921,
                -0x11D53BF0, -0xC0DA491, -0x20AA5773
            ),
            DEMO_TRANSFORM(
                -789, -647, 76,  -643, 757, -241,  95, -234, -991,
                -0x11D397C0, -0xC0FCC61, -0x20AB9F03
            ),
            DEMO_TRANSFORM(
                -1018, -44, 96,  52, 604, 823,  -92, 824, -599,
                -0x11D80E30, -0xC0EA331, -0x20AA2173
            ),
            DEMO_TRANSFORM(
                -971, -153, 282,  -37, 948, 383,  -318, 353, -905,
                -0x11D83530, -0xC1068C1, -0x20AC8B13
            ),
            DEMO_TRANSFORM(
                -988, -264, -31,  -228, 904, -420,  136, -399, -932,
                -0x11D6CF28, -0xC0DAB31, -0x20AA36F3
            ),
            DEMO_TRANSFORM(
                -752, 84, 688,  -407, 768, -538,  -562, -669, -531,
                -0x11D7B368, -0xC0A22F1, -0x20ABDBB3
            ),
            DEMO_TRANSFORM(
                -468, -908, -54,  -880, 467, -230,  229, -58, -995,
                -0x11D52D38, -0xC09EEA9, -0x20AB9393
            ),
            DEMO_TRANSFORM(
                -193, -1004, -29,  -984, 182, 212,  -202, 68, -1000,
                -0x11D109DC, -0xC09DE39, -0x20AAA0D3
            ),
            DEMO_TRANSFORM(
                -876, 527, -17,  524, 863, -164,  -69, -150, -1009,
                -0x11DA0088, -0xC0B3949, -0x20ABBAA3
            ),
            DEMO_TRANSFORM(
                -693, 735, -159,  730, 710, 100,  182, -45, -1005,
                -0x11DDA224, -0xC0D5AB5, -0x20AB294B
            ),
            DEMO_TRANSFORM(
                -934, -313, -275,  -336, 965, 44,  246, 131, -984,
                -0x11D17372, -0xC11877E, -0x20AAFFC6
            ),
            DEMO_TRANSFORM(
                -934, -313, -275,  -336, 965, 44,  246, 131, -984,
                -0x11D7D742, -0xC13AC8E, -0x20ACE1E6
            ),
        },
        .localVelocityZ = -0xC8FC6,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3893,
        .modelBaseYaw = 0x1345,
        .modelBaseRoll = 88,
        .projectedPos = DEMO_VEC3(-0x11D75922, -0xC073887, -0x20AD15D6),
        .movementSpeed = 0xBF475,
        .grabInputFrames = 20,
        .collisionVelocityX = -0xB778D,
        .collisionVelocityZ = -0x3616B,
        .pitchAngle = 3893,
        .facingAngle = 0x1345,
        .leanAngle = 88,
        .surfacePitchAngle = -203,
        .surfaceRollAngle = -22,
        .leanInput = 320,
        .leanScale = 32,
        .modeAngle = 91,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x7F140, -0x18D40),
            DEMO_MODEL_PART(1, 4, 229, 3398, 3584, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 673, 3398, 3584, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3062, 3372, 466, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 117, 3235, 195, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 206, 3781, 264, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 955, 3587, 3232, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 695, 3769, 3179, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3398, 3653, 920, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3156, 3799, 1081, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 1526,
        .animationFrameDataOffset = 1526,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x11DE5900, -0xC195A9A, -0x20AAA1D8),
            DEMO_VEC3(-0x11CB67FC, -0xC12F7AA, -0x20A50CB0),
            DEMO_VEC3(-0x11DC3DC8, -0xC19C6A8, -0x20B30BFC),
            DEMO_VEC3(-0x11C94CC4, -0xC1363B8, -0x20AD76D4),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(108048),
        .shadowMtx = DEMO_HEAP_MATRIX(108048 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x11DE5900, -0xC18B288, -0x20AAA1D8),
            DEMO_VEC3(-0x11CB67FC, -0xC1256F4, -0x20A50CB0),
            DEMO_VEC3(-0x11DC3DC8, -0xC1793F8, -0x20B30BFC),
            DEMO_VEC3(-0x11C94CC4, -0xC113864, -0x20AD76D4),
            DEMO_VEC3(-0x11D2E4C7, -0xC14D7B7, -0x20A75E4B),
            DEMO_VEC3(-0x11C0049F, -0xC0E7C23, -0x20A193F7),
            DEMO_VEC3(-0x11D0B20B, -0xC13B927, -0x20AFC1DF),
            DEMO_VEC3(-0x11BDD1E3, -0xC0D5D93, -0x20A9F78B),
        },
        .groundContactMask = 15,
        .coursePathIndex = 36,
        .coursePathOffset = 0x952025,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(102608),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 853,
    } },
    /* Segment 14, frame 980. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1314B211, -0xC751696, -0x20D8E5DE),
        .renderPosition = DEMO_VEC3(-0x1314B211, -0xC75852F, -0x20D8E5DE),
        .previousPosition = DEMO_VEC3(-0x13089E13, -0xC7195BF, -0x20D88BFF),
        .velocity = DEMO_VEC3(-0xC13FE, -0x44217, -0x59DF),
        .groundOffset = 0x15677,
        .collisionBottomY = -0xC7BD7D6,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x47DE,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x35D04,
        .modeValue = 0x40000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -980, -292, -28,  -293, 979, 52,  12, 58, -1022,
                -0x1314B211, -0xC75852F, -0x20D8E5DE
            ),
            DEMO_TRANSFORM(
                -927, -295, 317,  -293, 979, 52,  -318, -43, -971,
                -0x1316A5F2, -0xC6F2EB3, -0x20D6BA4A
            ),
            DEMO_TRANSFORM(
                -695, -697, 279,  -314, 616, 754,  -682, 425, -632,
                -0x13152572, -0xC6F3A63, -0x20D737B2
            ),
            DEMO_TRANSFORM(
                -695, -697, 279,  -747, 606, -345,  69, -439, -921,
                -0x131439F2, -0xC710863, -0x20D96D62
            ),
            DEMO_TRANSFORM(
                -967, 67, 328,  335, 264, 930,  -22, 986, -272,
                -0x1317DD12, -0xC7017D3, -0x20D649F2
            ),
            DEMO_TRANSFORM(
                -882, -95, 510,  76, 971, 313,  -513, 308, -830,
                -0x1318D852, -0xC70DE33, -0x20D90372
            ),
            DEMO_TRANSFORM(
                -845, -267, 511,  -429, 896, -241,  -385, -414, -852,
                -0x1316A5F2, -0xC6F2EB3, -0x20D6BA4A
            ),
            DEMO_TRANSFORM(
                -408, 71, 935,  -566, 794, -308,  -748, -641, -277,
                -0x13185372, -0xC6BADF3, -0x20D7AB4A
            ),
            DEMO_TRANSFORM(
                -614, -764, 291,  -655, 678, 396,  -489, 50, -897,
                -0x13160D7A, -0xC6B7703, -0x20D8CD02
            ),
            DEMO_TRANSFORM(
                -314, -955, 189,  -609, 347, 744,  -759, 115, -676,
                -0x13127356, -0xC6C7443, -0x20DAD23E
            ),
            DEMO_TRANSFORM(
                -813, 70, 617,  -99, 988, -245,  -613, -255, -778,
                -0x131A2E0A, -0xC6CC513, -0x20D64D52
            ),
            DEMO_TRANSFORM(
                -694, 686, 307,  542, 746, -442,  -520, -136, -869,
                -0x131B6706, -0xC70404F, -0x20D432E2
            ),
            DEMO_TRANSFORM(
                -980, -292, -28,  -293, 979, 52,  12, 58, -1022,
                -0x13120F59, -0xC7220FF, -0x20D8AC46
            ),
            DEMO_TRANSFORM(
                -980, -292, -28,  -293, 979, 52,  12, 58, -1022,
                -0x1318C3A9, -0xC74214F, -0x20D8DE26
            ),
        },
        .localVelocityZ = -0xC8FFD,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3907,
        .modelBaseYaw = 0x13ED,
        .modelBaseRoll = 39,
        .projectedPos = DEMO_VEC3(-0x13181F91, -0xC6996D6, -0x20D8FF5E),
        .movementSpeed = 0xC154C,
        .grabInputFrames = 20,
        .collisionVelocityX = -0xC13FE,
        .collisionVelocityZ = -0x59DF,
        .pitchAngle = 3907,
        .facingAngle = 0x13ED,
        .leanAngle = 39,
        .surfacePitchAngle = -196,
        .surfaceRollAngle = -30,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x6BC80, -0x1D480),
            DEMO_MODEL_PART(1, 4, 122, 3449, 3480, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 925, 3449, 3480, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2721, 3399, 534, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 200, 3432, 16, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 224, 3872, 160, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 472, 3512, 3440, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 376, 3712, 3264, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3928, 3568, 320, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3504, 3792, 824, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 262,
        .animationFrameCount = 20,
        .animationFramesRemaining = 20,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x131EAF49, -0xC79A1D2, -0x20D4D1FA),
            DEMO_VEC3(-0x130ACFA5, -0xC735FEE, -0x20D43E22),
            DEMO_VEC3(-0x131E947D, -0xC79F68F, -0x20DD8D9A),
            DEMO_VEC3(-0x130AB4D9, -0xC73F78A, -0x20DCF9C2),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140304),
        .shadowMtx = DEMO_HEAP_MATRIX(140304 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x131EAF49, -0xC78BC37, -0x20D4D1FA),
            DEMO_VEC3(-0x130ACFA5, -0xC72CD93, -0x20D43E22),
            DEMO_VEC3(-0x131E947D, -0xC783CCB, -0x20DD8D9A),
            DEMO_VEC3(-0x130AB4D9, -0xC724E27, -0x20DCF9C2),
            DEMO_VEC3(-0x1312A973, -0xC74DF22, -0x20D479BF),
            DEMO_VEC3(-0x12FEA433, -0xC6F74C6, -0x20D3E5E7),
            DEMO_VEC3(-0x131297F3, -0xC743022, -0x20DD3217),
            DEMO_VEC3(-0x12FE92B3, -0xC6EC5C6, -0x20DC9E3F),
        },
        .groundContactMask = 15,
        .coursePathIndex = 38,
        .coursePathOffset = 0x559DC5,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135376),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 880,
    } },
    /* Segment 15, frame 1043. */
    { .player = {
        .characterId = RACE_CHARACTER_SLASH,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 1,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1D441BD9, -0x129A4C01, -0x1C6D1AD9),
        .renderPosition = DEMO_VEC3(-0x1D441BD9, -0x1299A166, -0x1C6D1AD9),
        .previousPosition = DEMO_VEC3(-0x1D3927E4, -0x12981683, -0x1C72DA04),
        .velocity = DEMO_VEC3(-0xAF3F5, -0x2EABE, 0x5BF2B),
        .collisionBottomY = -0x12A10141,
        .groundCorrectionY = 2715,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x21FD1,
        .modeValue = 0x46000,
        .heldInput = STICK_DOWN | STICK_LEFT | A_BUTTON,
        .previousInput = STICK_DOWN | STICK_LEFT | A_BUTTON,
        .stickX = -31,
        .stickY = -31,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -892, -178, 468,  -159, 1008, 78,  -474, -4, -906,
                -0x1D441BD9, -0x1299A166, -0x1C6D1AD9
            ),
            DEMO_TRANSFORM(
                -680, -166, 746,  -159, 1008, 78,  -747, -64, -696,
                -0x1D441E19, -0x1293B726, -0x1C6AE019
            ),
            DEMO_TRANSFORM(
                -502, -653, 607,  257, 560, 816,  -853, 553, -111,
                -0x1D430AD9, -0x1293F6CE, -0x1C6C01A1
            ),
            DEMO_TRANSFORM(
                -502, -653, 607,  -853, 553, -111,  -257, -560, -816,
                -0x1D43CC29, -0x12959AFE, -0x1C6E65A1
            ),
            DEMO_TRANSFORM(
                -681, 239, 724,  748, 18, 697,  150, 994, -188,
                -0x1D450969, -0x1294737E, -0x1C69D221
            ),
            DEMO_TRANSFORM(
                -534, 37, 872,  288, 972, 133,  -824, 315, -518,
                -0x1D473A99, -0x1294815E, -0x1C6BDD41
            ),
            DEMO_TRANSFORM(
                -290, -401, 895,  -895, 489, -70,  -401, -803, -490,
                -0x1D441E19, -0x1293B726, -0x1C6AE019
            ),
            DEMO_TRANSFORM(
                347, 119, 954,  -610, 812, 120,  -744, -610, 347,
                -0x1D479D59, -0x1291CDA6, -0x1C6B2699
            ),
            DEMO_TRANSFORM(
                630, -38, 804,  -230, 971, 225,  -772, -320, 589,
                -0x1D4677E1, -0x12910FBE, -0x1C6D4D51
            ),
            DEMO_TRANSFORM(
                665, -513, 583,  247, 867, 481,  -736, -172, 688,
                -0x1D46DB25, -0x12948B0E, -0x1C6FB355
            ),
            DEMO_TRANSFORM(
                -411, -74, 933,  -431, 920, -116,  -831, -440, -400,
                -0x1D47E301, -0x129305EE, -0x1C68EE41
            ),
            DEMO_TRANSFORM(
                -598, 526, 641,  66, 818, -610,  -827, -315, -512,
                -0x1D471CB1, -0x12968A46, -0x1C66ADED
            ),
            DEMO_TRANSFORM(
                -892, -178, 468,  -159, 1008, 78,  -474, -4, -906,
                -0x1D417289, -0x12968F6E, -0x1C6E83A9
            ),
            DEMO_TRANSFORM(
                -892, -178, 468,  -159, 1008, 78,  -474, -4, -906,
                -0x1D478CD9, -0x1297C75E, -0x1C6B5039
            ),
        },
        .localVelocityZ = -0xC9000,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3982,
        .modelBaseYaw = -0x5AC5,
        .modelBaseRoll = -3,
        .projectedPos = DEMO_VEC3(-0x1D45F519, -0x128E7B41, -0x1C6C2299),
        .movementSpeed = 0xC5E77,
        .grabInputFrames = 6,
        .collisionVelocityX = -0xAF3F5,
        .collisionVelocityZ = 0x5BF2B,
        .pitchAngle = 3982,
        .facingAngle = -0x5AC5,
        .leanAngle = -3,
        .surfacePitchAngle = -114,
        .surfaceRollAngle = -4092,
        .leanScale = 32,
        .stateFlags = 0x190,
        .mode = 2,
        .updateState = 1,
        .updateTimer = 1,
        .targetSpeedLimit = 0xC9400,
        .speedLimit = 0xC9400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x60000, -0x20000),
            DEMO_MODEL_PART(1, 4, 56, 3480, 3416, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 1080, 3480, 3416, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2512, 3416, 576, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 776, 3576, 3832, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 296, 4024, 88, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 88, 112, 4048, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 4000, 144, 3704, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 168, 3512, 64, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3624, 3584, 752, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 5,
        .animationStartOffset = 2550,
        .animationFrameDataOffset = 2790,
        .animationFrameCount = 4,
        .animationFramesRemaining = 1,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x1D4B2029, -0x129C2F82, -0x1C647C91),
            DEMO_VEC3(-0x1D390905, -0x12988997, -0x1C6DF99D),
            DEMO_VEC3(-0x1D4F2EAD, -0x129C1ECB, -0x1C6C3C15),
            DEMO_VEC3(-0x1D3D1789, -0x129878E0, -0x1C75B921),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141328),
        .shadowMtx = DEMO_HEAP_MATRIX(141328 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x1D4B2029, -0x129B6AD0, -0x1C647C91),
            DEMO_VEC3(-0x1D390905, -0x1297CE24, -0x1C6DF99D),
            DEMO_VEC3(-0x1D4F2EAD, -0x129B74A8, -0x1C6C3C15),
            DEMO_VEC3(-0x1D3D1789, -0x1297D7FC, -0x1C75B921),
            DEMO_VEC3(-0x1D402C34, -0x12993552, -0x1C6A3BBC),
            DEMO_VEC3(-0x1D2E1510, -0x129598A6, -0x1C73B8C8),
            DEMO_VEC3(-0x1D443AB8, -0x12993F2A, -0x1C71FB40),
            DEMO_VEC3(-0x1D322394, -0x1295A27E, -0x1C7B784C),
        },
        .groundContactMask = 15,
        .coursePathIndex = 48,
        .coursePathOffset = 0xEC080B,
        .shotType = 3,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 245,
        .modelPartMatrices = DEMO_MODEL_MATRICES(135376),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 1146,
    } },
},
{
    /* Player 1. */
    /* Segment 0, frame 0. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(0xB59C3E, -0x10411DC, -0x63D39D5),
        .renderPosition = DEMO_VEC3(0xB59C3E, -0x105C6B5, -0x63D39D5),
        .previousPosition = DEMO_VEC3(0xB3E0A0, -0x102FEFE, -0x630C2C4),
        .velocity = DEMO_VEC3(0x1BB9E, -0x1D41E, -0xC7711),
        .groundOffset = 0x33648,
        .collisionBottomY = -0x10AD31C,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0xE16F,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x13CED,
        .heldInput = STICK_LEFT,
        .previousInput = STICK_LEFT,
        .stickX = -31,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                140, -105, -1008,  -195, 996, -130,  995, 210, 116,
                0xB59C3E, -0x105C6B5, -0x63D39D5
            ),
            DEMO_TRANSFORM(
                -203, -165, -989,  52, 1006, -179,  1001, -86, -191,
                0xB2F1C9, -0xFCF29F, -0x63E83A0
            ),
            DEMO_TRANSFORM(
                -141, -516, -872,  -578, 763, -358,  831, 443, -398,
                0xB32DDD, -0xFDEEEB, -0x63CD88C
            ),
            DEMO_TRANSFORM(
                -141, -516, -872,  257, 833, -535,  980, -293, 14,
                0xB5705D, -0x100EA6B, -0x63B720C
            ),
            DEMO_TRANSFORM(
                -183, 390, -928,  -737, 591, 394,  686, 738, 175,
                0xB2950D, -0xFE6B3B, -0x63FBEAC
            ),
            DEMO_TRANSFORM(
                -997, -198, -117,  -183, 998, -129,  140, -105, -1008,
                0xB5760D, -0x100BA3B, -0x64148EC
            ),
            DEMO_TRANSFORM(
                -305, -61, -975,  520, 853, -216,  826, -560, -223,
                0xB2F1C9, -0xFCF29F, -0x63E83A0
            ),
            DEMO_TRANSFORM(
                -706, 106, -733,  515, 797, -380,  532, -631, -604,
                0xB41691, -0xFB1263, -0x63EFD44
            ),
            DEMO_TRANSFORM(
                -635, -546, -587,  -490, 856, -268,  634, 113, -794,
                0xB4B4A9, -0xFB219F, -0x63C8E38
            ),
            DEMO_TRANSFORM(
                155, -971, -280,  -906, -8, -474,  448, 320, -862,
                0xB82BB1, -0xFD24A7, -0x63A12B0
            ),
            DEMO_TRANSFORM(
                -406, 665, -662,  357, 776, 561,  868, -8, -541,
                0xB33769, -0xFB6DDF, -0x6415148
            ),
            DEMO_TRANSFORM(
                -175, 992, -177,  165, 205, 988,  994, 140, -195,
                0xB0D429, -0xFCD687, -0x6452140
            ),
            DEMO_TRANSFORM(
                140, -105, -1008,  -195, 996, -130,  995, 210, 116,
                0xB3E60E, -0x102B43D, -0x63ABD15
            ),
            DEMO_TRANSFORM(
                140, -105, -1008,  -195, 996, -130,  995, 210, 116,
                0xB4B80E, -0x103521D, -0x640A635
            ),
        },
        .localVelocityZ = -0xCA725,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 4029,
        .modelBaseYaw = 0x7FA6,
        .modelBaseRoll = 128,
        .projectedPos = DEMO_VEC3(0xB5C6FE, -0xF8225C, -0x63E71D5),
        .movementSpeed = 0xC95C1,
        .collisionVelocityX = 0x1BB9E,
        .collisionVelocityZ = -0xC7711,
        .pitchAngle = 4029,
        .facingAngle = 0x7FA6,
        .leanAngle = 128,
        .surfacePitchAngle = -67,
        .surfaceRollAngle = -1,
        .leanInput = 496,
        .leanScale = 32,
        .modeAngle = 141,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCABA0,
        .speedLimit = 0xCABA0,
        .rankSpeedBonus = 0x17A0,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 154, 3296, 0, 0, 0x94580, -0xEC00),
            DEMO_MODEL_PART(1, 4, 262, 3405, 3557, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 825, 3405, 3557, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2893, 3458, 779, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 304, 3360, 0x10B3, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 300, 3676, 265, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 83, 3728, 3828, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 244, 3836, 3120, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3620, 3742, 762, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3276, 4065, 1025, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2490,
        .animationFrameDataOffset = 2490,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(0xB2C6D0, -0x1085BA7, -0x647F127),
            DEMO_VEC3(0xAFF090, -0x1062FA2, -0x6338043),
            DEMO_VEC3(0xBB47EC, -0x1086599, -0x646F367),
            DEMO_VEC3(0xB871AC, -0x1063994, -0x6328283),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141072),
        .shadowMtx = DEMO_HEAP_MATRIX(141072 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(0xB2C6D0, -0x107BDA9, -0x647F127),
            DEMO_VEC3(0xAFF090, -0x1059BAD, -0x6338043),
            DEMO_VEC3(0xBB47EC, -0x105F1BD, -0x646F367),
            DEMO_VEC3(0xB871AC, -0x103CFC1, -0x6328283),
            DEMO_VEC3(0xB138EE, -0x106658F, -0x63B874A),
            DEMO_VEC3(0xAE0A7E, -0x1044CA7, -0x627235E),
            DEMO_VEC3(0xB9B6C2, -0x104A6C3, -0x63A622A),
            DEMO_VEC3(0xB68852, -0x1028DDB, -0x625FE3E),
        },
        .groundContactMask = 15,
        .coursePathIndex = 5,
        .coursePathOffset = 0x4937EC,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134480),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 190,
    } },
    /* Segment 1, frame 57. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x7D238F, -0x221572B, -0xB438DAA),
        .renderPosition = DEMO_VEC3(-0x7D238F, -0x224B3E5, -0xB438DAA),
        .previousPosition = DEMO_VEC3(-0x77523D, -0x21E0CF1, -0xB38BECB),
        .velocity = DEMO_VEC3(-0x5D152, -0x3FF70, -0xACEDF),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x2280C61,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 838,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x336BA,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -468, -264, -870,  -135, 989, -226,  899, 12, -487,
                -0x7D238F, -0x224B3E5, -0xB438DAA
            ),
            DEMO_TRANSFORM(
                -744, -248, -656,  -6, 959, -355,  702, -255, -699,
                -0x7F03FC, -0x21B49FE, -0xB456473
            ),
            DEMO_TRANSFORM(
                -682, -541, -537,  -490, 863, -246,  583, 93, -835,
                -0x7DEAD4, -0x21C18A6, -0xB43FF2B
            ),
            DEMO_TRANSFORM(
                -682, -541, -537,  -59, 756, -686,  760, -426, -535,
                -0x7C0014, -0x21F7826, -0xB43092B
            ),
            DEMO_TRANSFORM(
                -690, 220, -722,  -222, 875, 481,  721, 482, -542,
                -0x801964, -0x21CD306, -0xB45EB8B
            ),
            DEMO_TRANSFORM(
                -901, 0, 485,  -125, 989, -232,  -468, -264, -870,
                -0x7F3AA4, -0x2203E06, -0xB47CC8B
            ),
            DEMO_TRANSFORM(
                -770, -376, -558,  -70, 889, -500,  669, -339, -695,
                -0x7F03FC, -0x21B49FE, -0xB456473
            ),
            DEMO_TRANSFORM(
                -1022, -31, -19,  -11, 784, -657,  35, -657, -783,
                -0x7F2BA4, -0x21955A6, -0xB467E1F
            ),
            DEMO_TRANSFORM(
                -853, -541, -162,  -462, 837, -363,  325, -229, -942,
                -0x7D45AC, -0x218A23E, -0xB45019B
            ),
            DEMO_TRANSFORM(
                -171, -1009, 6,  -1007, 169, -57,  55, -15, -1021,
                -0x79624C, -0x21A950E, -0xB4330CB
            ),
            DEMO_TRANSFORM(
                -817, 134, -600,  177, 1007, -15,  589, -116, -828,
                -0x8108CC, -0x21A783E, -0xB47BC0B
            ),
            DEMO_TRANSFORM(
                -589, 743, -381,  442, 674, 629,  709, 198, -710,
                -0x83B8D4, -0x21E13FE, -0xB4923C3
            ),
            DEMO_TRANSFORM(
                -468, -264, -870,  -135, 989, -226,  899, 12, -487,
                -0x7CD283, -0x2210A65, -0xB414ED6
            ),
            DEMO_TRANSFORM(
                -468, -264, -870,  -135, 989, -226,  899, 12, -487,
                -0x7F91A3, -0x2229665, -0xB4668F6
            ),
        },
        .localVelocityZ = -0xCB540,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3926,
        .modelBaseYaw = -0x7EBE,
        .projectedPos = DEMO_VEC3(-0x7E9A8F, -0x215BF6B, -0xB46466A),
        .movementSpeed = 0xC463C,
        .collisionVelocityX = -0x5D152,
        .collisionVelocityZ = -0xACEDF,
        .pitchAngle = 3926,
        .facingAngle = -0x7EBE,
        .surfacePitchAngle = -170,
        .surfaceRollAngle = -9,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCB940,
        .speedLimit = 0xCB940,
        .rankSpeedBonus = 0x2540,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 360, 3360, 3880, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 272, 3768, 72, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 264, 3632, 3760, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 328, 3880, 3176, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3696, 3456, 456, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3232, 3832, 784, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 1286,
        .animationFrameCount = 36,
        .animationFramesRemaining = 19,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x85BB7B, -0x22805C7, -0xB4A4AB2),
            DEMO_VEC3(-0x7C3BD7, -0x222A6CA, -0xB38A5AE),
            DEMO_VEC3(-0x7E0B47, -0x2282910, -0xB4E75A6),
            DEMO_VEC3(-0x748BA3, -0x222CA13, -0xB3CD0A2),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(108304),
        .shadowMtx = DEMO_HEAP_MATRIX(108304 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x85BB7B, -0x2276DC5, -0xB4A4AB2),
            DEMO_VEC3(-0x7C3BD7, -0x2221445, -0xB38A5AE),
            DEMO_VEC3(-0x7E0B47, -0x2275385, -0xB4E75A6),
            DEMO_VEC3(-0x748BA3, -0x221FA05, -0xB3CD0A2),
            DEMO_VEC3(-0x7FEA29, -0x224238B, -0xB3F7BD3),
            DEMO_VEC3(-0x766A85, -0x21ECA0B, -0xB2DD6CF),
            DEMO_VEC3(-0x7839F5, -0x224094B, -0xB43A6C7),
            DEMO_VEC3(-0x6EBA51, -0x21EAFCB, -0xB3201C3),
        },
        .groundContactMask = 15,
        .coursePathIndex = 9,
        .coursePathOffset = 0x21A87DA,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(101712),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 299,
    } },
    /* Segment 2, frame 108. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x9C45701, -0x78821E2, -0x1600490C),
        .renderPosition = DEMO_VEC3(-0x9C45701, -0x788C160, -0x1600490C),
        .previousPosition = DEMO_VEC3(-0x9BE0F19, -0x781CD0C, -0x15F719B1),
        .velocity = DEMO_VEC3(-0x647E8, -0x71616, -0x92F5B),
        .groundOffset = 0x14A4D,
        .collisionBottomY = -0x78EE322,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 2767,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x660E6,
        .modeValue = 0x44000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -500, -513, -731,  -295, 885, -418,  842, 6, -581,
                -0x9C45701, -0x788C160, -0x1600490C
            ),
            DEMO_TRANSFORM(
                -755, -481, -494,  -16, 746, -700,  690, -508, -559,
                -0x9C7F4CD, -0x781C8F2, -0x1602B273
            ),
            DEMO_TRANSFORM(
                -518, -850, -237,  -878, 522, 47,  82, 227, -994,
                -0x9C6D441, -0x781FD96, -0x16011E2B
            ),
            DEMO_TRANSFORM(
                -518, -850, -237,  -10, 281, -984,  882, -495, -151,
                -0x9C36601, -0x7840816, -0x16014D2B
            ),
            DEMO_TRANSFORM(
                -572, 185, -827,  -647, 548, 572,  547, 843, -189,
                -0x9C90AE1, -0x78366B6, -0x1602913B
            ),
            DEMO_TRANSFORM(
                -759, -35, -685,  -26, 1022, -21,  685, 1, -759,
                -0x9C68321, -0x7858B36, -0x1604CD3B
            ),
            DEMO_TRANSFORM(
                -756, -563, -398,  -218, 755, -653,  653, -398, -679,
                -0x9C7F4CD, -0x781C8F2, -0x1602B273
            ),
            DEMO_TRANSFORM(
                -1012, -120, 86,  -141, 609, -810,  44, -813, -619,
                -0x9C86FB5, -0x7801FD6, -0x1604220B
            ),
            DEMO_TRANSFORM(
                -783, -657, 46,  -598, 678, -477,  275, -393, -904,
                -0x9C68965, -0x77EEF32, -0x16030073
            ),
            DEMO_TRANSFORM(
                -29, -995, 235,  -1022, 32, 12,  -18, -235, -995,
                -0x9C249DD, -0x77FFB52, -0x1601406B
            ),
            DEMO_TRANSFORM(
                -872, -99, -525,  29, 995, -235,  533, -216, -846,
                -0x9CA3AB5, -0x781AEF2, -0x1604F1F3
            ),
            DEMO_TRANSFORM(
                -710, 571, -463,  400, 841, 424,  618, 113, -807,
                -0x9CC796D, -0x785D012, -0x16054E53
            ),
            DEMO_TRANSFORM(
                -500, -513, -731,  -295, 885, -418,  842, 6, -581,
                -0x9C45BC9, -0x784A994, -0x15FEF15C
            ),
            DEMO_TRANSFORM(
                -500, -513, -731,  -295, 885, -418,  842, 6, -581,
                -0x9C749C9, -0x787AB14, -0x16033A9C
            ),
        },
        .localVelocityZ = -0xCDB60,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3754,
        .modelBaseYaw = -0x3E79,
        .modelBaseRoll = -3,
        .projectedPos = DEMO_VEC3(-0x9C7BB81, -0x77DBF22, -0x16053ECC),
        .movementSpeed = 0xB208C,
        .grabInputFrames = 4,
        .grabTrickId = 6,
        .grabHoldFrames = 19,
        .collisionVelocityX = -0x647E8,
        .collisionVelocityZ = -0x92F5B,
        .pitchAngle = 3754,
        .facingAngle = -0x3E79,
        .leanAngle = -3,
        .surfacePitchAngle = -342,
        .surfaceRollAngle = -4094,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xCDF60,
        .speedLimit = 0xCDF60,
        .rankSpeedBonus = 0x4B60,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 262, 3296, 0, 0, 0x81200, -0x19000),
            DEMO_MODEL_PART(1, 4, 154, 3459, 3421, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 1109, 3459, 3421, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2429, 3533, 1015, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 320, 3344, 3920, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 256, 3768, 104, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 256, 3624, 3768, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 336, 3872, 3176, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3672, 3456, 472, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3224, 3840, 792, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 1226,
        .animationFrameCount = 36,
        .animationFramesRemaining = 20,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x9CD01A3, -0x78EAA9F, -0x1605372E),
            DEMO_VEC3(-0x9C2DFE3, -0x7843D00, -0x15F66486),
            DEMO_VEC3(-0x9C5CE1F, -0x78EA556, -0x160A2D92),
            DEMO_VEC3(-0x9BBAC5F, -0x78437B7, -0x15FB5AEA),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141008),
        .shadowMtx = DEMO_HEAP_MATRIX(141008 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x9CD01A3, -0x78DFBA0, -0x1605372E),
            DEMO_VEC3(-0x9C2DFE3, -0x7839670, -0x15F66486),
            DEMO_VEC3(-0x9C5CE1F, -0x78DEC50, -0x160A2D92),
            DEMO_VEC3(-0x9BBAC5F, -0x7838720, -0x15FB5AEA),
            DEMO_VEC3(-0x9C6B9BB, -0x787852A, -0x15FC07D3),
            DEMO_VEC3(-0x9BC97FB, -0x77D1FFA, -0x15ED352B),
            DEMO_VEC3(-0x9BF8637, -0x78775DA, -0x1600FE37),
            DEMO_VEC3(-0x9B56477, -0x77D10AA, -0x15F22B8F),
        },
        .groundContactMask = 15,
        .coursePathIndex = 26,
        .coursePathOffset = 0x4E1DD5,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134480),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 604,
    } },
    /* Segment 3, frame 161. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xAFC8AD0, -0x867D103, -0x17BEA633),
        .renderPosition = DEMO_VEC3(-0xAFC8AD0, -0x86B239E, -0x17BEA633),
        .previousPosition = DEMO_VEC3(-0xAF541C1, -0x8636855, -0x17B4FB4F),
        .velocity = DEMO_VEC3(-0x7490F, -0x51DE2, -0x9AAE4),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x86E8637,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 3429,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x45556,
        .modeValue = 0x44000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -579, -349, -768,  -227, 961, -265,  812, 20, -621,
                -0xAFC8AD0, -0x86B239E, -0x17BEA633
            ),
            DEMO_TRANSFORM(
                -819, -331, -515,  -117, 929, -411,  601, -271, -782,
                -0xAFF3BE6, -0x861FD80, -0x17C0C68A
            ),
            DEMO_TRANSFORM(
                -722, -611, -388,  -573, 818, -221,  443, 60, -919,
                -0xAFDE3EE, -0x862A390, -0x17BF84A2
            ),
            DEMO_TRANSFORM(
                -722, -611, -388,  -207, 700, -715,  694, -426, -618,
                -0xAFBA6EE, -0x865D5D0, -0x17BEA6E2
            ),
            DEMO_TRANSFORM(
                -803, 133, -619,  -198, 896, 452,  600, 475, -677,
                -0xB004ABE, -0x8639C60, -0x17C10742
            ),
            DEMO_TRANSFORM(
                -788, 136, -637,  70, 1012, 130,  647, 56, -789,
                -0xAFF83FE, -0x8671C60, -0x17C2CBC2
            ),
            DEMO_TRANSFORM(
                -836, -345, -479,  -91, 896, -484,  583, -353, -762,
                -0xAFF3BE6, -0x861FD80, -0x17C0C68A
            ),
            DEMO_TRANSFORM(
                -1020, 48, 60,  -6, 744, -702,  -77, -700, -741,
                -0xAFF6F82, -0x860055C, -0x17C1D736
            ),
            DEMO_TRANSFORM(
                -855, -559, -53,  -519, 824, -311,  213, -233, -972,
                -0xAFD5F46, -0x85F65C0, -0x17C08D3A
            ),
            DEMO_TRANSFORM(
                -140, -1013, 28,  -1010, 141, 66,  -70, -19, -1020,
                -0xAF941BE, -0x86140E0, -0x17BF3422
            ),
            DEMO_TRANSFORM(
                -884, 102, -504,  153, 1009, -63,  491, -131, -887,
                -0xB017446, -0x8611500, -0x17C2E49A
            ),
            DEMO_TRANSFORM(
                -642, 728, -321,  508, 693, 555,  613, 189, -796,
                -0xB0436D6, -0x864C6D8, -0x17C3E06A
            ),
            DEMO_TRANSFORM(
                -579, -349, -768,  -227, 961, -265,  812, 20, -621,
                -0xAFC1B58, -0x8674FEE, -0x17BCB877
            ),
            DEMO_TRANSFORM(
                -579, -349, -768,  -227, 961, -265,  812, 20, -621,
                -0xAFF7FD8, -0x8695C2E, -0x17C13937
            ),
        },
        .localVelocityZ = -0xCE120,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = -0x3E5B,
        .modelBaseRoll = 6,
        .projectedPos = DEMO_VEC3(-0xAFF0190, -0x85C8983, -0x17C1EB33),
        .movementSpeed = 0xC1AF4,
        .grabInputFrames = 4,
        .grabTrickId = 6,
        .grabHoldFrames = 19,
        .collisionVelocityX = -0x7490F,
        .collisionVelocityZ = -0x9AAE4,
        .pitchAngle = 3869,
        .facingAngle = -0x3E5B,
        .leanAngle = 6,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -7,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xCE520,
        .speedLimit = 0xCE520,
        .rankSpeedBonus = 0x5120,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 192, 3320, 4064, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 208, 3752, 184, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 224, 3592, 3792, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 344, 3832, 3168, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3624, 3464, 528, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3200, 3864, 808, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 566,
        .animationFrameCount = 36,
        .animationFramesRemaining = 31,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xB05E18A, -0x86F5B35, -0x17C3C801),
            DEMO_VEC3(-0xAFA25FA, -0x8684143, -0x17B43569),
            DEMO_VEC3(-0xAFEEFA6, -0x86F7963, -0x17C916FD),
            DEMO_VEC3(-0xAF33416, -0x8685F71, -0x17B98465),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141008),
        .shadowMtx = DEMO_HEAP_MATRIX(141008 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xB05E18A, -0x86EC3BC, -0x17C3C801),
            DEMO_VEC3(-0xAFA25FA, -0x867AEB4, -0x17B43569),
            DEMO_VEC3(-0xAFEEFA6, -0x86E9888, -0x17C916FD),
            DEMO_VEC3(-0xAF33416, -0x8678380, -0x17B98465),
            DEMO_VEC3(-0xAFE9763, -0x86A5A56, -0x17BA1B79),
            DEMO_VEC3(-0xAF2DBD3, -0x863454E, -0x17AA88E1),
            DEMO_VEC3(-0xAF7A7AF, -0x86A254A, -0x17BF6DBD),
            DEMO_VEC3(-0xAEBEC1F, -0x8631042, -0x17AFDB25),
        },
        .groundContactMask = 15,
        .coursePathIndex = 27,
        .coursePathOffset = 0x194F4ED,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134480),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 650,
    } },
    /* Segment 4, frame 210. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xBA42EAC, -0x8CD446F, -0x189D1A9B),
        .renderPosition = DEMO_VEC3(-0xBA42EAC, -0x8D0A181, -0x189D1A9B),
        .previousPosition = DEMO_VEC3(-0xB9CE3FD, -0x8C8DAC5, -0x18936D8E),
        .velocity = DEMO_VEC3(-0x74AAF, -0x51EDE, -0x9AD0D),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x8D3F9A3,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 750,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x45651,
        .modeValue = 0x44000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -579, -349, -768,  -220, 962, -271,  814, 11, -619,
                -0xBA42EAC, -0x8D0A181, -0x189D1A9B
            ),
            DEMO_TRANSFORM(
                -820, -329, -516,  -109, 928, -416,  602, -278, -779,
                -0xBA6CE73, -0x8C778C2, -0x189F4943
            ),
            DEMO_TRANSFORM(
                -725, -609, -387,  -567, 821, -227,  446, 53, -919,
                -0xBA5788B, -0x8C81F8A, -0x189E0587
            ),
            DEMO_TRANSFORM(
                -725, -609, -387,  -200, 698, -720,  693, -434, -614,
                -0xBA3418B, -0x8CB548A, -0x189D21C7
            ),
            DEMO_TRANSFORM(
                -801, 135, -622,  -194, 899, 447,  606, 468, -678,
                -0xBA7DFEB, -0x8C9164A, -0x189F8887
            ),
            DEMO_TRANSFORM(
                -786, 139, -640,  77, 1012, 125,  650, 48, -788,
                -0xBA71DAB, -0x8CC9A0A, -0x18A147C7
            ),
            DEMO_TRANSFORM(
                -762, -624, -275,  -372, 726, -617,  571, -359, -768,
                -0xBA6CE73, -0x8C778C2, -0x189F4943
            ),
            DEMO_TRANSFORM(
                -929, -282, 322,  -428, 659, -654,  -27, -729, -717,
                -0xBA79FD7, -0x8C5E01A, -0x18A0A453
            ),
            DEMO_TRANSFORM(
                -799, -630, 103,  -612, 709, -409,  180, -382, -931,
                -0xBA5ABDB, -0x8C48732, -0x189FD1E3
            ),
            DEMO_TRANSFORM(
                -43, -1002, 205,  -1014, 68, 119,  -129, -199, -995,
                -0xBA15383, -0x8C5C2CA, -0x189E78FB
            ),
            DEMO_TRANSFORM(
                -931, -40, -423,  47, 1002, -198,  422, -200, -910,
                -0xBA9654B, -0x8C793D2, -0x18A129A3
            ),
            DEMO_TRANSFORM(
                -745, 610, -344,  468, 807, 419,  521, 148, -867,
                -0xBABDA2B, -0x8CB9792, -0x18A16BAB
            ),
            DEMO_TRANSFORM(
                -579, -349, -768,  -220, 962, -271,  814, 11, -619,
                -0xBA3BA30, -0x8CCCBD9, -0x189B3183
            ),
            DEMO_TRANSFORM(
                -579, -349, -768,  -220, 962, -271,  814, 11, -619,
                -0xBA71EB0, -0x8CED819, -0x189FB243
            ),
        },
        .localVelocityZ = -0xCE400,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = -0x3E5B,
        .projectedPos = DEMO_VEC3(-0xBA6A56C, -0x8C1FCEF, -0x18A05F9B),
        .movementSpeed = 0xC1DA8,
        .grabInputFrames = 4,
        .grabTrickId = 6,
        .grabHoldFrames = 19,
        .collisionVelocityX = -0x74AAF,
        .collisionVelocityZ = -0x9AD0D,
        .pitchAngle = 3869,
        .facingAngle = -0x3E5B,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -7,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xCE800,
        .speedLimit = 0xCE800,
        .rankSpeedBonus = 0x5400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 552, 3456, 3656, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 368, 3848, 3984, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 336, 3696, 3720, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 312, 3960, 3176, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3832, 3448, 328, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3296, 3784, 736, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 1946,
        .animationFrameCount = 36,
        .animationFramesRemaining = 8,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xBAD87DC, -0x8D4CFCD, -0x18A23E99),
            DEMO_VEC3(-0xBA1CC4C, -0x8CDB5DB, -0x1892AC01),
            DEMO_VEC3(-0xBA6910C, -0x8D4EBA4, -0x18A78935),
            DEMO_VEC3(-0xB9AD57C, -0x8CDD1B2, -0x1897F69D),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(108240),
        .shadowMtx = DEMO_HEAP_MATRIX(108240 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xBAD87DC, -0x8D43899, -0x18A23E99),
            DEMO_VEC3(-0xBA1CC4C, -0x8CD2391, -0x1892AC01),
            DEMO_VEC3(-0xBA6910C, -0x8D41F71, -0x18A78935),
            DEMO_VEC3(-0xB9AD57C, -0x8CD0A69, -0x1897F69D),
            DEMO_VEC3(-0xBA63D2D, -0x8CFCEEF, -0x1898918C),
            DEMO_VEC3(-0xB9A819D, -0x8C8B9E7, -0x1888FEF4),
            DEMO_VEC3(-0xB9F465D, -0x8CFB5C7, -0x189DDC28),
            DEMO_VEC3(-0xB938ACD, -0x8C8A0BF, -0x188E4990),
        },
        .groundContactMask = 15,
        .coursePathIndex = 27,
        .coursePathOffset = 0x2AB59C2,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(101712),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 673,
    } },
    /* Segment 5, frame 263. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x338C580, -0x2F0E655, -0xE66F2B2),
        .renderPosition = DEMO_VEC3(-0x338C580, -0x2F42001, -0xE66F2B2),
        .previousPosition = DEMO_VEC3(-0x330A4A8, -0x2EE8E32, -0xE5D68AE),
        .velocity = DEMO_VEC3(-0x820D8, -0x30D5C, -0x98A04),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x2F79B8E,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x2654,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x240E0,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -652, -186, -766,  -108, 1006, -151,  780, -15, -661,
                -0x338C580, -0x2F42001, -0xE66F2B2
            ),
            DEMO_TRANSFORM(
                -877, -165, -499,  -9, 976, -305,  525, -257, -839,
                -0x33A4F11, -0x2EA8783, -0xE67F5BB
            ),
            DEMO_TRANSFORM(
                -813, -468, -407,  -467, 904, -103,  407, 104, -932,
                -0x33902DD, -0x2EB7A83, -0xE66DB0F
            ),
            DEMO_TRANSFORM(
                -813, -468, -407,  -142, 795, -626,  603, -441, -698,
                -0x3372F1D, -0x2EF0283, -0xE66740F
            ),
            DEMO_TRANSFORM(
                -804, 302, -555,  -58, 858, 553,  629, 466, -658,
                -0x33B952D, -0x2EBF6D3, -0xE68514F
            ),
            DEMO_TRANSFORM(
                -782, 27, 659,  -98, 1006, -159,  -652, -186, -766,
                -0x33B5B2D, -0x2EF5193, -0xE6A7A4F
            ),
            DEMO_TRANSFORM(
                -894, -180, -464,  7, 948, -383,  497, -338, -827,
                -0x33A4F11, -0x2EA8783, -0xE67F5BB
            ),
            DEMO_TRANSFORM(
                -1001, 171, 123,  62, 811, -619,  -201, -599, -804,
                -0x33A4AB5, -0x2E871FB, -0xE68CD73
            ),
            DEMO_TRANSFORM(
                -923, -435, -62,  -417, 913, -194,  138, -150, -1002,
                -0x3381C71, -0x2E83C73, -0xE67937B
            ),
            DEMO_TRANSFORM(
                -272, -984, -67,  -979, 261, 138,  -115, 101, -1011,
                -0x33439D9, -0x2EABD5B, -0xE66AA3B
            ),
            DEMO_TRANSFORM(
                -883, 269, -439,  284, 982, 32,  430, -94, -923,
                -0x33C79F1, -0x2E91E13, -0xE69D77B
            ),
            DEMO_TRANSFORM(
                -553, 835, -207,  619, 557, 594,  598, 196, -806,
                -0x33FBE21, -0x2EC4C4B, -0xE6B0A93
            ),
            DEMO_TRANSFORM(
                -652, -186, -766,  -108, 1006, -151,  780, -15, -661,
                -0x337BF78, -0x2F09EF5, -0xE64A99A
            ),
            DEMO_TRANSFORM(
                -652, -186, -766,  -108, 1006, -151,  780, -15, -661,
                -0x33B9298, -0x2F1B5F5, -0xE69269A
            ),
        },
        .localVelocityZ = -0xCC000,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3977,
        .modelBaseYaw = 0x41CC,
        .modelBaseRoll = -18,
        .projectedPos = DEMO_VEC3(-0x33A2F00, -0x2E51A15, -0xE689B32),
        .movementSpeed = 0xC8853,
        .collisionVelocityX = -0x820D8,
        .collisionVelocityZ = -0x98A04,
        .pitchAngle = 3977,
        .facingAngle = 0x41CC,
        .leanAngle = -18,
        .surfacePitchAngle = -119,
        .surfaceRollAngle = -4080,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCC400,
        .speedLimit = 0xCC400,
        .rankSpeedBonus = 0x3000,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 192, 3320, 4064, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 208, 3752, 184, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 224, 3592, 3792, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 344, 3832, 3168, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3624, 3464, 528, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3200, 3864, 808, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 566,
        .animationFrameCount = 36,
        .animationFramesRemaining = 31,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x342B884, -0x2F708A5, -0xE6BE244),
            DEMO_VEC3(-0x3357E60, -0x2F33815, -0xE5C5CA4),
            DEMO_VEC3(-0x33C0CA0, -0x2F6CE01, -0xE7188C0),
            DEMO_VEC3(-0x32ED27C, -0x2F2FD71, -0xE620320),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(108240),
        .shadowMtx = DEMO_HEAP_MATRIX(108240 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x342B884, -0x2F5F1C3, -0xE6BE244),
            DEMO_VEC3(-0x3357E60, -0x2F22CE3, -0xE5C5CA4),
            DEMO_VEC3(-0x33C0CA0, -0x2F6131F, -0xE7188C0),
            DEMO_VEC3(-0x32ED27C, -0x2F24E3F, -0xE620320),
            DEMO_VEC3(-0x33A987E, -0x2F38863, -0xE625958),
            DEMO_VEC3(-0x32D5E5A, -0x2EFC383, -0xE52D3B8),
            DEMO_VEC3(-0x333EAF6, -0x2F3BAB3, -0xE67FDA4),
            DEMO_VEC3(-0x326B0D2, -0x2EFF5D3, -0xE587804),
        },
        .groundContactMask = 15,
        .coursePathIndex = 14,
        .coursePathOffset = 0x395B1F,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(101712),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 385,
    } },
    /* Segment 6, frame 312. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x23B64BC5, -0x178158F7, -0x1917F389),
        .renderPosition = DEMO_VEC3(-0x23B64BC5, -0x1781F260, -0x1917F389),
        .previousPosition = DEMO_VEC3(-0x23AE45FF, -0x177F5461, -0x19225D56),
        .velocity = DEMO_VEC3(-0x805C6, -0x2C5D6, 0xA69CD),
        .groundOffset = 0x1C5B6,
        .collisionBottomY = -0x17881A37,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x8C4D,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x212F1,
        .modeValue = 1024,
        .heldInput = B_BUTTON,
        .previousInput = B_BUTTON,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -616, -164, 800,  10, 1000, 213,  -817, 137, -601,
                -0x23B64BC5, -0x1781F260, -0x1917F389
            ),
            DEMO_TRANSFORM(
                -305, -196, 957,  -344, 959, 86,  -913, -296, -352,
                -0x23B51612, -0x1779F6AD, -0x191561C5
            ),
            DEMO_TRANSFORM(
                -273, -628, 760,  461, 615, 674,  -871, 522, 118,
                -0x23B437C6, -0x177AD8C1, -0x1916E3C1
            ),
            DEMO_TRANSFORM(
                -273, -628, 760,  -729, 659, 282,  -663, -465, -624,
                -0x23B60506, -0x177D4001, -0x19198681
            ),
            DEMO_TRANSFORM(
                -426, 490, 790,  902, 432, 218,  -229, 786, -613,
                -0x23B51CE6, -0x177B6C21, -0x19141601
            ),
            DEMO_TRANSFORM(
                -335, 286, 923,  303, 958, -188,  -918, 212, -399,
                -0x23B8A2E6, -0x177D1C21, -0x1914F001
            ),
            DEMO_TRANSFORM(
                -206, -298, 957,  -377, 928, 207,  -928, -311, -297,
                -0x23B51612, -0x1779F6AD, -0x191561C5
            ),
            DEMO_TRANSFORM(
                386, -57, 945,  -515, 843, 262,  -794, -575, 290,
                -0x23B5EA46, -0x1777ECAD, -0x1914ED0D
            ),
            DEMO_TRANSFORM(
                183, -559, 837,  46, 854, 561,  -1005, -63, 178,
                -0x23B5517A, -0x17776C6D, -0x19175025
            ),
            DEMO_TRANSFORM(
                -98, -1007, 151,  439, 95, 919,  -919, 153, 422,
                -0x23B5F252, -0x1779654D, -0x191B8CAD
            ),
            DEMO_TRANSFORM(
                -107, 273, 980,  15, 985, -274,  -1017, -14, -107,
                -0x23B653EA, -0x1778E0ED, -0x1912A3E5
            ),
            DEMO_TRANSFORM(
                21, 822, 607,  426, 545, -754,  -930, 268, -331,
                -0x23B6A65A, -0x177C0F55, -0x190F2CFD
            ),
            DEMO_TRANSFORM(
                -616, -164, 800,  10, 1000, 213,  -817, 137, -601,
                -0x23B3DBF5, -0x177EA280, -0x19193B0D
            ),
            DEMO_TRANSFORM(
                -616, -164, 800,  10, 1000, 213,  -817, 137, -601,
                -0x23B77915, -0x177F98E0, -0x19148A4D
            ),
        },
        .localVelocityZ = -0xD51DD,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3991,
        .modelBaseYaw = -0x49AC,
        .modelBaseRoll = 81,
        .projectedPos = DEMO_VEC3(-0x23B77885, -0x17758177, -0x19166D89),
        .movementSpeed = 0xD252F,
        .grabInputFrames = 5,
        .grabTrickId = 2,
        .grabHoldFrames = 37,
        .aerialTrickId = 4,
        .surfaceCueState = { 1, 0, 1, 1, 0, 1 },
        .trickAttackCompletedTrickMask = 0x10,
        .collisionVelocityX = -0x805C6,
        .collisionVelocityZ = 0xA69CD,
        .trailEffectTimer = 240,
        .pitchAngle = 3991,
        .facingAngle = -0x49AC,
        .leanAngle = 81,
        .surfacePitchAngle = -105,
        .surfaceRollAngle = -43,
        .leanScale = 32,
        .modeAngle = 34,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0x103A20,
        .speedLimit = 0xD55E0,
        .rankSpeedBonus = 0xA620,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 236, 3296, 0, 0, 0x85D00, -0x16800),
            DEMO_MODEL_PART(1, 4, 180, 3446, 3454, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 1040, 3446, 3454, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2542, 3515, 957, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 371, 3367, 3950, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 290, 3752, 100, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 227, 3661, 3776, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 308, 3876, 3163, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3690, 3524, 517, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3248, 3882, 836, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2490,
        .animationFrameDataOffset = 2490,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x23B90DDF, -0x1784C5EB, -0x190D459F),
            DEMO_VEC3(-0x23AC8E7B, -0x178170F0, -0x191D7E37),
            DEMO_VEC3(-0x23C0090F, -0x17856237, -0x191268DB),
            DEMO_VEC3(-0x23B389AB, -0x17820D3B, -0x1922A173),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141008),
        .shadowMtx = DEMO_HEAP_MATRIX(141008 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x23B90DDF, -0x178431F8, -0x190D459F),
            DEMO_VEC3(-0x23AC8E7B, -0x1780DDEC, -0x191D7E37),
            DEMO_VEC3(-0x23C0090F, -0x178306D4, -0x191268DB),
            DEMO_VEC3(-0x23B389AB, -0x177FB2C8, -0x1922A173),
            DEMO_VEC3(-0x23B10977, -0x17820DAE, -0x1917B9D0),
            DEMO_VEC3(-0x23A48A13, -0x177EB9A2, -0x1927F268),
            DEMO_VEC3(-0x23B801EB, -0x178087C2, -0x191CC844),
            DEMO_VEC3(-0x23AB8287, -0x177D33B6, -0x192D00DC),
        },
        .groundContactMask = 15,
        .coursePathIndex = 52,
        .coursePathOffset = 0xBCBC00,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134736),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_SPEED_FAN_LOOP,
        .loopSoundPitch = 0.0f,
        .speedFan = {
            .state = 1,
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x23AF13C5, -0x17800660, -0x19215389),
            .frontTransform = DEMO_TRANSFORM(
                115, 30, -150,  1, 187, 39,  153, -25, 112,
                -0x23AF13C5, -0x17800660, -0x19215389
            ),
            .backTransform = DEMO_TRANSFORM(
                115, 30, -150,  72, -175, 18,  -134, -66, -116,
                -0x23AEA5C5, -0x177E5130, -0x19218AD9
            ),
            .modelYaw = 2048,
            .spinAngleX = 1728,
            .frontMatrix = DEMO_HEAP_MATRIX(132816),
            .backMatrix = DEMO_HEAP_MATRIX(132816 + sizeof(Mtx)),
            .scaleStep = 4,
        },
        .rumbleTimer = 1,
        .rumblePatternIndex = 3,
        .replayFrame = 1330,
    } },
    /* Segment 7, frame 620. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x2B88C11C, -0x19AC0F8A, -0x11328490),
        .renderPosition = DEMO_VEC3(-0x2B88C11C, -0x19AF69E3, -0x11328490),
        .previousPosition = DEMO_VEC3(-0x2B845625, -0x19A84D76, -0x113E1442),
        .velocity = DEMO_VEC3(-0x46AF7, -0x47342, 0xB8FB2),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x19B2C0B8,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1447,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3ABFB,
        .modeValue = 0x40000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -349, -295, 915,  -93, 980, 280,  -957, 11, -362,
                -0x2B88C11C, -0x19AF69E3, -0x11328490
            ),
            DEMO_TRANSFORM(
                -7, -278, 985,  -276, 949, 266,  -985, -264, -82,
                -0x2B8906C7, -0x19A615E5, -0x112F9C68
            ),
            DEMO_TRANSFORM(
                45, -566, 851,  124, 848, 558,  -1014, 78, 106,
                -0x2B88ADBB, -0x19A6D61D, -0x113160E8
            ),
            DEMO_TRANSFORM(
                45, -566, 851,  -489, 735, 515,  -897, -429, -238,
                -0x2B892A3B, -0x19AA269D, -0x11338F28
            ),
            DEMO_TRANSFORM(
                -98, 189, 1000,  499, 885, -118,  -887, 477, -177,
                -0x2B88B32B, -0x19A7A6CD, -0x112E7E28
            ),
            DEMO_TRANSFORM(
                -92, 133, 1010,  -209, 990, -150,  -997, -220, -62,
                -0x2B8AA6EB, -0x19AB1BCD, -0x112E0768
            ),
            DEMO_TRANSFORM(
                145, -567, 839,  -322, 777, 581,  -960, -347, -67,
                -0x2B8906C7, -0x19A615E5, -0x112F9C68
            ),
            DEMO_TRANSFORM(
                754, -246, 646,  -331, 710, 657,  -606, -694, 444,
                -0x2B89BBE7, -0x19A46069, -0x112E5574
            ),
            DEMO_TRANSFORM(
                491, -606, 662,  0, 755, 691,  -897, -331, 362,
                -0x2B8A02B7, -0x19A32E7D, -0x11308678
            ),
            DEMO_TRANSFORM(
                177, -1008, -20,  641, 96, 791,  -777, -150, 647,
                -0x2B8B3597, -0x19A4A695, -0x1134D788
            ),
            DEMO_TRANSFORM(
                131, 15, 1015,  -160, 1010, 5,  -1001, -159, 132,
                -0x2B894CD7, -0x19A5F38D, -0x112C6D18
            ),
            DEMO_TRANSFORM(
                112, 662, 771,  114, 763, -672,  -1010, 160, 9,
                -0x2B885A7F, -0x19A9DC85, -0x1129F7F8
            ),
            DEMO_TRANSFORM(
                -349, -295, 915,  -93, 980, 280,  -957, 11, -362,
                -0x2B874D94, -0x19ABAF47, -0x11341CF0
            ),
            DEMO_TRANSFORM(
                -349, -295, 915,  -93, 980, 280,  -957, 11, -362,
                -0x2B895A34, -0x19AD6AE7, -0x112EBFB0
            ),
        },
        .localVelocityZ = -0xCEDFB,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3905,
        .modelBaseYaw = 0x2712,
        .projectedPos = DEMO_VEC3(-0x2B89FD9C, -0x19A092CA, -0x112F47D0),
        .movementSpeed = 0xC606B,
        .grabInputFrames = 9,
        .trickAttackCompletedTrickMask = 0x10,
        .collisionVelocityX = -0x46AF7,
        .collisionVelocityZ = 0xB8FB2,
        .pitchAngle = 3905,
        .facingAngle = 0x2712,
        .surfacePitchAngle = -191,
        .surfaceRollAngle = -40,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xD6700,
        .speedLimit = 0xCF200,
        .rankSpeedBonus = 0xD300,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3584, 3400, 640, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 544, 3440, 3672, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 360, 3840, 4000, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 328, 3688, 3720, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 312, 3960, 3176, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3816, 3448, 336, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3296, 3792, 744, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 1706,
        .animationFrameCount = 36,
        .animationFramesRemaining = 12,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x2B883502, -0x19B31233, -0x1127B20C),
            DEMO_VEC3(-0x2B811EAE, -0x19AD0A84, -0x113A3F34),
            DEMO_VEC3(-0x2B90638A, -0x19B3AB8B, -0x112AC9EC),
            DEMO_VEC3(-0x2B894D36, -0x19ADA3DC, -0x113D5714),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(139600),
        .shadowMtx = DEMO_HEAP_MATRIX(139600 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x2B883502, -0x19B275D7, -0x1127B20C),
            DEMO_VEC3(-0x2B811EAE, -0x19AC77A3, -0x113A3F34),
            DEMO_VEC3(-0x2B90638A, -0x19B25C23, -0x112AC9EC),
            DEMO_VEC3(-0x2B894D36, -0x19AC5DEF, -0x113D5714),
            DEMO_VEC3(-0x2B83CA0B, -0x19AEB3C3, -0x113341BE),
            DEMO_VEC3(-0x2B7CB3B7, -0x19A8B58F, -0x1145CEE6),
            DEMO_VEC3(-0x2B8BF893, -0x19AE9A0F, -0x1136599E),
            DEMO_VEC3(-0x2B84E23F, -0x19A89BDB, -0x1148E6C6),
        },
        .groundContactMask = 15,
        .coursePathIndex = 61,
        .coursePathOffset = 0xFACD7F,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134608),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .speedFan = {
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x2AC619EA, -0x1915DAB7, -0x131114D4),
            .frontTransform = DEMO_TRANSFORM(
                -711, 279, -681,  -538, 447, 746,  501, 877, -164,
                -0x2AC619EA, -0x1915DAB7, -0x131114D4
            ),
            .backTransform = DEMO_TRANSFORM(
                -711, 279, -681,  728, 394, -599,  98, -901, -472,
                -0x2ACD4D5A, -0x19110357, -0x130D3364
            ),
            .modelYaw = 2048,
            .spinAngleX = 0x65C0,
            .frontMatrix = DEMO_HEAP_MATRIX(100240),
            .backMatrix = DEMO_HEAP_MATRIX(100240 + sizeof(Mtx)),
            .scaleStep = 16,
        },
        .replayFrame = 1689,
    } },
    /* Segment 8, frame 674. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x2BD49D12, -0x19F95829, -0x106BFF32),
        .renderPosition = DEMO_VEC3(-0x2BD49D12, -0x19FC924A, -0x106BFF32),
        .previousPosition = DEMO_VEC3(-0x2BD01B07, -0x19F4FE86, -0x1077CB4A),
        .velocity = DEMO_VEC3(-0x4820B, -0x51AE3, 0xBCC18),
        .groundOffset = 0x3DCEF,
        .collisionBottomY = -0x1A001969,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 718,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x46516,
        .modeValue = 0x40000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -344, -340, 901,  -109, 965, 322,  -957, 11, -361,
                -0x2BD49D12, -0x19FC924A, -0x106BFF32
            ),
            DEMO_TRANSFORM(
                -2, -320, 972,  -303, 928, 305,  -977, -287, -97,
                -0x2BD4FC20, -0x19F3779D, -0x1068B38C
            ),
            DEMO_TRANSFORM(
                56, -610, 820,  137, 818, 599,  -1012, 77, 126,
                -0x2BD49C70, -0x19F42169, -0x106A7F84
            ),
            DEMO_TRANSFORM(
                56, -610, 820,  -517, 691, 549,  -881, -445, -270,
                -0x2BD52630, -0x19F753A9, -0x106CD684
            ),
            DEMO_TRANSFORM(
                -109, 158, 1005,  512, 882, -82,  -879, 494, -173,
                -0x2BD49E20, -0x19F511F9, -0x1067A684
            ),
            DEMO_TRANSFORM(
                -94, 88, 1015,  -225, 992, -107,  -993, -233, -72,
                -0x2BD69EA0, -0x19F88439, -0x106753C4
            ),
            DEMO_TRANSFORM(
                36, -333, 967,  -367, 898, 324,  -954, -358, -87,
                -0x2BD4FC20, -0x19F3779D, -0x1068B38C
            ),
            DEMO_TRANSFORM(
                590, 60, 833,  -596, 745, 368,  -585, -698, 465,
                -0x2BD5CAB4, -0x19F17E35, -0x1067FD4C
            ),
            DEMO_TRANSFORM(
                409, -548, 760,  6, 831, 596,  -937, -234, 335,
                -0x2BD5CA90, -0x19F0E5C5, -0x106A6DEC
            ),
            DEMO_TRANSFORM(
                103, -1011, 115,  590, 154, 821,  -829, -16, 599,
                -0x2BD6D080, -0x19F2CE75, -0x106E9D64
            ),
            DEMO_TRANSFORM(
                39, 114, 1016,  -140, 1008, -108,  -1013, -135, 54,
                -0x2BD59CF0, -0x19F286F5, -0x1065B52C
            ),
            DEMO_TRANSFORM(
                63, 736, 707,  199, 685, -732,  -1002, 183, -101,
                -0x2BD4F890, -0x19F62F8D, -0x1062CDC4
            ),
            DEMO_TRANSFORM(
                -344, -340, 901,  -109, 965, 322,  -957, 11, -361,
                -0x2BD3397A, -0x19F8C0FE, -0x106D6DDA
            ),
            DEMO_TRANSFORM(
                -344, -340, 901,  -109, 965, 322,  -957, 11, -361,
                -0x2BD53E3A, -0x19FAC01E, -0x1068253A
            ),
        },
        .localVelocityZ = -0xD6520,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3875,
        .modelBaseYaw = 0x2712,
        .projectedPos = DEMO_VEC3(-0x2BD60992, -0x19EE07A9, -0x10684472),
        .movementSpeed = 0xCA113,
        .grabInputFrames = 9,
        .trickAttackCompletedTrickMask = 0x10,
        .collisionVelocityX = -0x4820B,
        .collisionVelocityZ = 0xBCC18,
        .pitchAngle = 3875,
        .facingAngle = 0x2712,
        .surfacePitchAngle = -221,
        .surfaceRollAngle = -8,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xD6920,
        .speedLimit = 0xD6920,
        .rankSpeedBonus = 0xD520,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 119, 3296, 0, 0, 0x9AAC0, -0xB600),
            DEMO_MODEL_PART(1, 4, 297, 3387, 3602, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 731, 3387, 3602, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3046, 3434, 701, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3584, 3400, 640, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 192, 3320, 4064, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 208, 3752, 184, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 224, 3592, 3792, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 344, 3832, 3168, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3624, 3464, 528, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3200, 3864, 808, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 566,
        .animationFrameCount = 36,
        .animationFramesRemaining = 31,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x2BD4035A, -0x1A00A9B1, -0x106150E4),
            DEMO_VEC3(-0x2BCD0842, -0x19F9BD58, -0x107396B8),
            DEMO_VEC3(-0x2BDC31E2, -0x1A00CB73, -0x106467AC),
            DEMO_VEC3(-0x2BD536CA, -0x19F9DF19, -0x1076AD80),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(106832),
        .shadowMtx = DEMO_HEAP_MATRIX(106832 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x2BD4035A, -0x1A0012F6, -0x106150E4),
            DEMO_VEC3(-0x2BCD0842, -0x19F92B52, -0x107396B8),
            DEMO_VEC3(-0x2BDC31E2, -0x19FFF942, -0x106467AC),
            DEMO_VEC3(-0x2BD536CA, -0x19F9119E, -0x1076AD80),
            DEMO_VEC3(-0x2BCF814F, -0x19FB9AA9, -0x106D1CFC),
            DEMO_VEC3(-0x2BC88637, -0x19F4B305, -0x107F62D0),
            DEMO_VEC3(-0x2BD7AFD7, -0x19FB80F5, -0x107033C4),
            DEMO_VEC3(-0x2BD0B4BF, -0x19F49951, -0x10827998),
        },
        .groundContactMask = 15,
        .coursePathIndex = 62,
        .coursePathOffset = 0xCA7F38,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(101840),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .speedFan = {
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x2AC619EA, -0x1915DAB7, -0x131114D4),
            .frontTransform = DEMO_TRANSFORM(
                -711, 279, -681,  -538, 447, 746,  501, 877, -164,
                -0x2AC619EA, -0x1915DAB7, -0x131114D4
            ),
            .backTransform = DEMO_TRANSFORM(
                -711, 279, -681,  728, 394, -599,  98, -901, -472,
                -0x2ACD4D5A, -0x19110357, -0x130D3364
            ),
            .modelYaw = 2048,
            .spinAngleX = 0x65C0,
            .frontMatrix = DEMO_HEAP_MATRIX(100240),
            .backMatrix = DEMO_HEAP_MATRIX(100240 + sizeof(Mtx)),
            .scaleStep = 16,
        },
        .replayFrame = 1706,
    } },
    /* Segment 9, frame 723. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x773D059, -0x425D2C8, -0x129DCAF2),
        .renderPosition = DEMO_VEC3(-0x773D059, -0x42B32C8, -0x129DCAF2),
        .previousPosition = DEMO_VEC3(-0x76D0489, -0x4248C60, -0x1293DAB2),
        .velocity = DEMO_VEC3(-0x6CBD0, -0x14668, -0x9F040),
        .groundOffset = 0x60000,
        .collisionBottomY = -0x42BD2C8,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x14668,
        .modeValue = 0x44000,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -551, -302, -807,  -164, 978, -252,  846, -6, -576,
                -0x773D059, -0x42B32C8, -0x129DCAF2
            ),
            DEMO_TRANSFORM(
                -817, -281, -548,  -57, 941, -397,  613, -287, -767,
                -0x7768119, -0x420FFC8, -0x12A01402
            ),
            DEMO_TRANSFORM(
                -763, -553, -398,  -443, 857, -338,  516, -80, -879,
                -0x7753CD1, -0x421BC70, -0x129ECA32
            ),
            DEMO_TRANSFORM(
                -763, -553, -398,  -349, 830, -484,  584, -226, -808,
                -0x7738111, -0x4251570, -0x129D7772
            ),
            DEMO_TRANSFORM(
                -817, -131, -602,  -283, 967, 174,  546, 305, -808,
                -0x777A1C1, -0x4228F90, -0x12A06562
            ),
            DEMO_TRANSFORM(
                -817, -131, -602,  -178, 1006, 23,  589, 123, -826,
                -0x7768601, -0x4265710, -0x12A11362
            ),
            DEMO_TRANSFORM(
                -979, -186, 231,  -291, 764, -614,  -61, -653, -784,
                -0x7768119, -0x420FFC8, -0x12A01402
            ),
            DEMO_TRANSFORM(
                -881, 7, 518,  -425, 573, -732,  -296, -846, -491,
                -0x77724C9, -0x41F51E4, -0x12A16D62
            ),
            DEMO_TRANSFORM(
                -372, -70, 950,  47, 1017, 93,  -951, 78, -368,
                -0x774AEB9, -0x41F0D40, -0x12A1D7B2
            ),
            DEMO_TRANSFORM(
                -87, -952, 364,  -71, 371, 950,  -1017, 55, -97,
                -0x773F501, -0x422DB40, -0x12A48732
            ),
            DEMO_TRANSFORM(
                -919, -278, 352,  -274, 984, 60,  -356, -40, -958,
                -0x7797679, -0x41FF610, -0x12A0B652
            ),
            DEMO_TRANSFORM(
                -647, 733, 298,  784, 647, 114,  -106, 300, -972,
                -0x77AA259, -0x4247C20, -0x12A016C2
            ),
            DEMO_TRANSFORM(
                -551, -302, -807,  -164, 978, -252,  846, -6, -576,
                -0x7734CB9, -0x42771A8, -0x129BBEB2
            ),
            DEMO_TRANSFORM(
                -551, -302, -807,  -164, 978, -252,  846, -6, -576,
                -0x7768859, -0x42936A8, -0x12A079F2
            ),
        },
        .localVelocityZ = -0xCCF40,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = -0x3E79,
        .modelBaseRoll = -4,
        .projectedPos = DEMO_VEC3(-0x7762019, -0x41A8B48, -0x12A12BB2),
        .movementSpeed = 0xC0A3B,
        .grabInputFrames = 1,
        .collisionVelocityX = -0x6CBD0,
        .collisionVelocityZ = -0x9F040,
        .pitchAngle = 3869,
        .facingAngle = -0x3E79,
        .leanAngle = -4,
        .surfacePitchAngle = -311,
        .surfaceRollAngle = -4090,
        .leanScale = 32,
        .stateFlags = 0x389,
        .mode = 1,
        .updateState = 1,
        .targetSpeedLimit = 0xCD360,
        .speedLimit = 0xCD360,
        .rankSpeedBonus = 0x3F60,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3104, 1024, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 32, 3312, 80, 0x4000, 0xAC000, -0x10000),
            DEMO_MODEL_PART(1, 4, 288, 3392, 3720, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 400, 3392, 3720, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3488, 3328, 352, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3608, 3328, 352, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 264, 3832, 4056, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 408, 4056, 16, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 3752, 344, 3896, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 3480, 4040, 3248, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3936, 3896, 3968, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3576, 88, 520, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3104, 1024, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3104, 1024, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 4,
        .animationStartOffset = 2612,
        .animationFrameDataOffset = 2732,
        .animationFrameCount = 15,
        .animationFramesRemaining = 14,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x77D04C3, -0x438F777, -0x12A382C0),
            DEMO_VEC3(-0x771D68F, -0x42EEC88, -0x12932724),
            DEMO_VEC3(-0x775CA23, -0x438D5F0, -0x12A86EC0),
            DEMO_VEC3(-0x76A9BEF, -0x42EDDEB, -0x12981324),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140496),
        .shadowMtx = DEMO_HEAP_MATRIX(140496 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x77D04C3, -0x42E3C2E, -0x12A382C0),
            DEMO_VEC3(-0x771D68F, -0x4281C42, -0x12932724),
            DEMO_VEC3(-0x775CA23, -0x42E494E, -0x12A86EC0),
            DEMO_VEC3(-0x76A9BEF, -0x4282962, -0x12981324),
            DEMO_VEC3(-0x7762071, -0x42D7B8A, -0x12996D80),
            DEMO_VEC3(-0x76B1E55, -0x4266682, -0x12895554),
            DEMO_VEC3(-0x76EEABD, -0x42D723E, -0x129E6010),
            DEMO_VEC3(-0x763E8A1, -0x4265D36, -0x128E47E4),
        },
        .coursePathIndex = 24,
        .coursePathOffset = 0xDB62D9,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134480),
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 508,
    } },
    /* Segment 10, frame 775. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x758A14C, -0x420E3F3, -0x12760A3C),
        .renderPosition = DEMO_VEC3(-0x758A14C, -0x4202CD2, -0x12760A3C),
        .previousPosition = DEMO_VEC3(-0x751D5AF, -0x41B5ABB, -0x126C1A46),
        .velocity = DEMO_VEC3(-0x6CB9D, -0x676C9, -0x9EFF6),
        .collisionBottomY = -0x427D184,
        .groundCorrectionY = 0x1721,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x44F1B,
        .modeValue = 0x40000,
        .heldInput = A_BUTTON,
        .previousInput = A_BUTTON,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -543, -349, -794,  -195, 962, -289,  845, -1, -577,
                -0x758A14C, -0x4202CD2, -0x12760A3C
            ),
            DEMO_TRANSFORM(
                -796, -324, -554,  141, 772, -655,  626, -587, -556,
                -0x75BAA24, -0x4195C72, -0x1276F58C
            ),
            DEMO_TRANSFORM(
                -524, -813, -333,  -872, 437, 307,  -101, 441, -917,
                -0x75AABA8, -0x419D43E, -0x127558C4
            ),
            DEMO_TRANSFORM(
                -524, -813, -333,  167, 288, -967,  862, -550, -15,
                -0x75742E8, -0x41B897E, -0x12768BC4
            ),
            DEMO_TRANSFORM(
                -373, 412, -859,  -824, 322, 513,  478, 879, 214,
                -0x75D0138, -0x41AC79E, -0x1276F8A4
            ),
            DEMO_TRANSFORM(
                -847, 13, 573,  -185, 962, -296,  -543, -349, -794,
                -0x759C8B8, -0x41C0A5E, -0x1278F9E4
            ),
            DEMO_TRANSFORM(
                -962, -334, 98,  -167, 186, -991,  305, -948, -230,
                -0x75BAA24, -0x4195C72, -0x1276F58C
            ),
            DEMO_TRANSFORM(
                -966, 7, 336,  -296, 465, -861,  -160, -910, -437,
                -0x75C0814, -0x418F366, -0x12792368
            ),
            DEMO_TRANSFORM(
                -873, 184, 499,  -40, 934, -415,  -531, -374, -790,
                -0x759A43C, -0x4182E2A, -0x12792324
            ),
            DEMO_TRANSFORM(
                -179, -746, 676,  -646, 612, 504,  -772, -338, -579,
                -0x7575A34, -0x41C4792, -0x1278BC7C
            ),
            DEMO_TRANSFORM(
                -875, -411, 334,  -504, 844, -282,  -162, -407, -924,
                -0x75E570C, -0x419CFFA, -0x1278A7B4
            ),
            DEMO_TRANSFORM(
                -829, 583, -133,  583, 839, 43,  133, -40, -1013,
                -0x75E8194, -0x41E1CDA, -0x1276BBBC
            ),
            DEMO_TRANSFORM(
                -543, -349, -794,  -195, 962, -289,  845, -1, -577,
                -0x7583AEC, -0x41C54EA, -0x1274231C
            ),
            DEMO_TRANSFORM(
                -543, -349, -794,  -195, 962, -289,  845, -1, -577,
                -0x75B69CC, -0x41E612A, -0x1278CA7C
            ),
        },
        .localVelocityZ = -0xCCEE0,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = -0x3E79,
        .modelBaseRoll = -9,
        .projectedPos = DEMO_VEC3(-0x75AF10C, -0x4159C73, -0x12796AFC),
        .movementSpeed = 0xC09E1,
        .collisionVelocityX = -0x6CB9D,
        .collisionVelocityZ = -0x9EFF6,
        .pitchAngle = 3869,
        .facingAngle = -0x3E79,
        .leanAngle = -9,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -4093,
        .leanScale = 32,
        .stateFlags = 0x190,
        .mode = 2,
        .updateState = 1,
        .updateTimer = 1,
        .targetSpeedLimit = 0xCD2E0,
        .speedLimit = 0xCD2E0,
        .rankSpeedBonus = 0x3EE0,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 336, 3296, 0, 0, 0x74000, -0x20000),
            DEMO_MODEL_PART(1, 4, 80, 3496, 3328, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 1304, 3496, 3328, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2112, 3584, 1176, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 760, 3728, 3968, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 480, 3936, 56, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 16, 4088, 120, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 3928, 112, 3424, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 136, 3872, 3872, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3632, 3872, 568, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 5,
        .animationStartOffset = 3514,
        .animationFrameDataOffset = 3694,
        .animationFrameCount = 4,
        .animationFramesRemaining = 2,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x761BE92, -0x4248157, -0x127B9F3A),
            DEMO_VEC3(-0x756BC76, -0x41D6106, -0x126B870E),
            DEMO_VEC3(-0x75A8622, -0x42473D6, -0x12808D6A),
            DEMO_VEC3(-0x74F8406, -0x41D5386, -0x1270753E),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141008),
        .shadowMtx = DEMO_HEAP_MATRIX(141008 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x761BE92, -0x423B5B2, -0x127B9F3A),
            DEMO_VEC3(-0x756BC76, -0x41CA0AA, -0x126B870E),
            DEMO_VEC3(-0x75A8622, -0x423B8FA, -0x12808D6A),
            DEMO_VEC3(-0x74F8406, -0x41CA3F2, -0x1270753E),
            DEMO_VEC3(-0x75AF453, -0x41F44E4, -0x1271B0A2),
            DEMO_VEC3(-0x74FF237, -0x4182FDC, -0x12619876),
            DEMO_VEC3(-0x753B927, -0x41F5204, -0x12769C16),
            DEMO_VEC3(-0x748B70B, -0x4183CFC, -0x126683EA),
        },
        .groundContactMask = 15,
        .coursePathIndex = 24,
        .coursePathOffset = 0xAB3A22,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134480),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 504,
    } },
    /* Segment 11, frame 825. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xF6E4877, -0xB376626, -0x1F4CA4EC),
        .renderPosition = DEMO_VEC3(-0xF6E4877, -0xB3AB4A1, -0x1F4CA4EC),
        .previousPosition = DEMO_VEC3(-0xF65F76E, -0xB33ACFE, -0x1F436950),
        .velocity = DEMO_VEC3(-0x85109, -0x46E5F, -0x93B9C),
        .groundOffset = 0x40000,
        .collisionBottomY = -0xB3E1B5D,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x1185,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3A52F,
        .modeValue = 0x44000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -656, -292, -728,  -214, 980, -200,  755, 23, -689,
                -0xF6E4877, -0xB3AB4A1, -0x1F4CA4EC
            ),
            DEMO_TRANSFORM(
                -873, -279, -454,  -118, 951, -356,  520, -252, -844,
                -0xF70CFBC, -0xB31615E, -0x1F4E1AC4
            ),
            DEMO_TRANSFORM(
                -775, -568, -350,  -559, 846, -132,  363, 90, -952,
                -0xF6F6350, -0xB3221FA, -0x1F4D00C0
            ),
            DEMO_TRANSFORM(
                -775, -568, -350,  -243, 740, -661,  621, -418, -696,
                -0xF6D3390, -0xB35703A, -0x1F4C7C80
            ),
            DEMO_TRANSFORM(
                -849, 189, -538,  -125, 879, 508,  556, 488, -706,
                -0xF71F240, -0xB32F39A, -0x1F4E55D0
            ),
            DEMO_TRANSFORM(
                -836, 193, -557,  119, 1002, 169,  577, 73, -840,
                -0xF717500, -0xB36629A, -0x1F505250
            ),
            DEMO_TRANSFORM(
                -893, -263, -424,  -70, 928, -425,  494, -342, -828,
                -0xF70CFBC, -0xB31615E, -0x1F4E1AC4
            ),
            DEMO_TRANSFORM(
                -1007, 118, 135,  2, 780, -660,  -180, -650, -768,
                -0xF70F764, -0xB2F575E, -0x1F4F09D4
            ),
            DEMO_TRANSFORM(
                -882, -518, -14,  -503, 862, -222,  124, -185, -998,
                -0xF6EC4B4, -0xB2EECFE, -0x1F4DE5CC
            ),
            DEMO_TRANSFORM(
                -182, -1007, -14,  -994, 177, 163,  -157, 42, -1009,
                -0xF6AA634, -0xB31072E, -0x1F4CFE8C
            ),
            DEMO_TRANSFORM(
                -917, 174, -419,  194, 1004, -7,  409, -87, -933,
                -0xF732144, -0xB3035BE, -0x1F4FF8BC
            ),
            DEMO_TRANSFORM(
                -633, 767, -237,  576, 645, 546,  559, 204, -831,
                -0xF762124, -0xB33B516, -0x1F50F6DC
            ),
            DEMO_TRANSFORM(
                -656, -292, -728,  -214, 980, -200,  755, 23, -689,
                -0xF6D8A33, -0xB36FE19, -0x1F4A9738
            ),
            DEMO_TRANSFORM(
                -656, -292, -728,  -214, 980, -200,  755, 23, -689,
                -0xF7162F3, -0xB38B539, -0x1F4EDC58
            ),
        },
        .localVelocityZ = -0xCF820,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3907,
        .modelBaseYaw = 0x31DE,
        .modelBaseRoll = 8,
        .projectedPos = DEMO_VEC3(-0xF7093B7, -0xB2BE666, -0x1F4F30AC),
        .movementSpeed = 0xC6D1B,
        .grabInputFrames = 4,
        .grabTrickId = 6,
        .grabHoldFrames = 19,
        .collisionVelocityX = -0x85109,
        .collisionVelocityZ = -0x93B9C,
        .pitchAngle = 3907,
        .facingAngle = 0x31DE,
        .leanAngle = 8,
        .surfacePitchAngle = -189,
        .surfaceRollAngle = -14,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xCFC20,
        .speedLimit = 0xCFC20,
        .rankSpeedBonus = 0x6820,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 144, 3320, 24, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 184, 3752, 216, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 216, 3584, 3800, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 344, 3824, 3168, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3600, 3472, 544, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3192, 3864, 816, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 686,
        .animationFrameCount = 36,
        .animationFramesRemaining = 29,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xF782951, -0xB3E5E28, -0x1F5114B4),
            DEMO_VEC3(-0xF6ADBB9, -0xB386998, -0x1F425050),
            DEMO_VEC3(-0xF71B535, -0xB3E9432, -0x1F56F988),
            DEMO_VEC3(-0xF64679D, -0xB389FA1, -0x1F483524),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141008),
        .shadowMtx = DEMO_HEAP_MATRIX(141008 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xF782951, -0xB3DC561, -0x1F5114B4),
            DEMO_VEC3(-0xF6ADBB9, -0xB37D6BD, -0x1F425050),
            DEMO_VEC3(-0xF71B535, -0xB3D9285, -0x1F56F988),
            DEMO_VEC3(-0xF64679D, -0xB37A3E1, -0x1F483524),
            DEMO_VEC3(-0xF6FD776, -0xB3A0BCF, -0x1F47D800),
            DEMO_VEC3(-0xF6289DE, -0xB341D2B, -0x1F39139C),
            DEMO_VEC3(-0xF6964FE, -0xB39CE8F, -0x1F4DBF04),
            DEMO_VEC3(-0xF5C1766, -0xB33DFEB, -0x1F3EFAA0),
        },
        .groundContactMask = 15,
        .coursePathIndex = 34,
        .coursePathOffset = 0x54A0F5,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134480),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 834,
    } },
    /* Segment 12, frame 878. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1513369C, -0xD31706A, -0x2126B6DA),
        .renderPosition = DEMO_VEC3(-0x1513369C, -0xD33E326, -0x2126B6DA),
        .previousPosition = DEMO_VEC3(-0x15077B8B, -0xD2C0C6B, -0x212816FB),
        .velocity = DEMO_VEC3(-0xBBB11, -0x6253F, 0x16021),
        .groundOffset = 0x3C3E6,
        .collisionBottomY = -0xD3831AA,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0xB12A,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x57009,
        .modeValue = 0x44000,
        .heldInput = STICK_LEFT,
        .previousInput = STICK_LEFT,
        .stickX = -31,
        .stickY = -1,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -932, -409, 109,  -383, 927, 200,  -178, 141, -997,
                -0x1513369C, -0xD33E326, -0x2126B6DA
            ),
            DEMO_TRANSFORM(
                -818, -429, 439,  -468, 909, 14,  -396, -190, -924,
                -0x1516AEA5, -0xD2B4A0F, -0x21241CC0
            ),
            DEMO_TRANSFORM(
                -639, -707, 372,  -479, 719, 545,  -638, 165, -781,
                -0x1514E921, -0xD2BC567, -0x2124C5F0
            ),
            DEMO_TRANSFORM(
                -639, -707, 372,  -777, 659, -81,  -182, -333, -949,
                -0x15130961, -0xD2E9527, -0x2126E770
            ),
            DEMO_TRANSFORM(
                -928, 60, 425,  297, 822, 530,  -310, 605, -764,
                -0x15174F31, -0xD2D0727, -0x21237C50
            ),
            DEMO_TRANSFORM(
                -934, 41, 416,  102, 1009, 127,  -405, 158, -925,
                -0x151878F1, -0xD303DE7, -0x21258F10
            ),
            DEMO_TRANSFORM(
                -818, -308, 531,  -575, 694, -483,  -214, -685, -728,
                -0x1516AEA5, -0xD2B4A0F, -0x21241CC0
            ),
            DEMO_TRANSFORM(
                -560, -43, 855,  -709, 595, -434,  -478, -831, -355,
                -0x1517F215, -0xD29C367, -0x21252C70
            ),
            DEMO_TRANSFORM(
                -229, -631, 772,  -396, 784, 523,  -914, -181, -420,
                -0x1515CE6D, -0xD292DFF, -0x21265A48
            ),
            DEMO_TRANSFORM(
                2, -1021, -65,  -228, -64, 995,  -997, 12, -228,
                -0x1513B295, -0xD2AB41F, -0x212A4830
            ),
            DEMO_TRANSFORM(
                -722, 491, 531,  211, 861, -509,  -692, -249, -710,
                -0x1519CDDD, -0xD2AAF9F, -0x2123C238
            ),
            DEMO_TRANSFORM(
                -421, 914, 179,  819, 458, -407,  -444, -23, -921,
                -0x151C6555, -0xD2CDA2F, -0x212078A0
            ),
            DEMO_TRANSFORM(
                -932, -409, 109,  -383, 927, 200,  -178, 141, -997,
                -0x151179B8, -0xD30134E, -0x2125B792
            ),
            DEMO_TRANSFORM(
                -932, -409, 109,  -383, 927, 200,  -178, 141, -997,
                -0x1516EFB8, -0xD32792E, -0x212513B2
            ),
        },
        .localVelocityZ = -0xD07A4,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3828,
        .modelBaseYaw = 0x344C,
        .modelBaseRoll = 91,
        .projectedPos = DEMO_VEC3(-0x1517F95C, -0xD2670EA, -0x2126285A),
        .movementSpeed = 0xBCFA3,
        .grabInputFrames = 4,
        .grabTrickId = 6,
        .grabHoldFrames = 19,
        .collisionVelocityX = -0xBBB11,
        .collisionVelocityZ = 0x16021,
        .pitchAngle = 3828,
        .facingAngle = 0x344C,
        .leanAngle = 91,
        .surfacePitchAngle = -268,
        .surfaceRollAngle = -31,
        .leanInput = 496,
        .leanScale = 33,
        .modeAngle = 145,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xD0C20,
        .speedLimit = 0xD0C20,
        .rankSpeedBonus = 0x7820,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 124, 3296, 0, 0, 0x99BC0, -0xBE00),
            DEMO_MODEL_PART(1, 4, 292, 3390, 3595, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 745, 3390, 3595, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3023, 3438, 713, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 301, 3360, 0x10BF, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 300, 3673, 271, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 78, 3731, 3830, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 241, 3835, 3118, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3617, 3750, 771, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3277, 4072, 1032, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2490,
        .animationFrameDataOffset = 2490,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x151BE4C0, -0xD396B66, -0x21215832),
            DEMO_VEC3(-0x15090200, -0xD30F7E1, -0x21238EEE),
            DEMO_VEC3(-0x151D6B38, -0xD39F9C1, -0x2129DEC6),
            DEMO_VEC3(-0x150A8878, -0xD31AF05, -0x212C1582),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141008),
        .shadowMtx = DEMO_HEAP_MATRIX(141008 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x151BE4C0, -0xD38A322, -0x21215832),
            DEMO_VEC3(-0x15090200, -0xD305826, -0x21238EEE),
            DEMO_VEC3(-0x151D6B38, -0xD376E26, -0x2129DEC6),
            DEMO_VEC3(-0x150A8878, -0xD2F232A, -0x212C1582),
            DEMO_VEC3(-0x15102E0F, -0xD33BF14, -0x2122DBF1),
            DEMO_VEC3(-0x14FD78B3, -0xD2AFCC8, -0x2124BA7D),
            DEMO_VEC3(-0x15117E63, -0xD32C49C, -0x212B7379),
            DEMO_VEC3(-0x14FEC907, -0xD2A0250, -0x212D5205),
        },
        .groundContactMask = 15,
        .coursePathIndex = 40,
        .coursePathOffset = 0xD17B8B,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134480),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = 0.0f,
        .replayFrame = 962,
    } },
    /* Segment 13, frame 928. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1012AA71, -0xB7DFA21, -0x1FF5EF96),
        .renderPosition = DEMO_VEC3(-0x1012AA71, -0xB808D33, -0x1FF5EF96),
        .previousPosition = DEMO_VEC3(-0x10092A58, -0xB7A29F3, -0x1FEDF12E),
        .velocity = DEMO_VEC3(-0x98019, -0x46F67, -0x7FE68),
        .groundOffset = 0x40000,
        .collisionBottomY = -0xB84995A,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0xCCEE,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3B462,
        .modeValue = 0x44000,
        .previousInput = STICK_LEFT,
        .stickX = -25,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -749, -297, -630,  -342, 962, -45,  606, 177, -804,
                -0x1012AA71, -0xB808D33, -0x1FF5EF96
            ),
            DEMO_TRANSFORM(
                -911, -335, -323,  -275, 960, -218,  375, -108, -945,
                -0x1016530A, -0xB777ECA, -0x1FF5D8FD
            ),
            DEMO_TRANSFORM(
                -768, -619, -271,  -654, 783, 66,  167, 222, -984,
                -0x1014A726, -0xB782D0A, -0x1FF51B81
            ),
            DEMO_TRANSFORM(
                -768, -619, -271,  -434, 766, -519,  517, -274, -838,
                -0x10121866, -0xB7B3C4A, -0x1FF55D81
            ),
            DEMO_TRANSFORM(
                -949, 138, -355,  -113, 805, 620,  363, 615, -732,
                -0x10175296, -0xB7928DA, -0x1FF60DC1
            ),
            DEMO_TRANSFORM(
                -940, 145, -376,  50, 988, 259,  400, 220, -915,
                -0x1016E156, -0xB7C4E9A, -0x1FF87A01
            ),
            DEMO_TRANSFORM(
                -957, -277, -230,  -86, 811, -616,  350, -558, -782,
                -0x1016530A, -0xB777ECA, -0x1FF5D8FD
            ),
            DEMO_TRANSFORM(
                -994, -35, 239,  -194, 717, -702,  -144, -728, -703,
                -0x1016836A, -0xB75B676, -0x1FF733E9
            ),
            DEMO_TRANSFORM(
                -761, -625, 274,  -621, 804, 109,  -283, -85, -979,
                -0x10142772, -0xB753BE2, -0x1FF67D25
            ),
            DEMO_TRANSFORM(
                -21, -1016, -117,  -898, -37, 488,  -488, 113, -891,
                -0x100FDDDA, -0xB76D9B2, -0x1FF79675
            ),
            DEMO_TRANSFORM(
                -914, 447, -99,  457, 909, -102,  43, -136, -1013,
                -0x1018D4A2, -0xB769672, -0x1FF79D95
            ),
            DEMO_TRANSFORM(
                -464, 908, -72,  842, 458, 358,  350, 103, -955,
                -0x101CD962, -0xB790BDA, -0x1FF7752D
            ),
            DEMO_TRANSFORM(
                -749, -297, -630,  -342, 962, -45,  606, 177, -804,
                -0x1011EB25, -0xB7CFD7B, -0x1FF3A1CE
            ),
            DEMO_TRANSFORM(
                -749, -297, -630,  -342, 962, -45,  606, 177, -804,
                -0x10164F65, -0xB7EBB5B, -0x1FF753EE
            ),
        },
        .localVelocityZ = -0xCFA37,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3904,
        .modelBaseYaw = 0x3238,
        .modelBaseRoll = 111,
        .projectedPos = DEMO_VEC3(-0x10155431, -0xB727EE1, -0x1FF82D56),
        .movementSpeed = 0xC6A80,
        .grabInputFrames = 4,
        .grabTrickId = 6,
        .grabHoldFrames = 19,
        .collisionVelocityX = -0x98019,
        .collisionVelocityZ = -0x7FE68,
        .pitchAngle = 3904,
        .facingAngle = 0x3238,
        .leanAngle = 111,
        .surfacePitchAngle = -192,
        .surfaceRollAngle = -4093,
        .leanInput = 400,
        .leanScale = 32,
        .modeAngle = 114,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xCFE80,
        .speedLimit = 0xCFE80,
        .rankSpeedBonus = 0x6A80,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 321, 3362, 0x1061, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 297, 3695, 223, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 119, 3711, 3815, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 260, 3846, 3131, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3638, 3687, 700, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3269, 4019, 977, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2490,
        .animationFrameDataOffset = 2490,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x101CDA17, -0xB85098B, -0x1FF8E3AC),
            DEMO_VEC3(-0x100DAA0F, -0xB7EE3F2, -0x1FEC1BA8),
            DEMO_VEC3(-0x1017AAD3, -0xB8513CA, -0x1FFFC384),
            DEMO_VEC3(-0x10087ACB, -0xB7F0497, -0x1FF2FB80),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107728),
        .shadowMtx = DEMO_HEAP_MATRIX(107728 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x101CDA17, -0xB845255, -0x1FF8E3AC),
            DEMO_VEC3(-0x100DAA0F, -0xB7E4C59, -0x1FEC1BA8),
            DEMO_VEC3(-0x1017AAD3, -0xB82CE0D, -0x1FFFC384),
            DEMO_VEC3(-0x10087ACB, -0xB7CC811, -0x1FF2FB80),
            DEMO_VEC3(-0x101350CC, -0xB807F2A, -0x1FF10A58),
            DEMO_VEC3(-0x10044CDC, -0xB7A80F6, -0x1FE40BDC),
            DEMO_VEC3(-0x100E07D4, -0xB7EFEB6, -0x1FF7D680),
            DEMO_VEC3(-0xFFF03E4, -0xB790082, -0x1FEAD804),
        },
        .groundContactMask = 15,
        .coursePathIndex = 35,
        .coursePathOffset = 0x13FE048,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(101712),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 853,
    } },
    /* Segment 14, frame 980. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1135E268, -0xBE49CC0, -0x20987BFA),
        .renderPosition = DEMO_VEC3(-0x1135E268, -0xBE773EC, -0x20987BFA),
        .previousPosition = DEMO_VEC3(-0x112A46D8, -0xBE0DC8D, -0x209406EC),
        .velocity = DEMO_VEC3(-0xB9B90, -0x48173, -0x4750E),
        .groundOffset = 0x3E426,
        .collisionBottomY = -0xBEB5E00,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x6CFA,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3B091,
        .modeValue = 0x44000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -918, -283, -352,  -301, 977, -1,  337, 102, -960,
                -0x1135E268, -0xBE773EC, -0x20987BFA
            ),
            DEMO_TRANSFORM(
                -979, -297, -8,  -290, 961, -194,  64, -183, -1004,
                -0x1138F748, -0xBDE4385, -0x2097D942
            ),
            DEMO_TRANSFORM(
                -834, -592, 16,  -573, 815, 227,  -144, 176, -997,
                -0x11372D74, -0xBDF0081, -0x20979976
            ),
            DEMO_TRANSFORM(
                -834, -592, 16,  -544, 755, -422,  232, -353, -931,
                -0x1134EFB4, -0xBE23001, -0x20987D36
            ),
            DEMO_TRANSFORM(
                -1004, 184, -62,  115, 828, 589,  157, 571, -834,
                -0x113A0BB4, -0xBDFDFA1, -0x20979FA6
            ),
            DEMO_TRANSFORM(
                -1003, 181, -81,  166, 995, 167,  108, 151, -1005,
                -0x113A7F74, -0xBE31BE1, -0x2099ED26
            ),
            DEMO_TRANSFORM(
                -915, -450, 76,  -454, 878, -259,  48, -266, -986,
                -0x1138F748, -0xBDE4385, -0x2097D942
            ),
            DEMO_TRANSFORM(
                -751, -149, 677,  -518, 785, -401,  -461, -638, -652,
                -0x1139F6F0, -0xBDC5539, -0x20986B16
            ),
            DEMO_TRANSFORM(
                -706, -611, 417,  -635, 796, 93,  -380, -195, -929,
                -0x11379E80, -0xBDB726D, -0x20988A8A
            ),
            DEMO_TRANSFORM(
                -60, -1020, 46,  -792, 76, 642,  -644, 1, -794,
                -0x11336A40, -0xBDD1015, -0x2099EC52
            ),
            DEMO_TRANSFORM(
                -1014, 64, 118,  58, 1019, -56,  -122, -49, -1014,
                -0x113C1690, -0xBDDA5DD, -0x20982B3A
            ),
            DEMO_TRANSFORM(
                -739, 685, 171,  706, 707, 217,  27, 276, -984,
                -0x113ECB38, -0xBE1790D, -0x2097A8AA
            ),
            DEMO_TRANSFORM(
                -918, -283, -352,  -301, 977, -1,  337, 102, -960,
                -0x11345314, -0xBE3D4EC, -0x2096C0D2
            ),
            DEMO_TRANSFORM(
                -918, -283, -352,  -301, 977, -1,  337, 102, -960,
                -0x1139B414, -0xBE57E8C, -0x2098D192
            ),
        },
        .localVelocityZ = -0xCFDDC,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3913,
        .modelBaseYaw = 0x3311,
        .modelBaseRoll = 60,
        .projectedPos = DEMO_VEC3(-0x1138FCA8, -0xBD91580, -0x2099AC7A),
        .movementSpeed = 0xC6F1C,
        .grabInputFrames = 4,
        .grabTrickId = 6,
        .grabHoldFrames = 19,
        .collisionVelocityX = -0xB9B90,
        .collisionVelocityZ = -0x4750E,
        .pitchAngle = 3913,
        .facingAngle = 0x3311,
        .leanAngle = 60,
        .surfacePitchAngle = -183,
        .surfaceRollAngle = -27,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xD01E0,
        .speedLimit = 0xD01E0,
        .rankSpeedBonus = 0x6DE0,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 116, 3296, 0, 0, 0x9B240, -0xB200),
            DEMO_MODEL_PART(1, 4, 300, 3386, 3605, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 724, 3386, 3605, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3057, 3432, 695, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 392, 3368, 3848, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 288, 3776, 48, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 272, 3640, 3760, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 328, 3888, 3176, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3712, 3456, 440, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3240, 3824, 776, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 266,
        .animationFrameCount = 36,
        .animationFramesRemaining = 36,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x1140A056, -0xBEB59B6, -0x2097F3BA),
            DEMO_VEC3(-0x112E0636, -0xBE59603, -0x2090CF22),
            DEMO_VEC3(-0x113DBE9A, -0xBEBCB76, -0x20A028D2),
            DEMO_VEC3(-0x112B247A, -0xBE60802, -0x2099043A),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(139984),
        .shadowMtx = DEMO_HEAP_MATRIX(139984 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x1140A056, -0xBEAC330, -0x2097F3BA),
            DEMO_VEC3(-0x112E0636, -0xBE5033C, -0x2090CF22),
            DEMO_VEC3(-0x113DBE9A, -0xBE9E49C, -0x20A028D2),
            DEMO_VEC3(-0x112B247A, -0xBE424A8, -0x2099043A),
            DEMO_VEC3(-0x1134F096, -0xBE70F91, -0x20937AE2),
            DEMO_VEC3(-0x11226752, -0xBE1115D, -0x208C5CC6),
            DEMO_VEC3(-0x1132265E, -0xBE5F075, -0x209BB112),
            DEMO_VEC3(-0x111F9D1A, -0xBDFF241, -0x209492F6),
        },
        .groundContactMask = 15,
        .coursePathIndex = 36,
        .coursePathOffset = -0x49405,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134480),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 880,
    } },
    /* Segment 15, frame 1043. */
    { .player = {
        .playerIndex = 1,
        .characterId = RACE_CHARACTER_NANCY,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 2,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1C0C4591, -0x122D65B0, -0x1D9BA295),
        .renderPosition = DEMO_VEC3(-0x1C0C4591, -0x122D9E3A, -0x1D9BA295),
        .previousPosition = DEMO_VEC3(-0x1C0185A7, -0x1229073C, -0x1DA1AE70),
        .velocity = DEMO_VEC3(-0xABFEA, -0x51FB4, 0x60BDB),
        .groundOffset = 0x10ED1,
        .collisionBottomY = -0x123426F0,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x3647,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x46A7F,
        .modeValue = 0x44000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -838, -349, 471,  -318, 961, 144,  -492, -28, -896,
                -0x1C0C4591, -0x122D9E3A, -0x1D9BA295
            ),
            DEMO_TRANSFORM(
                -625, -315, 746,  -600, 813, -159,  -544, -535, -681,
                -0x1C0DF2FE, -0x1226218F, -0x1D991423
            ),
            DEMO_TRANSFORM(
                -394, -752, 571,  67, 594, 830,  -941, 357, -180,
                -0x1C0C4CF2, -0x1226A977, -0x1D99FA63
            ),
            DEMO_TRANSFORM(
                -394, -752, 571,  -939, 376, -153,  -97, -582, -835,
                -0x1C0C8FF2, -0x1228FC37, -0x1D9D3863
            ),
            DEMO_TRANSFORM(
                -853, 370, 425,  563, 514, 682,  33, 803, -633,
                -0x1C0E21E2, -0x12279617, -0x1D97CA83
            ),
            DEMO_TRANSFORM(
                -752, 148, 678,  116, 1012, -92,  -684, 9, -760,
                -0x1C105562, -0x12299857, -0x1D9A7483
            ),
            DEMO_TRANSFORM(
                -502, -454, 767,  -598, 824, 95,  -660, -402, -669,
                -0x1C0DF2FE, -0x1226218F, -0x1D991423
            ),
            DEMO_TRANSFORM(
                13, -67, 1020,  -734, 709, 56,  -711, -733, -38,
                -0x1C0F43CA, -0x1224520F, -0x1D98DE47
            ),
            DEMO_TRANSFORM(
                -92, -561, 851,  -450, 788, 470,  -913, -332, -317,
                -0x1C0DE49E, -0x122369CF, -0x1D9AC41B
            ),
            DEMO_TRANSFORM(
                115, -1004, 157,  -66, 150, 1010,  -1014, -124, -48,
                -0x1C0BE876, -0x12251F6F, -0x1D9EAEE3
            ),
            DEMO_TRANSFORM(
                -598, 60, 828,  -123, 1002, -162,  -821, -194, -578,
                -0x1C10581E, -0x1225A14F, -0x1D97046B
            ),
            DEMO_TRANSFORM(
                -449, 687, 609,  547, 745, -437,  -738, 134, -695,
                -0x1C1152C6, -0x1229664F, -0x1D945C2B
            ),
            DEMO_TRANSFORM(
                -838, -349, 471,  -318, 961, 144,  -492, -28, -896,
                -0x1C0A6339, -0x1229C18A, -0x1D9BEFBD
            ),
            DEMO_TRANSFORM(
                -838, -349, 471,  -318, 961, 144,  -492, -28, -896,
                -0x1C0F4D59, -0x122BCDCA, -0x1D992C1D
            ),
        },
        .localVelocityZ = -0xD1F20,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = -0x4AB2,
        .modelBaseRoll = -27,
        .projectedPos = DEMO_VEC3(-0x1C0FD711, -0x12221E30, -0x1D99A0D5),
        .movementSpeed = 0xC555A,
        .grabInputFrames = 4,
        .grabTrickId = 6,
        .grabHoldFrames = 19,
        .collisionVelocityX = -0xABFEA,
        .collisionVelocityZ = 0x60BDB,
        .pitchAngle = 3869,
        .facingAngle = -0x4AB2,
        .leanAngle = -27,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -4093,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xD2320,
        .speedLimit = 0xD2320,
        .rankSpeedBonus = 0x8F20,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 275, 3296, 0, 0, 0x7EC80, -0x1A400),
            DEMO_MODEL_PART(1, 4, 141, 3465, 3405, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 1144, 3465, 3405, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2373, 3542, 1043, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 392, 3368, 3848, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 288, 3776, 48, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 272, 3640, 3760, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 328, 3888, 3176, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3712, 3456, 440, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3240, 3824, 776, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 206,
        .animationFrameDataOffset = 266,
        .animationFrameCount = 36,
        .animationFramesRemaining = 36,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x1C12AAAD, -0x1232144B, -0x1D930629),
            DEMO_VEC3(-0x1C01ABA9, -0x122AF6D5, -0x1D9C955D),
            DEMO_VEC3(-0x1C16DF79, -0x1231FF05, -0x1D9AAFCD),
            DEMO_VEC3(-0x1C05E075, -0x122AE18F, -0x1DA43F01),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(141008),
        .shadowMtx = DEMO_HEAP_MATRIX(141008 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x1C12AAAD, -0x12310A1E, -0x1D930629),
            DEMO_VEC3(-0x1C01ABA9, -0x1229F516, -0x1D9C955D),
            DEMO_VEC3(-0x1C16DF79, -0x1231475E, -0x1D9AAFCD),
            DEMO_VEC3(-0x1C05E075, -0x122A3256, -0x1DA43F01),
            DEMO_VEC3(-0x1C07EF69, -0x122C6B30, -0x1D990FD4),
            DEMO_VEC3(-0x1BF6F065, -0x12255628, -0x1DA29F08),
            DEMO_VEC3(-0x1C0C1AE9, -0x122CC56C, -0x1DA0BDD8),
            DEMO_VEC3(-0x1BFB1BE5, -0x1225B064, -0x1DAA4D0C),
        },
        .groundContactMask = 15,
        .coursePathIndex = 47,
        .coursePathOffset = 0x30AB40,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 218,
        .modelPartMatrices = DEMO_MODEL_MATRICES(134480),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 1146,
    } },
},
{
    /* Player 2. */
    /* Segment 0, frame 0. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(0xE7B59F, -0x10319EC, -0x6224FA7),
        .renderPosition = DEMO_VEC3(0xE7B59F, -0x10526CD, -0x6224FA7),
        .previousPosition = DEMO_VEC3(0xE4A09F, -0x1021611, -0x61618C9),
        .velocity = DEMO_VEC3(0x31500, -0x1C51B, -0xC36DE),
        .groundOffset = 0x31925,
        .collisionBottomY = -0x109DB2C,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x6C44,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x10FD1,
        .heldInput = STICK_LEFT,
        .pressedInput = STICK_LEFT,
        .stickX = -31,
        .stickY = -12,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                249, -89, -988,  -66, 1015, -108,  990, 90, 241,
                0xE7B59F, -0x10526CD, -0x6224FA7
            ),
            DEMO_TRANSFORM(
                -98, -114, -1012,  -66, 1015, -108,  1016, 54, -105,
                0xE5A800, -0xFDAABE, -0x623810C
            ),
            DEMO_TRANSFORM(
                -93, -474, -902,  -522, 799, -366,  874, 427, -315,
                0xE5D530, -0xFDFEC6, -0x621F7EC
            ),
            DEMO_TRANSFORM(
                -93, -474, -902,  261, 863, -481,  984, -274, 42,
                0xE75D40, -0x1005636, -0x620E53C
            ),
            DEMO_TRANSFORM(
                -129, 198, -995,  -877, 482, 210,  510, 880, 109,
                0xE58B50, -0xFE54A6, -0x624EF1C
            ),
            DEMO_TRANSFORM(
                -990, -90, -241,  -66, 1015, -108,  249, -89, -988,
                0xE81D40, -0xFFBE86, -0x6258CCC
            ),
            DEMO_TRANSFORM(
                153, -53, -1010,  463, 911, 22,  898, -461, 160,
                0xE5A800, -0xFDAABE, -0x623810C
            ),
            DEMO_TRANSFORM(
                -643, 285, -742,  586, 815, -194,  537, -547, -676,
                0xE77780, -0xFA1AFE, -0x6236B0C
            ),
            DEMO_TRANSFORM(
                146, -822, -592,  445, 589, -708,  909, -155, 441,
                0xE6DDC8, -0xFA6B5E, -0x620F614
            ),
            DEMO_TRANSFORM(
                81, -953, -364,  -31, 362, -956,  1019, 87, 0,
                0xE4F338, -0xFAF8CA, -0x61D3654
            ),
            DEMO_TRANSFORM(
                321, 764, -599,  25, 624, 810,  971, -268, 176,
                0xE79D58, -0xFAAE8E, -0x625E584
            ),
            DEMO_TRANSFORM(
                295, 862, -463,  -258, 535, 833,  945, -123, 372,
                0xE82668, -0xFB7A22, -0x62A091C
            ),
            DEMO_TRANSFORM(
                249, -89, -988,  -66, 1015, -108,  990, 90, 241,
                0xE6B20F, -0x1025DA5, -0x61F3227
            ),
            DEMO_TRANSFORM(
                249, -89, -988,  -66, 1015, -108,  990, 90, 241,
                0xE866AF, -0x102FA45, -0x625F477
            ),
        },
        .localVelocityZ = -0xCA6BE,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 4039,
        .modelBaseYaw = 0x4F5F,
        .modelBaseRoll = 58,
        .projectedPos = DEMO_VEC3(0xE7F6DF, -0xF725EC, -0x62353E7),
        .movementSpeed = 0xC98E0,
        .collisionVelocityX = 0x31500,
        .collisionVelocityZ = -0xC36DE,
        .pitchAngle = 4039,
        .facingAngle = 0x4F5F,
        .leanAngle = 58,
        .surfacePitchAngle = -57,
        .surfaceRollAngle = -6,
        .leanInput = 384,
        .leanScale = 44,
        .modeAngle = 135,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCABA0,
        .speedLimit = 0xCABA0,
        .rankSpeedBonus = 0x17A0,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x7B040, -0x19C40),
            DEMO_MODEL_PART(1, 4, 206, 3409, 3562, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 726, 3409, 3562, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2990, 3378, 480, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 77, 3139, 282, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 197, 3737, 314, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 1189, 3623, 3132, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 850, 3796, 3137, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3142, 3695, 1211, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 2988, 3803, 1205, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2126,
        .animationFrameDataOffset = 2126,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(0xE60167, -0x10721DB, -0x62D5CA3),
            DEMO_VEC3(0xE0F31F, -0x10542CC, -0x619537F),
            DEMO_VEC3(0xEE781F, -0x107386B, -0x62B4BCF),
            DEMO_VEC3(0xE969D7, -0x105595C, -0x61742AB),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140752),
        .shadowMtx = DEMO_HEAP_MATRIX(140752 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(0xE60167, -0x10671A7, -0x62D5CA3),
            DEMO_VEC3(0xE0F31F, -0x104A15F, -0x619537F),
            DEMO_VEC3(0xEE781F, -0x105AC3B, -0x62B4BCF),
            DEMO_VEC3(0xE969D7, -0x103DBF3, -0x61742AB),
            DEMO_VEC3(0xE32ADD, -0x1055850, -0x62133A3),
            DEMO_VEC3(0xDDA7DD, -0x1038040, -0x60D4AEB),
            DEMO_VEC3(0xEB9961, -0x104F928, -0x61EE6A7),
            DEMO_VEC3(0xE61661, -0x1032118, -0x60AFDEF),
        },
        .groundContactMask = 15,
        .coursePathIndex = 5,
        .coursePathOffset = 0x36B6AE,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133584),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 190,
    } },
    /* Segment 1, frame 57. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1BD6C4, -0x2104143, -0xB30C784),
        .renderPosition = DEMO_VEC3(-0x1BD6C4, -0x2134BF7, -0xB30C784),
        .previousPosition = DEMO_VEC3(-0x13F913, -0x20C9051, -0xB277C87),
        .velocity = DEMO_VEC3(-0x7DDB1, -0x4616C, -0x94AFD),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x216F1BD,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x554C,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x391FE,
        .heldInput = STICK_LEFT,
        .previousInput = STICK_LEFT,
        .stickX = -31,
        .stickY = -2,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -633, -292, -748,  -358, 956, -69,  719, 219, -694,
                -0x1BD6C4, -0x2134BF7, -0xB30C784
            ),
            DEMO_TRANSFORM(
                -839, -349, -471,  -358, 956, -69,  464, 107, -906,
                -0x1FC13A, -0x20BFCAF, -0xB3051BB
            ),
            DEMO_TRANSFORM(
                -690, -639, -403,  -718, 724, 81,  234, 337, -936,
                -0x1E59C2, -0x20BF13F, -0xB2F9863
            ),
            DEMO_TRANSFORM(
                -690, -639, -403,  -474, 791, -442,  588, -112, -829,
                -0x1C3F12, -0x20E103F, -0xB2FD583
            ),
            DEMO_TRANSFORM(
                -911, -69, -459,  -409, 598, 722,  220, 827, -560,
                -0x20CF02, -0x20CF72F, -0xB30F9A3
            ),
            DEMO_TRANSFORM(
                -719, -219, 694,  -358, 956, -69,  -633, -292, -748,
                -0x1F9BE2, -0x20EB80F, -0xB331723
            ),
            DEMO_TRANSFORM(
                -675, -279, -716,  85, 920, -438,  763, -349, -583,
                -0x1FC13A, -0x20BFCAF, -0xB3051BB
            ),
            DEMO_TRANSFORM(
                -1018, 70, 74,  6, 784, -656,  -102, -652, -781,
                -0x1F6C3A, -0x20864AF, -0xB32083B
            ),
            DEMO_TRANSFORM(
                -160, -923, -411,  -378, 440, -842,  937, 20, -410,
                -0x1DD0AA, -0x2082927, -0xB3011C3
            ),
            DEMO_TRANSFORM(
                0, -1001, -210,  -826, 123, -589,  603, 169, -809,
                -0x1C1DD2, -0x207F89B, -0xB2C2E67
            ),
            DEMO_TRANSFORM(
                -370, 630, -715,  492, 784, 436,  817, -186, -587,
                -0x211D0A, -0x2098637, -0xB339113
            ),
            DEMO_TRANSFORM(
                -360, 750, -594,  344, 694, 668,  893, 34, -497,
                -0x23A3A6, -0x20B2A33, -0xB36902B
            ),
            DEMO_TRANSFORM(
                -633, -292, -748,  -358, 956, -69,  719, 219, -694,
                -0x1A8C74, -0x20FF627, -0xB2E63F4
            ),
            DEMO_TRANSFORM(
                -633, -292, -748,  -358, 956, -69,  719, 219, -694,
                -0x1EE184, -0x211F677, -0xB338244
            ),
        },
        .localVelocityZ = -0xCB500,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3907,
        .modelBaseYaw = 0x51CA,
        .modelBaseRoll = 147,
        .projectedPos = DEMO_VEC3(-0x1E0DC4, -0x204C183, -0xB336544),
        .movementSpeed = 0xC2CD0,
        .collisionVelocityX = -0x7DDB1,
        .collisionVelocityZ = -0x94AFD,
        .pitchAngle = 3907,
        .facingAngle = 0x51CA,
        .leanAngle = 147,
        .surfacePitchAngle = -189,
        .surfaceRollAngle = -4042,
        .leanInput = 496,
        .leanScale = 34,
        .modeAngle = 150,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCB940,
        .speedLimit = 0xCB940,
        .rankSpeedBonus = 0x2540,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 63, 3107, 311, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 194, 3722, 331, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 1269, 3636, 3098, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 903, 3806, 3123, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3054, 3709, 1310, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 2930, 3804, 1248, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2126,
        .animationFrameDataOffset = 2126,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x255582, -0x217E233, -0xB3565FA),
            DEMO_VEC3(-0x187DEE, -0x211E587, -0xB2639D6),
            DEMO_VEC3(-0x1F2F9A, -0x2175776, -0xB3B5532),
            DEMO_VEC3(-0x125806, -0x2115ACA, -0xB2C290E),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107984),
        .shadowMtx = DEMO_HEAP_MATRIX(107984 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x255582, -0x21732D1, -0xB3565FA),
            DEMO_VEC3(-0x187DEE, -0x211442D, -0xB2639D6),
            DEMO_VEC3(-0x1F2F9A, -0x21553C1, -0xB3B5532),
            DEMO_VEC3(-0x125806, -0x20F651D, -0xB2C290E),
            DEMO_VEC3(-0x1D612D, -0x2139318, -0xB2C4795),
            DEMO_VEC3(-0x10D239, -0x20DA474, -0xB1CDFC9),
            DEMO_VEC3(-0x171FED, -0x211B408, -0xB321945),
            DEMO_VEC3(-0xA90F9, -0x20BC564, -0xB22B179),
        },
        .groundContactMask = 15,
        .coursePathIndex = 9,
        .coursePathOffset = 0x1E0F2E5,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(100816),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 299,
    } },
    /* Segment 2, frame 108. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x9AB62E9, -0x7558842, -0x15AC0304),
        .renderPosition = DEMO_VEC3(-0x9AB62E9, -0x7576586, -0x15AC0304),
        .previousPosition = DEMO_VEC3(-0x9A66366, -0x74D631F, -0x15A32860),
        .velocity = DEMO_VEC3(-0x4FF83, -0x8E663, -0x8DAA4),
        .groundOffset = 0x27D44,
        .collisionBottomY = -0x75C4982,
        .targetGroundOffset = 0x40000,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x7CCE9,
        .modeValue = 0x46000,
        .stickX = -16,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -422, -555, -748,  -306, 859, -463,  880, 32, -521,
                -0x9AB62E9, -0x7576586, -0x15AC0304
            ),
            DEMO_TRANSFORM(
                -694, -534, -529,  -306, 859, -463,  686, -156, -742,
                -0x9AF0A3E, -0x751471E, -0x15AE7C5A
            ),
            DEMO_TRANSFORM(
                -536, -822, -288,  -815, 592, -170,  304, 139, -966,
                -0x9ADDFA6, -0x750EA26, -0x15AD7BEA
            ),
            DEMO_TRANSFORM(
                -536, -822, -288,  -227, 458, -884,  840, -400, -424,
                -0x9AB7C06, -0x752A6B6, -0x15ACFBDA
            ),
            DEMO_TRANSFORM(
                -765, -229, -638,  -639, 565, 564,  227, 821, -566,
                -0x9AFE826, -0x7527AD6, -0x15AF08DA
            ),
            DEMO_TRANSFORM(
                -862, -325, -445,  -348, 961, -24,  426, 130, -920,
                -0x9AE08E6, -0x75422F6, -0x15B0AFDA
            ),
            DEMO_TRANSFORM(
                -684, -475, -592,  70, 754, -686,  756, -500, -472,
                -0x9AF0A3E, -0x751471E, -0x15AE7C5A
            ),
            DEMO_TRANSFORM(
                -1021, 8, -45,  42, 572, -845,  17, -847, -573,
                -0x9AEC3BE, -0x74E545E, -0x15B12A9A
            ),
            DEMO_TRANSFORM(
                -287, -974, -122,  -533, 261, -832,  823, -170, -581,
                -0x9AD2096, -0x74D895E, -0x15AF625A
            ),
            DEMO_TRANSFORM(
                -92, -1013, 108,  -878, 23, -522,  514, -141, -872,
                -0x9AA9CCE, -0x74C9746, -0x15AC18DE
            ),
            DEMO_TRANSFORM(
                -718, 246, -684,  400, 938, -81,  608, -325, -755,
                -0x9B07886, -0x74FDC0E, -0x15B2474A
            ),
            DEMO_TRANSFORM(
                -454, 525, -750,  445, 859, 332,  801, -179, -611,
                -0x9B356FE, -0x752D02E, -0x15B35142
            ),
            DEMO_TRANSFORM(
                -422, -555, -748,  -306, 859, -463,  880, 32, -521,
                -0x9AAB0F1, -0x753665E, -0x15AA95EC
            ),
            DEMO_TRANSFORM(
                -422, -555, -748,  -306, 859, -463,  880, 32, -521,
                -0x9AD9451, -0x75732EE, -0x15AFB3CC
            ),
        },
        .localVelocityZ = -0xCDB5F,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3722,
        .modelBaseYaw = -0x2EB1,
        .modelBaseRoll = 25,
        .projectedPos = DEMO_VEC3(-0x9AE95E9, -0x74B7442, -0x15B1AE44),
        .movementSpeed = 0xA2AD8,
        .grabInputFrames = 44,
        .collisionVelocityX = -0x4FF83,
        .collisionVelocityZ = -0x8DAA4,
        .pitchAngle = 3722,
        .facingAngle = -0x2EB1,
        .leanAngle = 25,
        .surfacePitchAngle = -374,
        .surfaceRollAngle = -4054,
        .leanInput = 256,
        .leanScale = 32,
        .modeAngle = 73,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCDF60,
        .speedLimit = 0xCDF60,
        .rankSpeedBonus = 0x4B60,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x75BC0, -0x1AFC0),
            DEMO_MODEL_PART(1, 4, 177, 3423, 3534, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 795, 3423, 3534, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2897, 3385, 499, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 134, 3274, 159, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 210, 3799, 243, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 859, 3572, 3274, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 632, 3757, 3196, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3503, 3636, 801, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3225, 3798, 1030, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2126,
        .animationFrameDataOffset = 2126,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x9B36D45, -0x75E32B2, -0x15B15E52),
            DEMO_VEC3(-0x9AADD8D, -0x7534CB7, -0x15A2337A),
            DEMO_VEC3(-0x9ABE845, -0x75DD453, -0x15B5D28E),
            DEMO_VEC3(-0x9A3588D, -0x752C4B9, -0x15A6A7B6),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140688),
        .shadowMtx = DEMO_HEAP_MATRIX(140688 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x9B36D45, -0x75D2AD2, -0x15B15E52),
            DEMO_VEC3(-0x9AADD8D, -0x751E7DE, -0x15A2337A),
            DEMO_VEC3(-0x9ABE845, -0x75CE32E, -0x15B5D28E),
            DEMO_VEC3(-0x9A3588D, -0x751A03A, -0x15A6A7B6),
            DEMO_VEC3(-0x9AE2A34, -0x756A8FD, -0x15A8315C),
            DEMO_VEC3(-0x9A63B4C, -0x749F851, -0x1599D4B8),
            DEMO_VEC3(-0x9A68B80, -0x75690ED, -0x15AC7C08),
            DEMO_VEC3(-0x99E9C98, -0x749E041, -0x159E1F64),
        },
        .groundContactMask = 15,
        .coursePathIndex = 26,
        .coursePathOffset = 0x3B864F5,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133584),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 604,
    } },
    /* Segment 3, frame 161. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xAD820C3, -0x847047E, -0x1768868C),
        .renderPosition = DEMO_VEC3(-0xAD820C3, -0x8499AAC, -0x1768868C),
        .previousPosition = DEMO_VEC3(-0xAD0B98B, -0x842BB3F, -0x175EF33F),
        .velocity = DEMO_VEC3(-0x76738, -0x50A7F, -0x9934D),
        .groundOffset = 0x37F26,
        .collisionBottomY = -0x84DC5BE,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x48F8,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x45546,
        .modeValue = 0x46000,
        .stickX = -20,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -588, -348, -761,  -257, 961, -240,  796, 53, -640,
                -0xAD820C3, -0x8499AAC, -0x1768868C
            ),
            DEMO_TRANSFORM(
                -822, -345, -501,  -257, 961, -240,  552, -67, -859,
                -0xADB5374, -0x84243DA, -0x1769671E
            ),
            DEMO_TRANSFORM(
                -698, -652, -368,  -686, 757, -38,  296, 219, -953,
                -0xAD9FEE4, -0x8423A72, -0x17688D0E
            ),
            DEMO_TRANSFORM(
                -698, -652, -368,  -334, 720, -643,  669, -319, -704,
                -0xAD7FC04, -0x8447232, -0x17686FFE
            ),
            DEMO_TRANSFORM(
                -869, -47, -537,  -436, 661, 647,  317, 779, -582,
                -0xADC67C4, -0x8433D92, -0x176A052E
            ),
            DEMO_TRANSFORM(
                -949, -142, -353,  -224, 975, 211,  307, 273, -936,
                -0xADB20C4, -0x8452DB2, -0x176BEA9E
            ),
            DEMO_TRANSFORM(
                -777, -279, -604,  108, 863, -537,  656, -472, -626,
                -0xADB5374, -0x84243DA, -0x1769671E
            ),
            DEMO_TRANSFORM(
                -1011, 130, 83,  31, 716, -729,  -151, -718, -712,
                -0xADAE774, -0x83EE45A, -0x176B80DE
            ),
            DEMO_TRANSFORM(
                -374, -916, -260,  -542, 434, -749,  782, -137, -644,
                -0xAD90F2C, -0x83EA172, -0x1769C3FE
            ),
            DEMO_TRANSFORM(
                -174, -1006, -67,  -903, 186, -441,  446, -16, -920,
                -0xAD65718, -0x83E6EAA, -0x176682FA
            ),
            DEMO_TRANSFORM(
                -665, 512, -583,  549, 854, 124,  549, -233, -830,
                -0xADCDABC, -0x83FFF22, -0x176CB74E
            ),
            DEMO_TRANSFORM(
                -426, 710, -599,  547, 726, 470,  751, -125, -682,
                -0xAE02AC0, -0x8421F82, -0x176E4F6E
            ),
            DEMO_TRANSFORM(
                -588, -348, -761,  -257, 961, -240,  796, 53, -640,
                -0xAD6BE3B, -0x8461164, -0x1766823C
            ),
            DEMO_TRANSFORM(
                -588, -348, -761,  -257, 961, -240,  796, 53, -640,
                -0xADAC48B, -0x8487264, -0x176BB6DC
            ),
        },
        .localVelocityZ = -0xCE0F1,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3870,
        .modelBaseYaw = -0x2E53,
        .modelBaseRoll = 36,
        .projectedPos = DEMO_VEC3(-0xADA9F03, -0x83BBB7E, -0x176BC04C),
        .movementSpeed = 0xC1A81,
        .grabInputFrames = 44,
        .collisionVelocityX = -0x76738,
        .collisionVelocityZ = -0x9934D,
        .pitchAngle = 3870,
        .facingAngle = -0x2E53,
        .leanAngle = 36,
        .surfacePitchAngle = -226,
        .surfaceRollAngle = -9,
        .leanInput = 320,
        .leanScale = 32,
        .modeAngle = 91,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCE520,
        .speedLimit = 0xCE520,
        .rankSpeedBonus = 0x5120,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x7E780, -0x18F80),
            DEMO_MODEL_PART(1, 4, 225, 3400, 3581, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 681, 3400, 3581, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3052, 3373, 468, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 117, 3235, 195, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 206, 3781, 264, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 955, 3587, 3232, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 695, 3769, 3179, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3398, 3653, 920, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3156, 3799, 1081, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2126,
        .animationFrameDataOffset = 2126,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xAE17F47, -0x84E0903, -0x176D81B0),
            DEMO_VEC3(-0xAD59123, -0x846EF3A, -0x175E1368),
            DEMO_VEC3(-0xADAB063, -0x84E3AC2, -0x1772F9B0),
            DEMO_VEC3(-0xACEC23F, -0x84720F9, -0x17638B68),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140688),
        .shadowMtx = DEMO_HEAP_MATRIX(140688 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xAE17F47, -0x84D5B44, -0x176D81B0),
            DEMO_VEC3(-0xAD59123, -0x8464E04, -0x175E1368),
            DEMO_VEC3(-0xADAB063, -0x84CE754, -0x1772F9B0),
            DEMO_VEC3(-0xACEC23F, -0x845DA14, -0x17638B68),
            DEMO_VEC3(-0xADA110D, -0x84903EE, -0x17640EA1),
            DEMO_VEC3(-0xACE4F01, -0x841EEE6, -0x17548139),
            DEMO_VEC3(-0xAD32415, -0x848C9F6, -0x17696545),
            DEMO_VEC3(-0xAC76209, -0x841B4EE, -0x1759D7DD),
        },
        .groundContactMask = 15,
        .coursePathIndex = 27,
        .coursePathOffset = 0x1398474,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133584),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 650,
    } },
    /* Segment 4, frame 210. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xB893FF2, -0x8ABBC52, -0x183FAC9E),
        .renderPosition = DEMO_VEC3(-0xB893FF2, -0x8AF16C6, -0x183FAC9E),
        .previousPosition = DEMO_VEC3(-0xB8187F2, -0x8A754BE, -0x183653E6),
        .velocity = DEMO_VEC3(-0x7B800, -0x51CC9, -0x958B8),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x8B27187,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1420,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x4517B,
        .modeValue = 0x46000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -613, -348, -742,  -221, 963, -268,  789, 0, -652,
                -0xB893FF2, -0x8AF16C6, -0x183FAC9E
            ),
            DEMO_TRANSFORM(
                -843, -327, -479,  -221, 963, -268,  537, -117, -863,
                -0xB8C2C47, -0x8A767A2, -0x1840DC3C
            ),
            DEMO_TRANSFORM(
                -739, -615, -348,  -618, 807, -112,  342, 128, -955,
                -0xB8AD477, -0x8A76532, -0x184006FC
            ),
            DEMO_TRANSFORM(
                -739, -615, -348,  -331, 746, -615,  624, -332, -739,
                -0xB890467, -0x8A9C272, -0x183FB26C
            ),
            DEMO_TRANSFORM(
                -878, -43, -523,  -375, 765, 565,  368, 677, -673,
                -0xB8D4CB7, -0x8A85AD2, -0x18416E6C
            ),
            DEMO_TRANSFORM(
                -959, -115, -336,  -182, 990, 181,  305, 230, -949,
                -0xB8C3347, -0x8AA9922, -0x1843168C
            ),
            DEMO_TRANSFORM(
                -938, -284, -293,  -95, 868, -533,  397, -462, -822,
                -0xB8C2C47, -0x8A767A2, -0x1840DC3C
            ),
            DEMO_TRANSFORM(
                -977, 81, 293,  -145, 741, -689,  -267, -700, -696,
                -0xB8C8B87, -0x8A403A2, -0x1842F1BC
            ),
            DEMO_TRANSFORM(
                -447, -869, -303,  -775, 537, -396,  496, 56, -893,
                -0xB8A3537, -0x8A3BEA2, -0x1841F774
            ),
            DEMO_TRANSFORM(
                -176, -1004, -94,  -996, 188, -137,  152, 68, -1009,
                -0xB867EC7, -0x8A403CA, -0x183FEC78
            ),
            DEMO_TRANSFORM(
                -951, 36, -375,  127, 990, -225,  355, -255, -924,
                -0xB8ECA07, -0x8A521A2, -0x184366A4
            ),
            DEMO_TRANSFORM(
                -647, 659, -439,  631, 772, 228,  479, -126, -895,
                -0xB911D2F, -0x8A8AF62, -0x18434F94
            ),
            DEMO_TRANSFORM(
                -613, -348, -742,  -221, 963, -268,  789, 0, -652,
                -0xB87B202, -0x8AB8C66, -0x183DCACE
            ),
            DEMO_TRANSFORM(
                -613, -348, -742,  -221, 963, -268,  789, 0, -652,
                -0xB8BE2C2, -0x8ADED66, -0x1842DDBE
            ),
        },
        .localVelocityZ = -0xCE3FF,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3870,
        .modelBaseYaw = -0x2E3E,
        .projectedPos = DEMO_VEC3(-0xB8BD872, -0x8A07352, -0x1842D15E),
        .movementSpeed = 0xC1F2C,
        .grabInputFrames = 44,
        .collisionVelocityX = -0x7B800,
        .collisionVelocityZ = -0x958B8,
        .pitchAngle = 3870,
        .facingAngle = -0x2E3E,
        .surfacePitchAngle = -226,
        .surfaceRollAngle = -17,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCE800,
        .speedLimit = 0xCE800,
        .rankSpeedBonus = 0x5400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 184, 3432, 16, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 232, 3872, 144, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 680, 3528, 3224, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 520, 3752, 3128, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3816, 3480, 392, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3376, 3776, 864, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 1342,
        .animationFrameCount = 30,
        .animationFramesRemaining = 12,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xB92D56E, -0x8B34799, -0x184468B4),
            DEMO_VEC3(-0xB86697E, -0x8AC310B, -0x18355E48),
            DEMO_VEC3(-0xB8C1666, -0x8B3847C, -0x1849FAF4),
            DEMO_VEC3(-0xB7FAA76, -0x8AC6DF0, -0x183AF088),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107920),
        .shadowMtx = DEMO_HEAP_MATRIX(107920 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xB92D56E, -0x8B29D66, -0x184468B4),
            DEMO_VEC3(-0xB86697E, -0x8AB9026, -0x18355E48),
            DEMO_VEC3(-0xB8C1666, -0x8B29D66, -0x1849FAF4),
            DEMO_VEC3(-0xB7FAA76, -0x8AB9026, -0x183AF088),
            DEMO_VEC3(-0xB8B1D6E, -0x8AE35D2, -0x183B0FFC),
            DEMO_VEC3(-0xB7EB17E, -0x8A72892, -0x182C0590),
            DEMO_VEC3(-0xB845E66, -0x8AE35D2, -0x1840A23C),
            DEMO_VEC3(-0xB77F276, -0x8A72892, -0x183197D0),
        },
        .groundContactMask = 15,
        .coursePathIndex = 27,
        .coursePathOffset = 0x24F6144,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(100816),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 673,
    } },
    /* Segment 5, frame 263. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x33325EB, -0x2E4F727, -0xE040684),
        .renderPosition = DEMO_VEC3(-0x33325EB, -0x2E70CE3, -0xE040684),
        .previousPosition = DEMO_VEC3(-0x329EE33, -0x2E31993, -0xDFB6F78),
        .velocity = DEMO_VEC3(-0x937B8, -0x29ED4, -0x8970C),
        .groundOffset = 0x33E7C,
        .collisionBottomY = -0x2EBB867,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x88C0,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x1D937,
        .heldInput = STICK_RIGHT,
        .previousInput = STICK_RIGHT,
        .stickX = 31,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -740, -153, -689,  -28, 1004, -193,  706, -120, -731,
                -0x33325EB, -0x2E70CE3, -0xE040684
            ),
            DEMO_TRANSFORM(
                -934, -103, -403,  -28, 1004, -193,  415, -165, -920,
                -0x3347565, -0x2DF3E6D, -0xE045A14
            ),
            DEMO_TRANSFORM(
                -873, -448, -285,  -487, 895, 86,  211, 209, -978,
                -0x3331375, -0x2DF951D, -0xE03AABC
            ),
            DEMO_TRANSFORM(
                -873, -448, -285,  -206, 791, -613,  490, -467, -766,
                -0x331A5D5, -0x2E2348D, -0xE03EB6C
            ),
            DEMO_TRANSFORM(
                -905, 204, -429,  -194, 683, 736,  433, 733, -566,
                -0x335D055, -0x2DFE2ED, -0xE04D92C
            ),
            DEMO_TRANSFORM(
                -706, 120, 731,  -28, 1004, -193,  -740, -153, -689,
                -0x3353EA5, -0x2E1E32D, -0xE07015C
            ),
            DEMO_TRANSFORM(
                -955, -338, 139,  -358, 942, -172,  -71, -210, -998,
                -0x3347565, -0x2DF3E6D, -0xE045A14
            ),
            DEMO_TRANSFORM(
                -651, -79, 784,  -502, 826, -333,  -607, -598, -565,
                -0x335DBA5, -0x2DB906D, -0xE050614
            ),
            DEMO_TRANSFORM(
                -331, -929, 272,  -802, 424, 472,  -541, -61, -865,
                -0x33359AD, -0x2DB32ED, -0xE054774
            ),
            DEMO_TRANSFORM(
                -34, -1022, -21,  -682, 7, 762,  -761, 39, -682,
                -0x32FC411, -0x2DAEFED, -0xE078A30
            ),
            DEMO_TRANSFORM(
                -971, 92, 308,  40, 1007, -176,  -319, -155, -959,
                -0x338040D, -0x2DCD96D, -0xE0499B4
            ),
            DEMO_TRANSFORM(
                -756, 654, 216,  668, 774, -6,  -168, 137, -999,
                -0x33A0F2D, -0x2E05B5D, -0xE035AB4
            ),
            DEMO_TRANSFORM(
                -740, -153, -689,  -28, 1004, -193,  706, -120, -731,
                -0x330AFB3, -0x2E4129B, -0xE02239C
            ),
            DEMO_TRANSFORM(
                -740, -153, -689,  -28, 1004, -193,  706, -120, -731,
                -0x335BF23, -0x2E51F3B, -0xE06DAAC
            ),
        },
        .localVelocityZ = -0xCBF8F,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3998,
        .modelBaseYaw = -0x5DE9,
        .modelBaseRoll = -78,
        .projectedPos = DEMO_VEC3(-0x33476AB, -0x2D91A67, -0xE054004),
        .movementSpeed = 0xC9989,
        .collisionVelocityX = -0x937B8,
        .collisionVelocityZ = -0x8970C,
        .pitchAngle = 3998,
        .facingAngle = -0x5DE9,
        .leanAngle = -78,
        .surfacePitchAngle = -98,
        .surfaceRollAngle = -4094,
        .leanInput = -496,
        .leanScale = 32,
        .modeAngle = -141,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCC400,
        .speedLimit = 0xCC400,
        .rankSpeedBonus = 0x3000,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x7C3C0, -0x197C0),
            DEMO_MODEL_PART(1, 4, 213, 3406, 3569, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3406, 3569, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3012, 3376, 476, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 196, 3649, -282, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 308, 0x101D, -134, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 130, 3971, 3279, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, -14, 3973, 2999, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 4052, 3777, 43, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3669, 3900, 450, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 2,
        .animationStartOffset = 2004,
        .animationFrameDataOffset = 2004,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x33DAA21, -0x2E9CC23, -0xE07E406),
            DEMO_VEC3(-0x32EAA15, -0x2E6B001, -0xDF9E9F2),
            DEMO_VEC3(-0x337A1C1, -0x2E9BEF0, -0xE0E2316),
            DEMO_VEC3(-0x328A1B5, -0x2E6A2CD, -0xE002902),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107920),
        .shadowMtx = DEMO_HEAP_MATRIX(107920 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x33DAA21, -0x2E816F5, -0xE07E406),
            DEMO_VEC3(-0x32EAA15, -0x2E4FAAD, -0xDF9E9F2),
            DEMO_VEC3(-0x337A1C1, -0x2E91F19, -0xE0E2316),
            DEMO_VEC3(-0x328A1B5, -0x2E602D1, -0xE002902),
            DEMO_VEC3(-0x33481D9, -0x2E67840, -0xDFF1924),
            DEMO_VEC3(-0x32544D9, -0x2E35BF8, -0xDF15FE8),
            DEMO_VEC3(-0x32E978D, -0x2E7394C, -0xE057F08),
            DEMO_VEC3(-0x31F5A8D, -0x2E41D04, -0xDF7C5CC),
        },
        .groundContactMask = 15,
        .coursePathIndex = 13,
        .coursePathOffset = 0xB3DB12,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(100816),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 385,
    } },
    /* Segment 6, frame 312. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x239B7C38, -0x1783663A, -0x18CC7060),
        .renderPosition = DEMO_VEC3(-0x239B7C38, -0x1785495D, -0x18CC7060),
        .previousPosition = DEMO_VEC3(-0x23909FE9, -0x17813528, -0x18D41D4B),
        .velocity = DEMO_VEC3(-0xADC4F, -0x2F252, 0x7ACEB),
        .groundOffset = 0x28323,
        .collisionBottomY = -0x178A277A,
        .targetGroundOffset = 0x40000,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x23F78,
        .modeValue = 1024,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -823, -175, 582,  -145, 1008, 97,  -590, -4, -836,
                -0x239B7C38, -0x1785495D, -0x18CC7060
            ),
            DEMO_TRANSFORM(
                -577, -163, 829,  -145, 1008, 97,  -832, -63, -591,
                -0x239B9003, -0x177E0509, -0x18CA5DA7
            ),
            DEMO_TRANSFORM(
                -455, -539, 741,  111, 788, 642,  -909, 365, -291,
                -0x239AA573, -0x177E45C9, -0x18CBA0D7
            ),
            DEMO_TRANSFORM(
                -455, -539, 741,  -671, 758, 140,  -623, -423, -691,
                -0x239AF913, -0x178094C9, -0x18CD8287
            ),
            DEMO_TRANSFORM(
                -580, 167, 825,  778, 490, 446,  -322, 881, -406,
                -0x239C5633, -0x177EC069, -0x18C932B7
            ),
            DEMO_TRANSFORM(
                -505, 223, 861,  438, 924, 16,  -774, 376, -552,
                -0x239E9E43, -0x17802FE9, -0x18CA81C7
            ),
            DEMO_TRANSFORM(
                -394, -134, 934,  -422, 930, -45,  -844, -404, -413,
                -0x239B9003, -0x177E0509, -0x18CA5DA7
            ),
            DEMO_TRANSFORM(
                231, 164, 982,  -593, 833, 0,  -800, -570, 284,
                -0x239D36C3, -0x177A6289, -0x18CA8AA7
            ),
            DEMO_TRANSFORM(
                -429, -726, 579,  -328, 715, 653,  -868, 88, -533,
                -0x239C0BAB, -0x177A8319, -0x18CCCD3F
            ),
            DEMO_TRANSFORM(
                -230, -938, 336,  -127, 369, 945,  -988, 170, -200,
                -0x239A016B, -0x177BB629, -0x18D0530F
            ),
            DEMO_TRANSFORM(
                -387, 189, 927,  -112, 986, -249,  -940, -196, -352,
                -0x239DF82B, -0x177B2A99, -0x18C83CCF
            ),
            DEMO_TRANSFORM(
                -373, 775, 553,  317, 661, -713,  -898, -88, -481,
                -0x239E5017, -0x177E6879, -0x18C5837F
            ),
            DEMO_TRANSFORM(
                -823, -175, 582,  -145, 1008, 97,  -590, -4, -836,
                -0x23990660, -0x178239B5, -0x18CE3100
            ),
            DEMO_TRANSFORM(
                -823, -175, 582,  -145, 1008, 97,  -590, -4, -836,
                -0x239EA7F0, -0x17836C65, -0x18CA3680
            ),
        },
        .localVelocityZ = -0xD8000,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3984,
        .modelBaseYaw = 0x6591,
        .modelBaseRoll = -3,
        .projectedPos = DEMO_VEC3(-0x239D2938, -0x177793FA, -0x18CB4160),
        .movementSpeed = 0xD4C8E,
        .grabInputFrames = 3,
        .grabTrickId = 2,
        .grabHoldFrames = 23,
        .aerialTrickId = 3,
        .trickAttackCompletedTrickMask = 0x8,
        .collisionVelocityX = -0xADC4F,
        .collisionVelocityZ = 0x7ACEB,
        .trailEffectTimer = 240,
        .pitchAngle = 3984,
        .facingAngle = 0x6591,
        .leanAngle = -3,
        .surfacePitchAngle = -112,
        .surfaceRollAngle = -10,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0x101400,
        .speedLimit = 0xD8400,
        .rankSpeedBonus = 0x8000,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x75F00, -0x1AF00),
            DEMO_MODEL_PART(1, 4, 178, 3422, 3535, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 793, 3422, 3535, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2901, 3385, 498, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3368, 3448, 480, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 192, 3432, 16, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 232, 3872, 152, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 608, 3512, 3304, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 472, 3736, 3176, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3864, 3504, 360, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3424, 3776, 848, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 1282,
        .animationFrameCount = 30,
        .animationFramesRemaining = 13,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x23A14F82, -0x1787B3DC, -0x18C2F82A),
            DEMO_VEC3(-0x23909E4E, -0x17841F34, -0x18CEC34A),
            DEMO_VEC3(-0x23A65A22, -0x1787D741, -0x18CA1D76),
            DEMO_VEC3(-0x2395A8EE, -0x17844299, -0x18D5E896),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140688),
        .shadowMtx = DEMO_HEAP_MATRIX(140688 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x23A14F82, -0x17870AFF, -0x18C2F82A),
            DEMO_VEC3(-0x23909E4E, -0x17837DE3, -0x18CEC34A),
            DEMO_VEC3(-0x23A65A22, -0x178714D7, -0x18CA1D76),
            DEMO_VEC3(-0x2395A8EE, -0x178387BB, -0x18D5E896),
            DEMO_VEC3(-0x23967333, -0x1784B689, -0x18CAA515),
            DEMO_VEC3(-0x2385C1FF, -0x1781296D, -0x18D67035),
            DEMO_VEC3(-0x239B7DD3, -0x1784C061, -0x18D1CA61),
            DEMO_VEC3(-0x238ACC9F, -0x17813345, -0x18DD9581),
        },
        .groundContactMask = 15,
        .coursePathIndex = 52,
        .coursePathOffset = 0xCA26AA,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133840),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_SPEED_FAN_LOOP,
        .loopSoundPitch = 0.0f,
        .speedFan = {
            .state = 1,
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x2391D738, -0x17833C5D, -0x18D34260),
            .frontTransform = DEMO_TRANSFORM(
                462, 98, -327,  -81, 567, 54,  331, 2, 470,
                -0x2391D738, -0x17833C5D, -0x18D34260
            ),
            .backTransform = DEMO_TRANSFORM(
                462, 98, -327,  336, -54, 462,  48, -564, -99,
                -0x2390D748, -0x177E082D, -0x18D3FED0
            ),
            .modelYaw = 2048,
            .spinAngleX = 0x1440,
            .frontMatrix = DEMO_HEAP_MATRIX(132688),
            .backMatrix = DEMO_HEAP_MATRIX(132688 + sizeof(Mtx)),
            .scaleStep = 10,
        },
        .rumbleTimer = 1,
        .rumblePatternIndex = 9,
        .replayFrame = 1330,
    } },
    /* Segment 7, frame 620. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x2D086D21, -0x1AC28096, -0xE63FFA0),
        .renderPosition = DEMO_VEC3(-0x2D086D21, -0x1AC38456, -0xE63FFA0),
        .previousPosition = DEMO_VEC3(-0x2D086D21, -0x1AC36316, -0xE63FFA0),
        .velocity = DEMO_VEC3(0, 0x2140, 0),
        .groundOffset = 0x1A3C0,
        .collisionBottomY = -0x1AC941D6,
        .targetGroundOffset = 0x40000,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = 0x7FFFFFFF,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                958, -23, 358,  359, 101, -953,  -13, 1018, 102,
                -0x2D0B5949, -0x1AC1BDD6, -0xE5BF9CC
            ),
            DEMO_TRANSFORM(
                959, -35, 355,  351, 273, -921,  -62, 985, 268,
                -0x2D08752D, -0x1AC0AD5E, -0xE63A0F0
            ),
            DEMO_TRANSFORM(
                792, -122, 636,  617, -160, -800,  195, 1003, -50,
                -0x2D0A08BD, -0x1AC0C25E, -0xE63B318
            ),
            DEMO_TRANSFORM(
                792, -122, 636,  612, 468, -673,  -209, 902, 435,
                -0x2D0BD77D, -0x1AC049CE, -0xE615AE8
            ),
            DEMO_TRANSFORM(
                1019, 42, 84,  91, -212, -996,  -22, 1000, -214,
                -0x2D07397D, -0x1AC0DCCE, -0xE62A878
            ),
            DEMO_TRANSFORM(
                1019, 42, 84,  51, 525, -876,  -79, 877, 520,
                -0x2D077E4D, -0x1AC03DCE, -0xE5FBCE8
            ),
            DEMO_TRANSFORM(
                959, -35, 355,  351, 273, -921,  -62, 985, 268,
                -0x2D08752D, -0x1AC0AD5E, -0xE63A0F0
            ),
            DEMO_TRANSFORM(
                307, 776, 591,  308, 510, -831,  -925, 428, -80,
                -0x2D0715AD, -0x1ABF9B9E, -0xE673A30
            ),
            DEMO_TRANSFORM(
                4, 881, 520,  981, -151, 247,  290, 497, -846,
                -0x2D0998FD, -0x1ABFA7CE, -0xE67A538
            ),
            DEMO_TRANSFORM(
                -231, 904, 420,  850, -45, 567,  520, 477, -741,
                -0x2D0D3349, -0x1AC0D246, -0xE699140
            ),
            DEMO_TRANSFORM(
                393, -884, -331,  -645, 9, -794,  689, 514, -553,
                -0x2D04EA3D, -0x1ABFD3DE, -0xE65E8D8
            ),
            DEMO_TRANSFORM(
                247, -913, -389,  -954, -107, -354,  275, 449, -877,
                -0x2D01C7F9, -0x1AC19702, -0xE63A638
            ),
            DEMO_TRANSFORM(
                958, -23, 358,  359, 101, -953,  -13, 1018, 102,
                -0x2D0DBF39, -0x1AC16A36, -0xE5F8754
            ),
            DEMO_TRANSFORM(
                958, -23, 358,  359, 101, -953,  -13, 1018, 102,
                -0x2D073249, -0x1AC192E6, -0xE5D1384
            ),
        },
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3919,
        .modelBaseYaw = 0x572B,
        .modelBaseRoll = 9,
        .projectedPos = DEMO_VEC3(-0x2D097521, -0x1AB6F1D6, -0xE60F2E0),
        .grabInputFrames = 3,
        .grabTrickId = 2,
        .grabHoldFrames = 23,
        .aerialTrickId = 3,
        .trickAttackCompletedTrickMask = 0x8,
        .pitchAngle = 3919,
        .facingAngle = 0x572B,
        .leanAngle = 9,
        .surfacePitchAngle = -177,
        .surfaceRollAngle = -4079,
        .leanScale = 33,
        .modeAngle = 32,
        .stateFlags = 0xA180,
        .mode = 8,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xD0140,
        .speedLimit = 0x78800,
        .rankSpeedBonus = 0x6D40,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 2960, 4072, 0, -0x4000, 0x40000, 0x7C000),
            DEMO_MODEL_PART(0, 1, 3072, 4072, 4088, 0, 0x84000, 0x4000),
            DEMO_MODEL_PART(1, 4, 2808, 3856, 4088, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 3224, 3856, 4088, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2760, 160, 4088, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3248, 160, 4088, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 3072, 4072, 4088, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 3136, 3944, 704, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 2072, 3944, 920, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 3920, 2184, 3136, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 2408, 120, 3256, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 2048, 120, 3152, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 2960, 4072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 2960, 4072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 17,
        .animationStartOffset = 0x1EAE,
        .animationFrameDataOffset = 0x1F26,
        .animationFrameCount = 28,
        .animationFramesRemaining = 27,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x2D019423, -0x1AC6ED63, -0xE58C7AC),
            DEMO_VEC3(-0x2D15005B, -0x1AC625DF, -0xE600CB0),
            DEMO_VEC3(-0x2D01B237, -0x1AC72E33, -0xE57E6E8),
            DEMO_VEC3(-0x2D151E6F, -0x1AC664F9, -0xE5F2BEC),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(139280),
        .shadowMtx = DEMO_HEAP_MATRIX(139280 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x2D019423, -0x1AC65334, -0xE58C7AC),
            DEMO_VEC3(-0x2D15005B, -0x1AC5DBE4, -0xE600CB0),
            DEMO_VEC3(-0x2D01B237, -0x1ABD9FC8, -0xE57E6E8),
            DEMO_VEC3(-0x2D151E6F, -0x1ABD2878, -0xE5F2BEC),
            DEMO_VEC3(-0x2D012851, -0x1AC65504, -0xE595450),
            DEMO_VEC3(-0x2D14D945, -0x1AC5B964, -0xE5FD050),
            DEMO_VEC3(-0x2D012F6D, -0x1ABDA198, -0xE586C70),
            DEMO_VEC3(-0x2D14E061, -0x1ABD05F8, -0xE5EE870),
        },
        .groundContactMask = 3,
        .coursePathIndex = 66,
        .coursePathOffset = 0x508FEE,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133712),
        .loopSoundId = SFX_SPEED_FAN_LOOP,
        .loopSoundPitch = 0.0f,
        .speedFan = {
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x2D120872, -0x1AB693B5, -0xE6A1A6D),
            .frontTransform = DEMO_TRANSFORM(
                -919, 352, -277,  -115, 425, 923,  433, 861, -342,
                -0x2D120872, -0x1AB693B5, -0xE6A1A6D
            ),
            .backTransform = DEMO_TRANSFORM(
                -919, 352, -277,  -72, 506, 885,  441, 815, -430,
                -0x2D166192, -0x1AB1A805, -0xE633B0D
            ),
            .modelYaw = 2048,
            .spinAngleX = 0x3040,
            .frontMatrix = DEMO_HEAP_MATRIX(132688),
            .backMatrix = DEMO_HEAP_MATRIX(132688 + sizeof(Mtx)),
            .scaleStep = 16,
        },
        .replayFrame = 1689,
    } },
    /* Segment 8, frame 674. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x2D086D21, -0x1AC02456, -0xE63FFA0),
        .renderPosition = DEMO_VEC3(-0x2D086D21, -0x1AC37F9F, -0xE63FFA0),
        .previousPosition = DEMO_VEC3(-0x2D086D21, -0x1AC02456, -0xE63FFA0),
        .velocity = DEMO_VEC3(0, -0xA000, 0),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x1AC6C456,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1207,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = 0x7FFFFFFF,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                37, 213, 1000,  987, 254, -90,  -268, 967, -196,
                -0x2D100BD1, -0x1AB8FC93, -0xE603364
            ),
            DEMO_TRANSFORM(
                104, 43, 1017,  870, 526, -111,  -528, 876, 16,
                -0x2D095D51, -0x1AB702B7, -0xE60DE78
            ),
            DEMO_TRANSFORM(
                -186, -135, 997,  1006, -45, 182,  20, 1013, 141,
                -0x2D09F131, -0x1AB754C7, -0xE624E00
            ),
            DEMO_TRANSFORM(
                -186, -135, 997,  545, 838, 215,  -845, 571, -81,
                -0x2D0CE3E1, -0x1AB73277, -0xE62D6B0
            ),
            DEMO_TRANSFORM(
                410, 231, 908,  899, -377, -310,  263, 922, -354,
                -0x2D09A301, -0x1AB73457, -0xE5F5310
            ),
            DEMO_TRANSFORM(
                410, 231, 908,  681, 607, -463,  -644, 790, 89,
                -0x2D0C45A1, -0x1AB61997, -0xE5E6A60
            ),
            DEMO_TRANSFORM(
                60, -214, 999,  1022, 32, -54,  -19, 1000, 216,
                -0x2D095D51, -0x1AB702B7, -0xE60DE78
            ),
            DEMO_TRANSFORM(
                54, -269, 986,  1009, -140, -94,  160, 977, 258,
                -0x2D055F51, -0x1AB6E2B7, -0xE6114B8
            ),
            DEMO_TRANSFORM(
                4, 798, 640,  -455, -571, 716,  916, -287, 353,
                -0x2D0604B9, -0x1AB6607F, -0xE637E50
            ),
            DEMO_TRANSFORM(
                458, 913, -55,  -479, 292, 855,  780, -357, 558,
                -0x2D045B75, -0x1AB5D7D7, -0xE675D90
            ),
            DEMO_TRANSFORM(
                454, -912, -86,  -435, -128, -917,  807, 444, -444,
                -0x2D05B969, -0x1AB76CEF, -0xE5E9D90
            ),
            DEMO_TRANSFORM(
                -312, -711, -666,  -620, 685, -440,  751, 269, -639,
                -0x2D033DFD, -0x1AB8BC9B, -0xE5B6CC8
            ),
            DEMO_TRANSFORM(
                37, 213, 1000,  987, 254, -90,  -268, 967, -196,
                -0x2D0DC351, -0x1AB917E3, -0xE63D78C
            ),
            DEMO_TRANSFORM(
                37, 213, 1000,  987, 254, -90,  -268, 967, -196,
                -0x2D0D8291, -0x1AB7A323, -0xE5D00AC
            ),
        },
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3919,
        .modelBaseYaw = 0x572B,
        .projectedPos = DEMO_VEC3(-0x2D097521, -0x1AB49596, -0xE60F2E0),
        .grabInputFrames = 3,
        .grabTrickId = 2,
        .grabHoldFrames = 23,
        .aerialTrickId = 3,
        .trickAttackCompletedTrickMask = 0x8,
        .pitchAngle = 3919,
        .facingAngle = 0x572B,
        .surfacePitchAngle = -177,
        .surfaceRollAngle = -4079,
        .leanScale = 33,
        .modeAngle = 32,
        .stateFlags = 0xA180,
        .mode = 8,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xD0360,
        .speedLimit = 0x87600,
        .rankSpeedBonus = 0x6F60,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 2640, 3464, 592, -0x60000, 0xBC000, 0x30000),
            DEMO_MODEL_PART(0, 1, 3000, 3416, 392, 0, 0x70000, 0x4000),
            DEMO_MODEL_PART(1, 4, 2520, 3200, 504, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 3184, 3200, 504, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2448, 3656, 360, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3128, 3656, 360, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 2960, 3320, 72, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 2936, 3312, 4072, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 944, 3856, 880, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 2808, 1760, 2768, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 2512, 4080, 3336, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 2320, 208, 2712, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 2640, 3464, 592, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 2640, 3464, 592, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 17,
        .animationStartOffset = 0x1EAE,
        .animationFrameDataOffset = 0x2322,
        .animationFrameCount = 28,
        .animationFramesRemaining = 10,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x2D0E8613, -0x1AC8BB62, -0xE553A58),
            DEMO_VEC3(-0x2D0F474F, -0x1AC33242, -0xE697F24),
            DEMO_VEC3(-0x2D10D053, -0x1AC867E1, -0xE56E7A4),
            DEMO_VEC3(-0x2D11918F, -0x1AC2E4F4, -0xE6B2C70),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(106512),
        .shadowMtx = DEMO_HEAP_MATRIX(106512 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x2D0E8613, -0x1ABAF695, -0xE553A58),
            DEMO_VEC3(-0x2D0F474F, -0x1ABF4785, -0xE697F24),
            DEMO_VEC3(-0x2D10D053, -0x1AB2B1A1, -0xE56E7A4),
            DEMO_VEC3(-0x2D11918F, -0x1AB70291, -0xE6B2C70),
            DEMO_VEC3(-0x2D0DD725, -0x1ABAEA11, -0xE558BCE),
            DEMO_VEC3(-0x2D1391E9, -0x1ABECF5D, -0xE691726),
            DEMO_VEC3(-0x2D0B4429, -0x1AB2E519, -0xE57E702),
            DEMO_VEC3(-0x2D10FEED, -0x1AB6CA65, -0xE6B725A),
        },
        .coursePathIndex = 66,
        .coursePathOffset = 0x508FEE,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(100944),
        .loopSoundId = SFX_SPEED_FAN_LOOP,
        .loopSoundPitch = 0.0f,
        .speedFan = {
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x2D120872, -0x1AB693B5, -0xE6A1A6D),
            .frontTransform = DEMO_TRANSFORM(
                -919, 352, -277,  -115, 425, 923,  433, 861, -342,
                -0x2D120872, -0x1AB693B5, -0xE6A1A6D
            ),
            .backTransform = DEMO_TRANSFORM(
                -919, 352, -277,  -72, 506, 885,  441, 815, -430,
                -0x2D166192, -0x1AB1A805, -0xE633B0D
            ),
            .modelYaw = 2048,
            .spinAngleX = 0x3040,
            .frontMatrix = DEMO_HEAP_MATRIX(132688),
            .backMatrix = DEMO_HEAP_MATRIX(132688 + sizeof(Mtx)),
            .scaleStep = 16,
        },
        .replayFrame = 1706,
    } },
    /* Segment 9, frame 723. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x766A57B, -0x4158B6C, -0x12466F83),
        .renderPosition = DEMO_VEC3(-0x766A57B, -0x414EB6C, -0x12466F83),
        .previousPosition = DEMO_VEC3(-0x75EF414, -0x411285C, -0x123D2D01),
        .velocity = DEMO_VEC3(-0x7B167, -0x51845, -0x94282),
        .collisionBottomY = -0x41C40A1,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x44F47,
        .modeValue = 0x46000,
        .heldInput = A_BUTTON,
        .previousInput = A_BUTTON,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -614, -349, -740,  -219, 962, -271,  788, -4, -652,
                -0x766A57B, -0x414EB6C, -0x12466F83
            ),
            DEMO_TRANSFORM(
                -844, -327, -477,  -219, 962, -271,  535, -121, -863,
                -0x769787B, -0x40F45EC, -0x1246C063
            ),
            DEMO_TRANSFORM(
                -619, -768, -270,  -804, 525, 349,  -123, 423, -922,
                -0x768207B, -0x40F4364, -0x1245EB93
            ),
            DEMO_TRANSFORM(
                -619, -768, -270,  -123, 423, -922,  804, -526, -349,
                -0x765C4EB, -0x410CDB4, -0x1246F1E3
            ),
            DEMO_TRANSFORM(
                -867, 73, -536,  -531, 76, 870,  103, 1018, -26,
                -0x76A99AB, -0x4103904, -0x12475153
            ),
            DEMO_TRANSFORM(
                -788, 4, 652,  -219, 962, -271,  -614, -349, -740,
                -0x7690B0B, -0x4107264, -0x1249DE63
            ),
            DEMO_TRANSFORM(
                -885, -511, -36,  -164, 351, -946,  485, -813, -386,
                -0x769787B, -0x40F45EC, -0x1246C063
            ),
            DEMO_TRANSFORM(
                -873, 94, 524,  -328, 697, -672,  -419, -742, -564,
                -0x76A1CBB, -0x40DE6AC, -0x124A72E3
            ),
            DEMO_TRANSFORM(
                -653, -5, 787,  -373, 903, -303,  -692, -481, -578,
                -0x767DEC3, -0x40CD2BC, -0x1249E5EB
            ),
            DEMO_TRANSFORM(
                -378, -451, 836,  -515, 854, 227,  -798, -337, -542,
                -0x7653A93, -0x4101EA8, -0x124A537B
            ),
            DEMO_TRANSFORM(
                -983, -211, -188,  -74, 850, -564,  272, -528, -831,
                -0x76C31A3, -0x40F526C, -0x124A133B
            ),
            DEMO_TRANSFORM(
                -802, 375, -512,  517, 865, -175,  367, -395, -868,
                -0x76DD7E3, -0x412D948, -0x1248607F
            ),
            DEMO_TRANSFORM(
                -614, -349, -740,  -219, 962, -271,  788, -4, -652,
                -0x765149B, -0x4116034, -0x124491B3
            ),
            DEMO_TRANSFORM(
                -614, -349, -740,  -219, 962, -271,  788, -4, -652,
                -0x769486B, -0x413C3D4, -0x1249A0B3
            ),
        },
        .localVelocityZ = -0xCCF5F,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = -0x2E3C,
        .modelBaseRoll = -3,
        .projectedPos = DEMO_VEC3(-0x769433B, -0x40A43EC, -0x124995C3),
        .movementSpeed = 0xC09DB,
        .collisionVelocityX = -0x7B167,
        .collisionVelocityZ = -0x94282,
        .pitchAngle = 3869,
        .facingAngle = -0x2E3C,
        .leanAngle = -3,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -18,
        .leanScale = 32,
        .stateFlags = 0x190,
        .mode = 2,
        .updateState = 1,
        .updateTimer = 1,
        .targetSpeedLimit = 0xCD360,
        .speedLimit = 0xCD360,
        .rankSpeedBonus = 0x3F60,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x60000, -0x20000),
            DEMO_MODEL_PART(1, 4, 56, 3480, 3416, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 1080, 3480, 3416, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2512, 3416, 576, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 776, 3576, 3832, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 296, 4024, 88, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 88, 112, 4048, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 4000, 144, 3704, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 168, 3512, 64, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3624, 3584, 752, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 5,
        .animationStartOffset = 3150,
        .animationFrameDataOffset = 3390,
        .animationFrameCount = 4,
        .animationFramesRemaining = 1,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x7703E23, -0x4191966, -0x124B24F1),
            DEMO_VEC3(-0x763C91F, -0x411FEC6, -0x123C2631),
            DEMO_VEC3(-0x76981D7, -0x419578C, -0x1250B8D5),
            DEMO_VEC3(-0x75D0CD3, -0x4123CEC, -0x1241BA15),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140176),
        .shadowMtx = DEMO_HEAP_MATRIX(140176 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x7703E23, -0x418714A, -0x124B24F1),
            DEMO_VEC3(-0x763C91F, -0x4115C42, -0x123C2631),
            DEMO_VEC3(-0x76981D7, -0x4187A96, -0x1250B8D5),
            DEMO_VEC3(-0x75D0CD3, -0x411658E, -0x1241BA15),
            DEMO_VEC3(-0x7688CBC, -0x4140E3A, -0x1241E26F),
            DEMO_VEC3(-0x75C17B8, -0x40CF932, -0x1232E3AF),
            DEMO_VEC3(-0x761D070, -0x4141786, -0x12477653),
            DEMO_VEC3(-0x7555B6C, -0x40D027E, -0x12387793),
        },
        .groundContactMask = 15,
        .coursePathIndex = 24,
        .coursePathOffset = 0x8C4D39,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133584),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 508,
    } },
    /* Segment 10, frame 775. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x747E052, -0x403FF6E, -0x12216605),
        .renderPosition = DEMO_VEC3(-0x747E052, -0x4035F6E, -0x12216605),
        .previousPosition = DEMO_VEC3(-0x7402F38, -0x3FF9C89, -0x121823E0),
        .velocity = DEMO_VEC3(-0x7B11A, -0x51815, -0x94225),
        .collisionBottomY = -0x40AB49E,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x44F1B,
        .modeValue = 0x44000,
        .heldInput = A_BUTTON,
        .previousInput = A_BUTTON,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -614, -349, -740,  -216, 962, -273,  788, -7, -652,
                -0x747E052, -0x4035F6E, -0x12216605
            ),
            DEMO_TRANSFORM(
                -844, -326, -477,  -216, 962, -273,  535, -124, -863,
                -0x74AAFD2, -0x3FDB84E, -0x1221BA45
            ),
            DEMO_TRANSFORM(
                -621, -767, -269,  -803, 528, 348,  -121, 422, -923,
                -0x7495822, -0x3FDB626, -0x1220E51D
            ),
            DEMO_TRANSFORM(
                -621, -767, -269,  -121, 422, -923,  803, -528, -347,
                -0x746FD52, -0x3FF4286, -0x1221EA1D
            ),
            DEMO_TRANSFORM(
                -867, 74, -537,  -532, 79, 870,  105, 1017, -28,
                -0x74BD152, -0x3FEAB06, -0x12224B0D
            ),
            DEMO_TRANSFORM(
                -788, 7, 652,  -216, 962, -273,  -614, -349, -740,
                -0x74A4252, -0x3FEE6A6, -0x1224D7BD
            ),
            DEMO_TRANSFORM(
                -886, -509, -36,  -162, 349, -947,  483, -815, -384,
                -0x74AAFD2, -0x3FDB84E, -0x1221BA45
            ),
            DEMO_TRANSFORM(
                -873, 97, 523,  -326, 696, -674,  -420, -742, -563,
                -0x74B5252, -0x3FC5A8E, -0x12256D85
            ),
            DEMO_TRANSFORM(
                -653, -2, 786,  -370, 903, -305,  -693, -480, -578,
                -0x74913F2, -0x3FB47AE, -0x1224E09D
            ),
            DEMO_TRANSFORM(
                -380, -448, 837,  -513, 855, 225,  -799, -336, -543,
                -0x74671BA, -0x3FE9576, -0x12254BB1
            ),
            DEMO_TRANSFORM(
                -983, -209, -189,  -71, 849, -565,  272, -531, -830,
                -0x74D6812, -0x3FDC4DE, -0x12250D9D
            ),
            DEMO_TRANSFORM(
                -800, 376, -513,  519, 863, -176,  367, -398, -867,
                -0x74F10AA, -0x40149AA, -0x122359B9
            ),
            DEMO_TRANSFORM(
                -614, -349, -740,  -216, 962, -273,  788, -7, -652,
                -0x7464DE2, -0x3FFD436, -0x121F8975
            ),
            DEMO_TRANSFORM(
                -614, -349, -740,  -216, 962, -273,  788, -7, -652,
                -0x74A81B2, -0x40237D6, -0x12249875
            ),
        },
        .localVelocityZ = -0xCCEDF,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = -0x2E3C,
        .modelBaseRoll = -5,
        .projectedPos = DEMO_VEC3(-0x74A7E12, -0x3F8B7EE, -0x12248C45),
        .movementSpeed = 0xC0963,
        .collisionVelocityX = -0x7B11A,
        .collisionVelocityZ = -0x94225,
        .pitchAngle = 3869,
        .facingAngle = -0x2E3C,
        .leanAngle = -5,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -18,
        .leanScale = 32,
        .stateFlags = 0x190,
        .mode = 2,
        .updateState = 1,
        .updateTimer = 1,
        .targetSpeedLimit = 0xCD2E0,
        .speedLimit = 0xCD2E0,
        .rankSpeedBonus = 0x3EE0,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x60000, -0x20000),
            DEMO_MODEL_PART(1, 4, 56, 3480, 3416, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 1080, 3480, 3416, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2512, 3416, 576, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 776, 3576, 3832, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 296, 4024, 88, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 88, 112, 4048, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 4000, 144, 3704, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 168, 3512, 64, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3624, 3584, 752, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 5,
        .animationStartOffset = 3150,
        .animationFrameDataOffset = 3390,
        .animationFrameCount = 4,
        .animationFramesRemaining = 1,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x7517986, -0x4078DC4, -0x12261C45),
            DEMO_VEC3(-0x7450482, -0x4007324, -0x12171D85),
            DEMO_VEC3(-0x74ABC22, -0x407CB31, -0x122BAE85),
            DEMO_VEC3(-0x73E471E, -0x400B091, -0x121CAFC5),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140688),
        .shadowMtx = DEMO_HEAP_MATRIX(140688 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x7517986, -0x406E1BE, -0x12261C45),
            DEMO_VEC3(-0x7450482, -0x3FFCCB6, -0x12171D85),
            DEMO_VEC3(-0x74ABC22, -0x406F226, -0x122BAE85),
            DEMO_VEC3(-0x73E471E, -0x3FFDD1E, -0x121CAFC5),
            DEMO_VEC3(-0x749C8B2, -0x4027D7B, -0x121CDA66),
            DEMO_VEC3(-0x73D53AE, -0x3FB6873, -0x120DDBA6),
            DEMO_VEC3(-0x7430AC2, -0x402909F, -0x12226C1A),
            DEMO_VEC3(-0x73695BE, -0x3FB7B97, -0x12136D5A),
        },
        .groundContactMask = 15,
        .coursePathIndex = 24,
        .coursePathOffset = 0x5C4FB9,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133584),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 504,
    } },
    /* Segment 11, frame 825. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xF7E8C1B, -0xB222586, -0x1EDBB09D),
        .renderPosition = DEMO_VEC3(-0xF7E8C1B, -0xB21FE4B, -0x1EDBB09D),
        .previousPosition = DEMO_VEC3(-0xF762984, -0xB1E8C83, -0x1ED285A7),
        .velocity = DEMO_VEC3(-0x86297, -0x45A43, -0x92AF6),
        .groundOffset = 0x17264,
        .collisionBottomY = -0xB28E6C6,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0xF99F,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3A50C,
        .modeValue = 0x46000,
        .heldInput = STICK_LEFT,
        .previousInput = STICK_LEFT,
        .stickX = -31,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -661, -294, -723,  -352, 958, -67,  696, 205, -721,
                -0xF7E8C1B, -0xB21FE4B, -0x1EDBB09D
            ),
            DEMO_TRANSFORM(
                -857, -346, -438,  -352, 958, -67,  433, 94, -922,
                -0xF821EF4, -0xB1C0177, -0x1EDADA8A
            ),
            DEMO_TRANSFORM(
                -624, -733, -346,  -804, 505, 379,  -100, 502, -885,
                -0xF80B154, -0xB1BF76F, -0x1EDA2DBA
            ),
            DEMO_TRANSFORM(
                -624, -733, -346,  -377, 648, -694,  717, -296, -666,
                -0xF7E5654, -0xB1D728F, -0x1EDB4A2A
            ),
            DEMO_TRANSFORM(
                -932, 8, -421,  -415, 147, 923,  68, 1012, -130,
                -0xF833474, -0xB1CFB1F, -0x1EDB769A
            ),
            DEMO_TRANSFORM(
                -976, -176, -248,  -260, 916, 374,  158, 420, -919,
                -0xF81FD34, -0xB1D69BF, -0x1EDE2B0A
            ),
            DEMO_TRANSFORM(
                -723, -274, -669,  64, 918, -445,  720, -357, -632,
                -0xF821EF4, -0xB1C0177, -0x1EDADA8A
            ),
            DEMO_TRANSFORM(
                -1012, 73, 126,  -25, 782, -658,  -145, -654, -773,
                -0xF81DE34, -0xB186AF7, -0x1EDC980A
            ),
            DEMO_TRANSFORM(
                -207, -924, -387,  -451, 438, -806,  894, 7, -496,
                -0xF8021F4, -0xB18325F, -0x1EDABDC2
            ),
            DEMO_TRANSFORM(
                -32, -1003, -198,  -869, 130, -523,  538, 151, -856,
                -0xF7E130C, -0xB17FF93, -0x1ED70826
            ),
            DEMO_TRANSFORM(
                -455, 629, -665,  529, 787, 382,  747, -174, -676,
                -0xF83AAA4, -0xB19892F, -0x1EDE02F2
            ),
            DEMO_TRANSFORM(
                -401, 751, -565,  407, 693, 632,  848, 23, -571,
                -0xF867F08, -0xB1B309B, -0x1EE0B686
            ),
            DEMO_TRANSFORM(
                -661, -294, -723,  -352, 958, -67,  696, 205, -721,
                -0xF7D2563, -0xB1EA5C3, -0x1ED9616D
            ),
            DEMO_TRANSFORM(
                -661, -294, -723,  -352, 958, -67,  696, 205, -721,
                -0xF81AB73, -0xB20A8B3, -0x1EDE538D
            ),
        },
        .localVelocityZ = -0xCF7A6,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3906,
        .modelBaseYaw = -0x5E1D,
        .modelBaseRoll = 138,
        .projectedPos = DEMO_VEC3(-0xF80DF9B, -0xB16A746, -0x1EDE3B9D),
        .movementSpeed = 0xC6C93,
        .grabInputFrames = 44,
        .collisionVelocityX = -0x86297,
        .collisionVelocityZ = -0x92AF6,
        .pitchAngle = 3906,
        .facingAngle = -0x5E1D,
        .leanAngle = 138,
        .surfacePitchAngle = -194,
        .surfaceRollAngle = -7,
        .leanInput = 496,
        .leanScale = 32,
        .modeAngle = 141,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCFC20,
        .speedLimit = 0xCFC20,
        .rankSpeedBonus = 0x6820,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x6C980, -0x1D180),
            DEMO_MODEL_PART(1, 4, 126, 3447, 3484, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 915, 3447, 3484, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2735, 3398, 531, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 72, 3126, 293, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 196, 3731, 321, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 1221, 3628, 3118, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 871, 3800, 3132, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3107, 3700, 1250, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 2965, 3804, 1222, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2126,
        .animationFrameDataOffset = 2126,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xF883A97, -0xB26A72B, -0x1EDFF099),
            DEMO_VEC3(-0xF7AD1C3, -0xB2083CD, -0x1ED14771),
            DEMO_VEC3(-0xF824673, -0xB26D4BE, -0x1EE619C9),
            DEMO_VEC3(-0xF74DD9F, -0xB20E1A1, -0x1ED770A1),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140688),
        .shadowMtx = DEMO_HEAP_MATRIX(140688 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xF883A97, -0xB25DA8B, -0x1EDFF099),
            DEMO_VEC3(-0xF7AD1C3, -0xB1FE41F, -0x1ED14771),
            DEMO_VEC3(-0xF824673, -0xB241877, -0x1EE619C9),
            DEMO_VEC3(-0xF74DD9F, -0xB1E220B, -0x1ED770A1),
            DEMO_VEC3(-0xF7FC646, -0xB221DAD, -0x1ED6F037),
            DEMO_VEC3(-0xF729BB2, -0xB1C2F09, -0x1EC80CB3),
            DEMO_VEC3(-0xF79B756, -0xB205B0D, -0x1EDCFE9B),
            DEMO_VEC3(-0xF6C8CC2, -0xB1A6C69, -0x1ECE1B17),
        },
        .groundContactMask = 15,
        .coursePathIndex = 34,
        .coursePathOffset = 0x15B515,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133584),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 834,
    } },
    /* Segment 12, frame 878. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1514F862, -0xD323C92, -0x20E8CA92),
        .renderPosition = DEMO_VEC3(-0x1514F862, -0xD352C63, -0x20E8CA92),
        .previousPosition = DEMO_VEC3(-0x15094634, -0xD2CCB8F, -0x20EA8C11),
        .velocity = DEMO_VEC3(-0xBB22E, -0x63243, 0x1C17F),
        .groundOffset = 0x3A471,
        .collisionBottomY = -0xD38FDD2,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x14A0,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x56671,
        .modeValue = 0x46000,
        .stickX = -6,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -931, -402, 139,  -395, 941, 71,  -156, 11, -1011,
                -0x1514F862, -0xD352C63, -0x20E8CA92
            ),
            DEMO_TRANSFORM(
                -823, -382, 472,  -395, 941, 71,  -460, -124, -905,
                -0x1517D095, -0xD2DDA53, -0x20E6B53F
            ),
            DEMO_TRANSFORM(
                -631, -673, 441,  -433, 757, 534,  -678, 141, -752,
                -0x15166A45, -0xD2DC0AB, -0x20E76F37
            ),
            DEMO_TRANSFORM(
                -631, -673, 441,  -759, 683, -43,  -266, -355, -921,
                -0x151524F5, -0xD2FF86B, -0x20E8FFB7
            ),
            DEMO_TRANSFORM(
                -895, -91, 486,  288, 721, 666,  -401, 720, -605,
                -0x1518D415, -0xD2EDF5B, -0x20E60D37
            ),
            DEMO_TRANSFORM(
                -772, -170, 649,  9, 987, 269,  -670, 209, -743,
                -0x1519AC45, -0xD30FC7B, -0x20E800B7
            ),
            DEMO_TRANSFORM(
                -782, -320, 577,  -532, 834, -258,  -389, -498, -804,
                -0x1517D095, -0xD2DDA53, -0x20E6B53F
            ),
            DEMO_TRANSFORM(
                -316, 89, 969,  -688, 699, -288,  -687, -741, -155,
                -0x1519E4D5, -0xD2A9813, -0x20E7B7FF
            ),
            DEMO_TRANSFORM(
                -431, -854, 362,  -773, 551, 379,  -511, -114, -878,
                -0x1517B98D, -0xD2A3833, -0x20E90097
            ),
            DEMO_TRANSFORM(
                -141, -986, 232,  -629, 269, 760,  -793, -37, -643,
                -0x15140CE1, -0xD2A9257, -0x20EB1947
            ),
            DEMO_TRANSFORM(
                -764, 180, 656,  -79, 955, -356,  -675, -317, -698,
                -0x151B8B0D, -0xD2BC883, -0x20E62EB7
            ),
            DEMO_TRANSFORM(
                -736, 645, 296,  478, 767, -480,  -524, -207, -853,
                -0x151CBE49, -0xD2EEE2B, -0x20E39823
            ),
            DEMO_TRANSFORM(
                -931, -402, 139,  -395, 941, 71,  -156, 11, -1011,
                -0x1512C0CA, -0xD318033, -0x20E91802
            ),
            DEMO_TRANSFORM(
                -931, -402, 139,  -395, 941, 71,  -156, 11, -1011,
                -0x15191E0A, -0xD343FB3, -0x20E82372
            ),
        },
        .localVelocityZ = -0xD0810,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3833,
        .modelBaseYaw = -0x5B9F,
        .modelBaseRoll = 8,
        .projectedPos = DEMO_VEC3(-0x1519A022, -0xD273352, -0x20E81812),
        .movementSpeed = 0xBD3BB,
        .grabInputFrames = 44,
        .collisionVelocityX = -0xBB22E,
        .collisionVelocityZ = 0x1C17F,
        .pitchAngle = 3833,
        .facingAngle = -0x5B9F,
        .leanAngle = 8,
        .surfacePitchAngle = -263,
        .surfaceRollAngle = -39,
        .leanInput = 96,
        .leanScale = 32,
        .modeAngle = 27,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xD0C20,
        .speedLimit = 0xD0C20,
        .rankSpeedBonus = 0x7820,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x7FB00, -0x18B00),
            DEMO_MODEL_PART(1, 4, 232, 3397, 3587, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 665, 3397, 3587, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3073, 3372, 463, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 176, 3374, 69, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 219, 3845, 190, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 615, 3534, 3379, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 470, 3728, 3239, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3771, 3593, 498, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3401, 3794, 900, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2126,
        .animationFrameDataOffset = 2126,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x151DBBFE, -0xD3A1734, -0x20E30DC4),
            DEMO_VEC3(-0x150ADE6E, -0xD31D756, -0x20E5E2B8),
            DEMO_VEC3(-0x151F1256, -0xD3A9404, -0x20EBB26C),
            DEMO_VEC3(-0x150C34C6, -0xD3277E8, -0x20EE8760),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140688),
        .shadowMtx = DEMO_HEAP_MATRIX(140688 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x151DBBFE, -0xD394B61, -0x20E30DC4),
            DEMO_VEC3(-0x150ADE6E, -0xD312601, -0x20E5E2B8),
            DEMO_VEC3(-0x151F1256, -0xD3932C5, -0x20EBB26C),
            DEMO_VEC3(-0x150C34C6, -0xD310D65, -0x20EE8760),
            DEMO_VEC3(-0x1511F35E, -0xD347E2A, -0x20E4DA7D),
            DEMO_VEC3(-0x14FF5176, -0xD2BCB6E, -0x20E796CD),
            DEMO_VEC3(-0x15133AF2, -0xD3478B2, -0x20ED8155),
            DEMO_VEC3(-0x1500990A, -0xD2BC5F6, -0x20F03DA5),
        },
        .groundContactMask = 15,
        .coursePathIndex = 40,
        .coursePathOffset = 0xD34634,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133584),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = 0.0f,
        .replayFrame = 962,
    } },
    /* Segment 13, frame 928. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x10310CA8, -0xB6A1CFE, -0x1F740FD5),
        .renderPosition = DEMO_VEC3(-0x10310CA8, -0xB6CDAFD, -0x1F740FD5),
        .previousPosition = DEMO_VEC3(-0x1026FE55, -0xB6634AB, -0x1F6CDCD5),
        .velocity = DEMO_VEC3(-0xA0E53, -0x493EB, -0x73300),
        .groundOffset = 0x40000,
        .collisionBottomY = -0xB70C896,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0xA201,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3DB51,
        .modeValue = 0x46000,
        .stickX = -23,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -793, -309, -568,  -330, 966, -63,  555, 134, -849,
                -0x10310CA8, -0xB6CDAFD, -0x1F740FD5
            ),
            DEMO_TRANSFORM(
                -934, -336, -248,  -330, 966, -63,  256, 22, -990,
                -0x1034808E, -0xB6556EA, -0x1F7353CC
            ),
            DEMO_TRANSFORM(
                -784, -628, -193,  -656, 764, 179,  33, 260, -988,
                -0x1032F8F6, -0xB6551A2, -0x1F72EE8C
            ),
            DEMO_TRANSFORM(
                -784, -628, -193,  -531, 782, -388,  386, -198, -926,
                -0x10310CF6, -0xB678EA2, -0x1F73755C
            ),
            DEMO_TRANSFORM(
                -992, -52, -243,  -218, 666, 745,  120, 774, -657,
                -0x1035B576, -0xB664DD2, -0x1F73A91C
            ),
            DEMO_TRANSFORM(
                -1010, -150, -52,  -159, 946, 354,  -3, 358, -958,
                -0x103511C6, -0xB684152, -0x1F75D80C
            ),
            DEMO_TRANSFORM(
                -906, -262, -395,  -52, 902, -478,  471, -403, -812,
                -0x1034808E, -0xB6556EA, -0x1F7353CC
            ),
            DEMO_TRANSFORM(
                -945, 93, 380,  -186, 765, -652,  -344, -672, -689,
                -0x1034B48E, -0xB61D06A, -0x1F75324C
            ),
            DEMO_TRANSFORM(
                -394, -916, -225,  -720, 450, -570,  610, -61, -819,
                -0x1032777E, -0xB619D02, -0x1F73FF24
            ),
            DEMO_TRANSFORM(
                -154, -1006, -104,  -991, 171, -183,  198, 72, -1001,
                -0x102F0F1E, -0xB6178BE, -0x1F7177A8
            ),
            DEMO_TRANSFORM(
                -759, 570, -379,  589, 833, 75,  351, -162, -947,
                -0x1036E49E, -0xB62E572, -0x1F75EDD4
            ),
            DEMO_TRANSFORM(
                -570, 740, -415,  634, 704, 385,  565, -42, -852,
                -0x103A886E, -0xB64D55E, -0x1F76F204
            ),
            DEMO_TRANSFORM(
                -793, -309, -568,  -330, 966, -63,  555, 134, -849,
                -0x102F2548, -0xB697055, -0x1F7246AD
            ),
            DEMO_TRANSFORM(
                -793, -309, -568,  -330, 966, -63,  555, 134, -849,
                -0x10349178, -0xB6B8D85, -0x1F7628AD
            ),
        },
        .localVelocityZ = -0xCFA3B,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3896,
        .modelBaseYaw = -0x5D95,
        .modelBaseRoll = 90,
        .projectedPos = DEMO_VEC3(-0x1033FE68, -0xB5EACFE, -0x1F762B15),
        .movementSpeed = 0xC5E09,
        .grabInputFrames = 44,
        .collisionVelocityX = -0xA0E53,
        .collisionVelocityZ = -0x73300,
        .pitchAngle = 3896,
        .facingAngle = -0x5D95,
        .leanAngle = 90,
        .surfacePitchAngle = -200,
        .surfaceRollAngle = -19,
        .leanInput = 368,
        .leanScale = 32,
        .modeAngle = 104,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCFE80,
        .speedLimit = 0xCFE80,
        .rankSpeedBonus = 0x6A80,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 105, 3207, 221, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 204, 3768, 278, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 1024, 3598, 3203, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 741, 3777, 3166, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3322, 3666, 1006, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3106, 3800, 1118, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2126,
        .animationFrameDataOffset = 2126,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x103B7600, -0xB71318F, -0x1F763037),
            DEMO_VEC3(-0x102B6304, -0xB6AEB9A, -0x1F6AADB7),
            DEMO_VEC3(-0x1036B64C, -0xB7192D2, -0x1F7D71F3),
            DEMO_VEC3(-0x1026A350, -0xB6B4CDC, -0x1F71EF73),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107408),
        .shadowMtx = DEMO_HEAP_MATRIX(107408 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x103B7600, -0xB708FAB, -0x1F763037),
            DEMO_VEC3(-0x102B6304, -0xB6A4B6F, -0x1F6AADB7),
            DEMO_VEC3(-0x1036B64C, -0xB6F6A8B, -0x1F7D71F3),
            DEMO_VEC3(-0x1026A350, -0xB69264F, -0x1F71EF73),
            DEMO_VEC3(-0x10316391, -0xB6CA6D9, -0x1F6F1E0F),
            DEMO_VEC3(-0x10217399, -0xB66629D, -0x1F636A47),
            DEMO_VEC3(-0x102C8911, -0xB6B8ED9, -0x1F764F63),
            DEMO_VEC3(-0x101C9919, -0xB654A9D, -0x1F6A9B9B),
        },
        .groundContactMask = 15,
        .coursePathIndex = 34,
        .coursePathOffset = 0x1006142,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(100816),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 853,
    } },
    /* Segment 14, frame 980. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x115BB14B, -0xBD15280, -0x200978C7),
        .renderPosition = DEMO_VEC3(-0x115BB14B, -0xBD4AB97, -0x200978C7),
        .previousPosition = DEMO_VEC3(-0x115044E0, -0xBCD99B0, -0x20048891),
        .velocity = DEMO_VEC3(-0xB6C6B, -0x46E06, -0x4F036),
        .groundOffset = 0x40000,
        .collisionBottomY = -0xBD807B6,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1769,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3A6B3,
        .modeValue = 0x46000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -900, -292, -389,  -270, 981, -111,  405, 4, -940,
                -0x115BB14B, -0xBD4AB97, -0x200978C7
            ),
            DEMO_TRANSFORM(
                -984, -277, -50,  -270, 981, -111,  78, -94, -1016,
                -0x115E709A, -0xBCCD946, -0x2008FCC9
            ),
            DEMO_TRANSFORM(
                -847, -574, 11,  -560, 831, 203,  -123, 161, -1002,
                -0x115CDDC2, -0xBCCEC06, -0x2008DC11
            ),
            DEMO_TRANSFORM(
                -847, -574, 11,  -536, 785, -376,  202, -318, -951,
                -0x115B3992, -0xBCF5B76, -0x200974B1
            ),
            DEMO_TRANSFORM(
                -1021, 9, -66,  -37, 750, 694,  55, 695, -748,
                -0x115FBFF2, -0xBCDBBC6, -0x20090191
            ),
            DEMO_TRANSFORM(
                -1012, -67, 135,  -28, 986, 271,  -148, 264, -977,
                -0x115FA432, -0xBCFEEA6, -0x200B0A71
            ),
            DEMO_TRANSFORM(
                -982, -239, 159,  -280, 919, -350,  -61, -380, -947,
                -0x115E709A, -0xBCCD946, -0x2008FCC9
            ),
            DEMO_TRANSFORM(
                -716, 70, 727,  -439, 772, -506,  -584, -667, -509,
                -0x115F88DA, -0xBC941C6, -0x200A5B09
            ),
            DEMO_TRANSFORM(
                -624, -767, 262,  -685, 676, 345,  -432, 34, -926,
                -0x115CFFEA, -0xBC91F56, -0x200A92F1
            ),
            DEMO_TRANSFORM(
                -315, -957, 179,  -663, 349, 696,  -712, 98, -728,
                -0x11594502, -0xBCA1976, -0x200C59F9
            ),
            DEMO_TRANSFORM(
                -809, 403, 478,  199, 907, -428,  -593, -246, -796,
                -0x1161CBBA, -0xBCA4A16, -0x2009CB91
            ),
            DEMO_TRANSFORM(
                -485, 895, 88,  769, 465, -488,  -468, -165, -894,
                -0x11641B26, -0xBCCD322, -0x20074AF5
            ),
            DEMO_TRANSFORM(
                -900, -292, -389,  -270, 981, -111,  405, 4, -940,
                -0x1159461B, -0xBD1464F, -0x20086A07
            ),
            DEMO_TRANSFORM(
                -900, -292, -389,  -270, 981, -111,  405, 4, -940,
                -0x115F6DFB, -0xBD3469F, -0x200B1337
            ),
        },
        .localVelocityZ = -0xCFD6E,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3907,
        .modelBaseYaw = -0x5D0A,
        .modelBaseRoll = 3,
        .projectedPos = DEMO_VEC3(-0x115ED6CB, -0xBC5D2C0, -0x200AD4C7),
        .movementSpeed = 0xC71FA,
        .grabInputFrames = 44,
        .collisionVelocityX = -0xB6C6B,
        .collisionVelocityZ = -0x4F036,
        .pitchAngle = 3907,
        .facingAngle = -0x5D0A,
        .leanAngle = 3,
        .surfacePitchAngle = -189,
        .surfaceRollAngle = -207,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xD01E0,
        .speedLimit = 0xD01E0,
        .rankSpeedBonus = 0x6DE0,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x82BC0, -0x17FC0),
            DEMO_MODEL_PART(1, 4, 249, 3389, 3604, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 625, 3389, 3604, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 3127, 3367, 453, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 144, 3432, 16, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 248, 3880, 120, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 240, 3632, 3520, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 160, 3776, 3288, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 3904, 3824, 408, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3664, 4008, 848, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 202,
        .animationFrameDataOffset = 802,
        .animationFrameCount = 30,
        .animationFramesRemaining = 21,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x11668BEF, -0xBD84B24, -0x20096645),
            DEMO_VEC3(-0x11544C97, -0xBD2593C, -0x20018309),
            DEMO_VEC3(-0x116315FF, -0xBDB3008, -0x20116E85),
            DEMO_VEC3(-0x1150D6A7, -0xBD53E1F, -0x20098B49),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(139664),
        .shadowMtx = DEMO_HEAP_MATRIX(139664 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x11668BEF, -0xBD7A78F, -0x20096645),
            DEMO_VEC3(-0x11544C97, -0xBD1B8EB, -0x20018309),
            DEMO_VEC3(-0x116315FF, -0xBD79E43, -0x20116E85),
            DEMO_VEC3(-0x1150D6A7, -0xBD1AF9F, -0x20098B49),
            DEMO_VEC3(-0x115B1EF8, -0xBD3EEAC, -0x2004760F),
            DEMO_VEC3(-0x1148DFA0, -0xBCE0008, -0x1FFC92D3),
            DEMO_VEC3(-0x1157AA20, -0xBD3E218, -0x200C7E4F),
            DEMO_VEC3(-0x11456AC8, -0xBCDF374, -0x20049B13),
        },
        .groundContactMask = 3,
        .coursePathIndex = 35,
        .coursePathOffset = 0x119E7C6,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133584),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 880,
    } },
    /* Segment 15, frame 1043. */
    { .player = {
        .playerIndex = 2,
        .characterId = RACE_CHARACTER_JAM,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 3,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1C0A3D40, -0x123ABF48, -0x1D551336),
        .renderPosition = DEMO_VEC3(-0x1C0A3D40, -0x123A15D9, -0x1D551336),
        .previousPosition = DEMO_VEC3(-0x1BFF6297, -0x12364536, -0x1D5AD0BB),
        .velocity = DEMO_VEC3(-0xADAA9, -0x52F52, 0x5BD85),
        .collisionBottomY = -0x12417488,
        .groundCorrectionY = 2415,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x46556,
        .modeValue = 0x44000,
        .heldInput = A_BUTTON,
        .previousInput = A_BUTTON,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -850, -349, 449,  -310, 962, 159,  -477, -4, -905,
                -0x1C0A3D40, -0x123A15D9, -0x1D551336
            ),
            DEMO_TRANSFORM(
                -640, -327, 728,  -310, 962, 159,  -735, -121, -701,
                -0x1C0B20C0, -0x12347059, -0x1D525FF6
            ),
            DEMO_TRANSFORM(
                -391, -768, 550,  177, 525, 859,  -928, 423, -68,
                -0x1C0A09D0, -0x12346DD1, -0x1D5384EE
            ),
            DEMO_TRANSFORM(
                -391, -768, 550,  -928, 423, -68,  -176, -526, -859,
                -0x1C0A8EC0, -0x1235F821, -0x1D56098E
            ),
            DEMO_TRANSFORM(
                -703, 73, 739,  743, 76, 699,  -4, 1018, -106,
                -0x1C0BEA00, -0x12356371, -0x1D5162BE
            ),
            DEMO_TRANSFORM(
                -525, -114, 870,  137, 991, 212,  -867, 226, -493,
                -0x1C0E17A0, -0x12359CD1, -0x1D536EFE
            ),
            DEMO_TRANSFORM(
                -217, -511, 859,  -960, 351, -33,  -278, -813, -554,
                -0x1C0B20C0, -0x12347059, -0x1D525FF6
            ),
            DEMO_TRANSFORM(
                334, 94, 962,  -725, 697, 183,  -639, -742, 294,
                -0x1C0EE100, -0x12331119, -0x1D528136
            ),
            DEMO_TRANSFORM(
                636, -5, 800,  -373, 903, 302,  -708, -481, 559,
                -0x1C0DE130, -0x1231FD29, -0x1D5495EE
            ),
            DEMO_TRANSFORM(
                741, -451, 541,  117, 854, 550,  -695, -337, 670,
                -0x1C0DC184, -0x12354915, -0x1D574222
            ),
            DEMO_TRANSFORM(
                -386, -211, 923,  -567, 850, -43,  -758, -528, -437,
                -0x1C0EF0C0, -0x12347CD9, -0x1D50642E
            ),
            DEMO_TRANSFORM(
                -665, 375, 679,  -65, 865, -542,  -774, -395, -538,
                -0x1C0D9DF8, -0x123803B5, -0x1D4E6DE2
            ),
            DEMO_TRANSFORM(
                -850, -349, 449,  -310, 962, 159,  -477, -4, -905,
                -0x1C081710, -0x12368AA1, -0x1D56395E
            ),
            DEMO_TRANSFORM(
                -850, -349, 449,  -310, 962, 159,  -477, -4, -905,
                -0x1C0DE7E0, -0x1238EE41, -0x1D53264E
            ),
        },
        .localVelocityZ = -0xD1000,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = -0x2AC3,
        .modelBaseRoll = -3,
        .projectedPos = DEMO_VEC3(-0x1C0DDB80, -0x122F77C8, -0x1D532976),
        .movementSpeed = 0xC4751,
        .grabInputFrames = 44,
        .collisionVelocityX = -0xADAA9,
        .collisionVelocityZ = 0x5BD85,
        .pitchAngle = 3869,
        .facingAngle = -0x2AC3,
        .leanAngle = -3,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -4087,
        .leanScale = 32,
        .stateFlags = 0x190,
        .mode = 2,
        .updateState = 1,
        .updateTimer = 1,
        .targetSpeedLimit = 0xD1400,
        .speedLimit = 0xD1400,
        .rankSpeedBonus = 0x8000,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 0, 3072, 0, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 0, 3296, 0, 0, 0x60000, -0x20000),
            DEMO_MODEL_PART(1, 4, 56, 3480, 3416, -0x18000, -0x8000, 0),
            DEMO_MODEL_PART(2, 5, 1080, 3480, 3416, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 2, 2512, 3416, 576, 0x18000, -0x8000, 0),
            DEMO_MODEL_PART(4, 3, 3640, 3448, 224, 0, -0x30000, 0),
            DEMO_MODEL_PART(1, 6, 776, 3576, 3832, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 296, 4024, 88, 0, 0x40000, 0),
            DEMO_MODEL_PART(6, 10, 88, 112, 4048, -0x28000, 0x38000, 0),
            DEMO_MODEL_PART(8, 11, 4000, 144, 3704, -0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(6, 8, 168, 3512, 64, 0x28000, 0x38000, 0),
            DEMO_MODEL_PART(10, 9, 3624, 3584, 752, 0x20000, -0x3C000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, -0x38000, 0x28000, 0),
            DEMO_MODEL_PART(0, 0, 0, 3072, 0, 0x38000, 0x28000, 0),
        },
        .modelPartCount = 14,
        .animationId = 5,
        .animationStartOffset = 3150,
        .animationFrameDataOffset = 3390,
        .animationFrameCount = 4,
        .animationFramesRemaining = 1,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x1C10D1E4, -0x123E6FE7, -0x1D4CA648),
            DEMO_VEC3(-0x1BFF94A0, -0x123753CD, -0x1D55C35C),
            DEMO_VEC3(-0x1C14E5E0, -0x123E4CC3, -0x1D546310),
            DEMO_VEC3(-0x1C03A89C, -0x123730AA, -0x1D5D8024),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140688),
        .shadowMtx = DEMO_HEAP_MATRIX(140688 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x1C10D1E4, -0x123D9BB7, -0x1D4CA648),
            DEMO_VEC3(-0x1BFF94A0, -0x123686AF, -0x1D55C35C),
            DEMO_VEC3(-0x1C14E5E0, -0x123DA503, -0x1D546310),
            DEMO_VEC3(-0x1C03A89C, -0x12368FFB, -0x1D5D8024),
            DEMO_VEC3(-0x1C05F73B, -0x123921A6, -0x1D5263CD),
            DEMO_VEC3(-0x1BF4B9F7, -0x12320C9E, -0x1D5B80E1),
            DEMO_VEC3(-0x1C0A0B37, -0x12392AF2, -0x1D5A2095),
            DEMO_VEC3(-0x1BF8CDF3, -0x123215EA, -0x1D633DA9),
        },
        .groundContactMask = 15,
        .coursePathIndex = 47,
        .coursePathOffset = 0x523BAC,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 147,
        .modelPartMatrices = DEMO_MODEL_MATRICES(133584),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 1146,
    } },
},
{
    /* Player 3. */
    /* Segment 0, frame 0. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(0xE84FA9, -0x104D37A, -0x63D04E0),
        .renderPosition = DEMO_VEC3(0xE84FA9, -0x107DBC7, -0x63D04E0),
        .previousPosition = DEMO_VEC3(0xE560DB, -0x103D457, -0x630C2EF),
        .velocity = DEMO_VEC3(0x2EECE, -0x1C063, -0xC41F1),
        .groundOffset = 0x3ABC9,
        .collisionBottomY = -0x10B94BA,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 892,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x10B26,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                237, -89, -991,  13, 1019, -88,  995, 7, 237,
                0xE84FA9, -0x107DBC7, -0x63D04E0
            ),
            DEMO_TRANSFORM(
                -111, -82, -1014,  224, 993, -105,  992, -234, -90,
                0xE7AE42, -0xFE5B45, -0x63E0564
            ),
            DEMO_TRANSFORM(
                -132, -414, -926,  -294, 909, -364,  970, 219, -237,
                0xE791EA, -0xFF728D, -0x63C6840
            ),
            DEMO_TRANSFORM(
                -132, -414, -926,  422, 826, -430,  922, -438, 63,
                0xE8B86A, -0x102FFCD, -0x63AFC40
            ),
            DEMO_TRANSFORM(
                -24, 415, -935,  -576, 766, 356,  845, 535, 215,
                0xE73E4A, -0xFFB09D, -0x63F60C0
            ),
            DEMO_TRANSFORM(
                -995, 4, -238,  25, 1019, -85,  237, -89, -991,
                0xE97E8A, -0x102AF5D, -0x640C500
            ),
            DEMO_TRANSFORM(
                -97, -226, -993,  269, 956, -244,  982, -284, -32,
                0xE7AE42, -0xFE5B45, -0x63E0564
            ),
            DEMO_TRANSFORM(
                -795, 10, -643,  346, 869, -413,  541, -539, -679,
                0xE845FE, -0xFC4185, -0x63E8EA4
            ),
            DEMO_TRANSFORM(
                -397, -303, -892,  38, 962, -344,  941, -168, -363,
                0xE8723A, -0xFBEFDD, -0x63C1274
            ),
            DEMO_TRANSFORM(
                -114, -983, -258,  -499, 279, -847,  885, 31, -511,
                0xE9442A, -0xFEF445, -0x6388C64
            ),
            DEMO_TRANSFORM(
                33, 209, -1001,  85, 997, 212,  1019, -90, 14,
                0xE7F80A, -0xFD0AAD, -0x640EC54
            ),
            DEMO_TRANSFORM(
                -58, 835, -587,  -288, 550, 813,  980, 212, 203,
                0xE7B7BA, -0x1006D15, -0x644325C
            ),
            DEMO_TRANSFORM(
                237, -89, -991,  13, 1019, -88,  995, 7, 237,
                0xE6ECDD, -0x1049D7B, -0x63A8BB0
            ),
            DEMO_TRANSFORM(
                237, -89, -991,  13, 1019, -88,  995, 7, 237,
                0xE850BD, -0x10523BB, -0x6405B50
            ),
        },
        .localVelocityZ = -0xCA7A0,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 4039,
        .modelBaseYaw = -0x1099,
        .modelBaseRoll = -3,
        .projectedPos = DEMO_VEC3(0xE88DE9, -0xF8DF7A, -0x63E09E0),
        .movementSpeed = 0xC9A83,
        .collisionVelocityX = 0x2EECE,
        .collisionVelocityZ = -0xC41F1,
        .pitchAngle = 4039,
        .facingAngle = -0x1099,
        .leanAngle = -3,
        .surfacePitchAngle = -57,
        .surfaceRollAngle = -6,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCABA0,
        .speedLimit = 0xCABA0,
        .rankSpeedBonus = 0x17A0,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 129, 3296, 0, 0, 0x98CC0, -0xC600),
            DEMO_MODEL_PART(1, 4, 287, 3392, 3589, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 759, 3392, 3589, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3001, 3441, 724, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 424, 3304, 3824, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 296, 3808, 32, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 272, 3512, 3848, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 504, 3896, 3184, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3528, 3304, 632, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3080, 3776, 880, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 2304,
        .animationFrameCount = 36,
        .animationFramesRemaining = 2,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(0xE67679, -0x1096E5D, -0x6481472),
            DEMO_VEC3(0xE1A7BD, -0x1078FF6, -0x633FBBE),
            DEMO_VEC3(0xEEF795, -0x1098623, -0x6460E02),
            DEMO_VEC3(0xEA28D9, -0x107A7BC, -0x631F54E),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140432),
        .shadowMtx = DEMO_HEAP_MATRIX(140432 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(0xE67679, -0x108CCAB, -0x6481472),
            DEMO_VEC3(0xE1A7BD, -0x106FC63, -0x633FBBE),
            DEMO_VEC3(0xEEF795, -0x108BB2B, -0x6460E02),
            DEMO_VEC3(0xEA28D9, -0x106EAE3, -0x631F54E),
            DEMO_VEC3(0xE387AB, -0x107A8DF, -0x63BD281),
            DEMO_VEC3(0xDEB8EF, -0x105D897, -0x627B9CD),
            DEMO_VEC3(0xEC08C7, -0x107975F, -0x639CC11),
            DEMO_VEC3(0xE73A0B, -0x105C717, -0x625B35D),
        },
        .groundContactMask = 15,
        .coursePathIndex = 5,
        .coursePathOffset = 0x5125A0,
        .rankIndex = 1,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132688),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 190,
    } },
    /* Segment 1, frame 57. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x67C2B1, -0x2056883, -0xAD8B7A3),
        .renderPosition = DEMO_VEC3(-0x67C2B1, -0x207DED8, -0xAD8B7A3),
        .previousPosition = DEMO_VEC3(-0x5F15FE, -0x20277D8, -0xACFF622),
        .velocity = DEMO_VEC3(-0x8ACB3, -0x3B1EB, -0x8C181),
        .groundOffset = 0x33241,
        .collisionBottomY = -0x20C29C3,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x1BEC,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x30241,
        .heldInput = STICK_RIGHT,
        .previousInput = STICK_RIGHT,
        .stickX = 31,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -698, -249, -705,  -41, 976, -303,  746, -178, -676,
                -0x67C2B1, -0x207DED8, -0xAD8B7A3
            ),
            DEMO_TRANSFORM(
                -909, -170, -437,  79, 884, -508,  462, -486, -772,
                -0x68CF04, -0x1FEE17A, -0xADAD82F
            ),
            DEMO_TRANSFORM(
                -844, -506, -279,  -498, 887, -101,  292, 52, -978,
                -0x6792F8, -0x1FFB5F6, -0xAD99523
            ),
            DEMO_TRANSFORM(
                -844, -506, -279,  -96, 611, -814,  569, -646, -552,
                -0x65A0B8, -0x2032D76, -0xAD92FA3
            ),
            DEMO_TRANSFORM(
                -747, 339, -610,  -55, 862, 548,  696, 433, -610,
                -0x6A3CE8, -0x20035A6, -0xADADD43
            ),
            DEMO_TRANSFORM(
                -747, 190, 673,  -32, 974, -311,  -698, -249, -705,
                -0x6A0568, -0x20393A6, -0xADD01C3
            ),
            DEMO_TRANSFORM(
                -673, -251, -728,  -204, 990, -153,  742, 44, -702,
                -0x68CF04, -0x1FEE17A, -0xADAD82F
            ),
            DEMO_TRANSFORM(
                -939, -256, 313,  -396, 738, -585,  -79, -659, -777,
                -0x6941C4, -0x1FCB42E, -0xADB2E3F
            ),
            DEMO_TRANSFORM(
                -562, -852, -63,  -462, 366, -835,  718, -431, -586,
                -0x67903C, -0x1FC54C2, -0xAD95D5F
            ),
            DEMO_TRANSFORM(
                39, -990, 257,  -704, -213, -710,  740, -150, -689,
                -0x6462AC, -0x1FBAE32, -0xAD5F27F
            ),
            DEMO_TRANSFORM(
                -424, 513, -776,  313, 882, 413,  876, -66, -523,
                -0x6AD9CC, -0x1FD8F72, -0xADCEBFF
            ),
            DEMO_TRANSFORM(
                -211, 866, -500,  188, 537, 850,  983, 83, -270,
                -0x6D1C5C, -0x1FFC11A, -0xAE06E4F
            ),
            DEMO_TRANSFORM(
                -698, -249, -705,  -41, 976, -303,  746, -178, -676,
                -0x66614D, -0x2042634, -0xAD70B4B
            ),
            DEMO_TRANSFORM(
                -698, -249, -705,  -41, 976, -303,  746, -178, -676,
                -0x6A796D, -0x2059BB4, -0xADB2D2B
            ),
        },
        .localVelocityZ = -0xCB508,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3936,
        .modelBaseYaw = -0x7E03,
        .modelBaseRoll = -126,
        .projectedPos = DEMO_VEC3(-0x69CFB1, -0x1F9C4C3, -0xADAC9E3),
        .movementSpeed = 0xC5349,
        .collisionVelocityX = -0x8ACB3,
        .collisionVelocityZ = -0x8C181,
        .pitchAngle = 3936,
        .facingAngle = -0x7E03,
        .leanAngle = -126,
        .surfacePitchAngle = -160,
        .surfaceRollAngle = -58,
        .leanInput = -496,
        .leanScale = 32,
        .modeAngle = -141,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCB940,
        .speedLimit = 0xCB940,
        .rankSpeedBonus = 0x2540,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 156, 3296, 0, 0, 0x941C0, -0xEE00),
            DEMO_MODEL_PART(1, 4, 260, 3406, 3555, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 828, 3406, 3555, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2887, 3459, 782, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, -83, 3050, 4029, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 380, 3772, -229, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 1009, 3660, 3245, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 1020, 4073, 3044, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 2875, 3605, 1116, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 2721, 3916, 1033, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 2,
        .animationStartOffset = 2366,
        .animationFrameDataOffset = 2366,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x72078F, -0x20B33ED, -0xADCF8F5),
            DEMO_VEC3(-0x63DECB, -0x2061DCD, -0xACEAE79),
            DEMO_VEC3(-0x6BA697, -0x20BE1C1, -0xAE2C0CD),
            DEMO_VEC3(-0x5D7DD3, -0x206CBA2, -0xAD47651),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107664),
        .shadowMtx = DEMO_HEAP_MATRIX(107664 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x72078F, -0x209A12E, -0xADCF8F5),
            DEMO_VEC3(-0x63DECB, -0x204957E, -0xACEAE79),
            DEMO_VEC3(-0x6BA697, -0x20B2832, -0xAE2C0CD),
            DEMO_VEC3(-0x5D7DD3, -0x2061C82, -0xAD47651),
            DEMO_VEC3(-0x696C96, -0x20698C9, -0xAD4084C),
            DEMO_VEC3(-0x5B0592, -0x20194E1, -0xAC5FAC4),
            DEMO_VEC3(-0x63266A, -0x2080C1D, -0xAD9F180),
            DEMO_VEC3(-0x54BF66, -0x2030835, -0xACBE3F8),
        },
        .groundContactMask = 15,
        .coursePathIndex = 9,
        .coursePathOffset = 0x1B15086,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(99920),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 299,
    } },
    /* Segment 2, frame 108. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x97C7696, -0x731651B, -0x15972C6A),
        .renderPosition = DEMO_VEC3(-0x97C7696, -0x734BFCC, -0x15972C6A),
        .previousPosition = DEMO_VEC3(-0x9767BF8, -0x729237B, -0x158F5103),
        .velocity = DEMO_VEC3(-0x5FA9E, -0x8F6CB, -0x7DB67),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x7381A46,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1359,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x82BBB,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -476, -655, -625,  -406, 786, -514,  809, 9, -626,
                -0x97C7696, -0x734BFCC, -0x15972C6A
            ),
            DEMO_TRANSFORM(
                -722, -617, -380,  -288, 737, -648,  665, -350, -694,
                -0x980DCEC, -0x72D43EF, -0x159BA9FD
            ),
            DEMO_TRANSFORM(
                -575, -826, -184,  -718, 594, -422,  448, -108, -913,
                -0x97F73E8, -0x72D42EF, -0x159A50A5
            ),
            DEMO_TRANSFORM(
                -575, -826, -184,  -322, 420, -875,  782, -433, -496,
                -0x97CA5E8, -0x72F952F, -0x1598AA25
            ),
            DEMO_TRANSFORM(
                -796, -205, -609,  -386, 927, 193,  513, 380, -799,
                -0x98191C8, -0x72F11DF, -0x159B6E05
            ),
            DEMO_TRANSFORM(
                -814, 0, 620,  -397, 786, -521,  -476, -655, -625,
                -0x9800FC8, -0x732B19F, -0x159C2F85
            ),
            DEMO_TRANSFORM(
                -595, -728, -403,  -289, 646, -738,  780, -315, -582,
                -0x980DCEC, -0x72D43EF, -0x159BA9FD
            ),
            DEMO_TRANSFORM(
                -944, -240, 311,  -387, 428, -844,  68, -898, -485,
                -0x9817FC4, -0x72BD86B, -0x159D4965
            ),
            DEMO_TRANSFORM(
                -766, -628, -253,  -337, 686, -680,  587, -426, -720,
                -0x97FF97C, -0x72A39A7, -0x159C1F35
            ),
            DEMO_TRANSFORM(
                51, -922, 441,  -900, -249, -417,  483, -367, -823,
                -0x97CC8C4, -0x72B5EAF, -0x1598D85D
            ),
            DEMO_TRANSFORM(
                -502, -484, -749,  -203, 899, -444,  868, -69, -537,
                -0x982E1DC, -0x72DC7F7, -0x159E1745
            ),
            DEMO_TRANSFORM(
                -622, 359, -728,  -13, 913, 461,  812, 290, -550,
                -0x983501C, -0x73279DF, -0x159E2F0D
            ),
            DEMO_TRANSFORM(
                -476, -655, -625,  -406, 786, -514,  809, 9, -626,
                -0x97CDA6E, -0x7308814, -0x1596633A
            ),
            DEMO_TRANSFORM(
                -476, -655, -625,  -406, 786, -514,  809, 9, -626,
                -0x97FA46E, -0x7345FB4, -0x159A0DDA
            ),
        },
        .localVelocityZ = -0xCDB5F,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3643,
        .modelBaseYaw = -0x2E58,
        .projectedPos = DEMO_VEC3(-0x9811D96, -0x7282D9B, -0x159D49AA),
        .movementSpeed = 0x9DF8D,
        .grabInputFrames = 24,
        .collisionVelocityX = -0x5FA9E,
        .collisionVelocityZ = -0x7DB67,
        .pitchAngle = 3643,
        .facingAngle = -0x2E58,
        .surfacePitchAngle = -453,
        .surfaceRollAngle = -23,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCDF60,
        .speedLimit = 0xCDF60,
        .rankSpeedBonus = 0x4B60,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 472, 3240, 3776, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 304, 3832, 16, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 192, 3376, 4016, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 664, 3904, 3224, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 2976, 3208, 1192, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 2904, 3760, 984, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 1584,
        .animationFrameCount = 36,
        .animationFramesRemaining = 14,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x984BEE0, -0x73C074A, -0x159AD6C4),
            DEMO_VEC3(-0x97B19A0, -0x72EB4AE, -0x158E28B0),
            DEMO_VEC3(-0x97DD38C, -0x73C6631, -0x15A03024),
            DEMO_VEC3(-0x9742E4C, -0x72F1395, -0x15938210),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140368),
        .shadowMtx = DEMO_HEAP_MATRIX(140368 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x984BEE0, -0x73B6F0A, -0x159AD6C4),
            DEMO_VEC3(-0x97B19A0, -0x72E2556, -0x158E28B0),
            DEMO_VEC3(-0x97DD38C, -0x73B5A42, -0x15A03024),
            DEMO_VEC3(-0x9742E4C, -0x72E108E, -0x15938210),
            DEMO_VEC3(-0x97EC442, -0x7332D6A, -0x1592FB5D),
            DEMO_VEC3(-0x9751F02, -0x725E3B6, -0x15864D49),
            DEMO_VEC3(-0x977D8EE, -0x73318A2, -0x159854BD),
            DEMO_VEC3(-0x96E33AE, -0x725CEEE, -0x158BA6A9),
        },
        .groundContactMask = 15,
        .coursePathIndex = 25,
        .coursePathOffset = 0x38CAA58,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132688),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = 0.0f,
        .replayFrame = 604,
    } },
    /* Segment 3, frame 161. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xABC1439, -0x833AD1E, -0x173B2F4F),
        .renderPosition = DEMO_VEC3(-0xABC1439, -0x8367B06, -0x173B2F4F),
        .previousPosition = DEMO_VEC3(-0xAB4BED7, -0x82F68A7, -0x17318C37),
        .velocity = DEMO_VEC3(-0x75562, -0x505B7, -0x9A318),
        .groundOffset = 0x375C8,
        .collisionBottomY = -0x83A6E5E,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 2016,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x45081,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -583, -348, -766,  -220, 962, -269,  811, 11, -623,
                -0xABC1439, -0x8367B06, -0x173B2F4F
            ),
            DEMO_TRANSFORM(
                -823, -327, -512,  -83, 915, -450,  602, -321, -762,
                -0xABEC607, -0x82DA2B7, -0x173D2540
            ),
            DEMO_TRANSFORM(
                -711, -636, -370,  -632, 791, -146,  377, 126, -942,
                -0xABD775F, -0x82E45EB, -0x173BD848
            ),
            DEMO_TRANSFORM(
                -711, -636, -370,  -176, 646, -772,  714, -472, -559,
                -0xABAFF1F, -0x8315CEB, -0x173B4608
            ),
            DEMO_TRANSFORM(
                -774, 188, -641,  -240, 837, 537,  624, 558, -589,
                -0xABFE0CF, -0x82F3B8B, -0x173D58A8
            ),
            DEMO_TRANSFORM(
                -814, 0, 620,  -210, 963, -276,  -583, -348, -766,
                -0xABEF04F, -0x832808B, -0x173F7228
            ),
            DEMO_TRANSFORM(
                -721, -451, -568,  -128, 867, -526,  714, -300, -668,
                -0xABEC607, -0x82DA2B7, -0x173D2540
            ),
            DEMO_TRANSFORM(
                -982, -115, 261,  -265, 711, -685,  -104, -726, -713,
                -0xABF0E2B, -0x82BBA9B, -0x173E4D68
            ),
            DEMO_TRANSFORM(
                -872, -363, -392,  -168, 899, -458,  507, -326, -826,
                -0xABD435F, -0x82AD67F, -0x173CC958
            ),
            DEMO_TRANSFORM(
                -153, -997, 172,  -920, 64, -442,  421, -221, -906,
                -0xABA798F, -0x82D76A7, -0x173A0A08
            ),
            DEMO_TRANSFORM(
                -569, -147, -837,  0, 1008, -177,  850, -98, -560,
                -0xAC0C8EF, -0x82D0B2F, -0x173F8FA8
            ),
            DEMO_TRANSFORM(
                -513, 664, -584,  162, 735, 692,  870, 254, -474,
                -0xAC22CDF, -0x8315787, -0x1740E9D8
            ),
            DEMO_TRANSFORM(
                -583, -348, -766,  -220, 962, -269,  811, 11, -623,
                -0xABB9C5D, -0x832A62A, -0x173945BF
            ),
            DEMO_TRANSFORM(
                -583, -348, -766,  -220, 962, -269,  811, 11, -623,
                -0xABF06DD, -0x834B02A, -0x173DC2BF
            ),
        },
        .localVelocityZ = -0xCE120,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3870,
        .modelBaseYaw = -0x2E58,
        .projectedPos = DEMO_VEC3(-0xABE8BB9, -0x828641E, -0x173E6D8F),
        .movementSpeed = 0xC1C2E,
        .grabInputFrames = 24,
        .collisionVelocityX = -0x75562,
        .collisionVelocityZ = -0x9A318,
        .pitchAngle = 3870,
        .facingAngle = -0x2E58,
        .surfacePitchAngle = -226,
        .surfaceRollAngle = -8,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCE520,
        .speedLimit = 0xCE520,
        .rankSpeedBonus = 0x5120,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 141, 3296, 0, 0, 0x96B00, -0xD800),
            DEMO_MODEL_PART(1, 4, 275, 3398, 3574, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 790, 3398, 3574, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2950, 3449, 750, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 472, 3240, 3776, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 304, 3832, 16, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 192, 3376, 4016, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 664, 3904, 3224, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 2976, 3208, 1192, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 2904, 3760, 984, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 2184,
        .animationFrameCount = 36,
        .animationFramesRemaining = 4,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xAC57481, -0x83AAE84, -0x174048B7),
            DEMO_VEC3(-0xAB9A431, -0x8339410, -0x1730C317),
            DEMO_VEC3(-0xABE8441, -0x83ACE2E, -0x17459B87),
            DEMO_VEC3(-0xAB2B3F1, -0x833B3BA, -0x173615E7),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140368),
        .shadowMtx = DEMO_HEAP_MATRIX(140368 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xAC57481, -0x83A0E80, -0x174048B7),
            DEMO_VEC3(-0xAB9A431, -0x8330140, -0x1730C317),
            DEMO_VEC3(-0xABE8441, -0x839F4CC, -0x17459B87),
            DEMO_VEC3(-0xAB2B3F1, -0x832E78C, -0x173615E7),
            DEMO_VEC3(-0xABE1F1F, -0x835A58D, -0x1736A59F),
            DEMO_VEC3(-0xAB24ECF, -0x82E984D, -0x17271FFF),
            DEMO_VEC3(-0xAB72EDF, -0x8358BD9, -0x173BF86F),
            DEMO_VEC3(-0xAAB5E8F, -0x82E7E99, -0x172C72CF),
        },
        .groundContactMask = 15,
        .coursePathIndex = 27,
        .coursePathOffset = 0x1044593,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132688),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = 0.0f,
        .replayFrame = 650,
    } },
    /* Segment 4, frame 210. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xB64D3AA, -0x8989C9B, -0x1818F043),
        .renderPosition = DEMO_VEC3(-0xB64D3AA, -0x89BF4BC, -0x1818F043),
        .previousPosition = DEMO_VEC3(-0xB5D7CA5, -0x89432AC, -0x180F4B04),
        .velocity = DEMO_VEC3(-0x75705, -0x51F25, -0x9A53F),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x89F51D1,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 2015,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x4517C,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -583, -348, -766,  -220, 962, -269,  811, 11, -623,
                -0xB64D3AA, -0x89BF4BC, -0x1818F043
            ),
            DEMO_TRANSFORM(
                -823, -327, -512,  -110, 929, -415,  598, -278, -782,
                -0xB67738E, -0x892CACF, -0x181B18D9
            ),
            DEMO_TRANSFORM(
                -728, -607, -385,  -567, 821, -224,  442, 53, -921,
                -0xB661C4E, -0x893724F, -0x1819D6F9
            ),
            DEMO_TRANSFORM(
                -728, -607, -385,  -203, 699, -718,  689, -434, -618,
                -0xB63E54E, -0x896A80F, -0x1818F6F9
            ),
            DEMO_TRANSFORM(
                -803, 137, -618,  -191, 899, 449,  603, 468, -680,
                -0xB6885BE, -0x89467EF, -0x181B5759
            ),
            DEMO_TRANSFORM(
                -814, 0, 620,  -210, 963, -276,  -583, -348, -766,
                -0xB67C67E, -0x897EB2F, -0x181D1899
            ),
            DEMO_TRANSFORM(
                -798, -469, -436,  -192, 840, -550,  610, -346, -743,
                -0xB67738E, -0x892CACF, -0x181B18D9
            ),
            DEMO_TRANSFORM(
                -999, -103, 192,  -208, 724, -691,  -66, -715, -728,
                -0xB67DFD6, -0x890F207, -0x181C4E39
            ),
            DEMO_TRANSFORM(
                -859, -551, -74,  -484, 809, -395,  272, -296, -940,
                -0xB65E10E, -0x890016F, -0x181B1AE1
            ),
            DEMO_TRANSFORM(
                -101, -1011, 122,  -1017, 95, -44,  33, -126, -1014,
                -0xB61E386, -0x891D1E7, -0x181960A9
            ),
            DEMO_TRANSFORM(
                -844, -1, -578,  88, 1011, -130,  571, -157, -833,
                -0xB69C68E, -0x8924BAF, -0x181D3CD1
            ),
            DEMO_TRANSFORM(
                -656, 671, -404,  420, 747, 557,  662, 192, -756,
                -0xB6C2E4E, -0x8963F4F, -0x181E2449
            ),
            DEMO_TRANSFORM(
                -583, -348, -766,  -220, 962, -269,  811, 11, -623,
                -0xB645BCE, -0x8981FE0, -0x181706B3
            ),
            DEMO_TRANSFORM(
                -583, -348, -766,  -220, 962, -269,  811, 11, -623,
                -0xB67C64E, -0x89A29E0, -0x181B83B3
            ),
        },
        .localVelocityZ = -0xCE400,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3870,
        .modelBaseYaw = -0x2E58,
        .projectedPos = DEMO_VEC3(-0xB674B2A, -0x88D539B, -0x181C2E83),
        .movementSpeed = 0xC1EE2,
        .grabInputFrames = 24,
        .collisionVelocityX = -0x75705,
        .collisionVelocityZ = -0x9A53F,
        .pitchAngle = 3870,
        .facingAngle = -0x2E58,
        .surfacePitchAngle = -226,
        .surfaceRollAngle = -8,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCE800,
        .speedLimit = 0xCE800,
        .rankSpeedBonus = 0x5400,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 408, 3336, 3832, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 288, 3792, 40, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 280, 3576, 3792, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 416, 3896, 3176, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3640, 3376, 520, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3160, 3800, 824, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 1404,
        .animationFrameCount = 36,
        .animationFramesRemaining = 17,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xB6E33F2, -0x8A0283A, -0x181E09AB),
            DEMO_VEC3(-0xB6263A2, -0x8990DC6, -0x180E840B),
            DEMO_VEC3(-0xB6743B2, -0x8A047E4, -0x18235C7B),
            DEMO_VEC3(-0xB5B7362, -0x8992D70, -0x1813D6DB),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107600),
        .shadowMtx = DEMO_HEAP_MATRIX(107600 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xB6E33F2, -0x89F8836, -0x181E09AB),
            DEMO_VEC3(-0xB6263A2, -0x8987AF6, -0x180E840B),
            DEMO_VEC3(-0xB6743B2, -0x89F6E82, -0x18235C7B),
            DEMO_VEC3(-0xB5B7362, -0x8986142, -0x1813D6DB),
            DEMO_VEC3(-0xB66DCED, -0x89B1E46, -0x1814646C),
            DEMO_VEC3(-0xB5B0C9D, -0x8941106, -0x1804DECC),
            DEMO_VEC3(-0xB5FECAD, -0x89B0492, -0x1819B73C),
            DEMO_VEC3(-0xB541C5D, -0x893F752, -0x180A319C),
        },
        .groundContactMask = 15,
        .coursePathIndex = 27,
        .coursePathOffset = 0x21ABA01,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(99920),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = 0.0f,
        .replayFrame = 673,
    } },
    /* Segment 5, frame 263. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x2F49A0E, -0x2E00B84, -0xE2A1A1D),
        .renderPosition = DEMO_VEC3(-0x2F49A0E, -0x2E35864, -0xE2A1A1D),
        .previousPosition = DEMO_VEC3(-0x2ECFD51, -0x2DE0CDA, -0xE201941),
        .velocity = DEMO_VEC3(-0x79CBD, -0x2BFEA, -0xA00DC),
        .groundOffset = 0x3F9DA,
        .collisionBottomY = -0x2E6CCC4,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 3322,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x20AAF,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -611, -169, -803,  -108, 1009, -129,  813, 7, -620,
                -0x2F49A0E, -0x2E35864, -0xE2A1A1D
            ),
            DEMO_TRANSFORM(
                -849, -157, -547,  0, 984, -280,  570, -232, -817,
                -0x2F629AF, -0x2D9C01D, -0xE2AEC93
            ),
            DEMO_TRANSFORM(
                -787, -463, -460,  -472, 901, -99,  451, 136, -908,
                -0x2F4EADF, -0x2DAB88D, -0xE29C783
            ),
            DEMO_TRANSFORM(
                -787, -463, -460,  -116, 809, -614,  642, -420, -675,
                -0x2F3129F, -0x2DE3E4D, -0xE296443
            ),
            DEMO_TRANSFORM(
                -775, 313, -588,  -96, 840, 576,  659, 492, -607,
                -0x2F7682F, -0x2DB2EAD, -0xE2B6253
            ),
            DEMO_TRANSFORM(
                -815, 4, 619,  -98, 1009, -137,  -611, -169, -803,
                -0x2F707AF, -0x2DE76AD, -0xE2DA253
            ),
            DEMO_TRANSFORM(
                -915, -326, -318,  -159, 898, -461,  427, -363, -855,
                -0x2F629AF, -0x2D9C01D, -0xE2AEC93
            ),
            DEMO_TRANSFORM(
                -1018, 12, 101,  -50, 826, -600,  -89, -602, -821,
                -0x2F6838B, -0x2D7C6B5, -0xE2BF007
            ),
            DEMO_TRANSFORM(
                -946, -322, 216,  -355, 948, -142,  -155, -207, -989,
                -0x2F43D37, -0x2D732A5, -0xE2B0C23
            ),
            DEMO_TRANSFORM(
                -232, -986, 144,  -892, 271, 419,  -442, -31, -921,
                -0x2F08A27, -0x2DA1CED, -0xE2B0523
            ),
            DEMO_TRANSFORM(
                -1016, 82, -83,  84, 1019, -24,  80, -31, -1019,
                -0x2F8B5E7, -0x2D8CB15, -0xE2C9A43
            ),
            DEMO_TRANSFORM(
                -596, 830, 25,  806, 571, 264,  200, 174, -987,
                -0x2FB85B7, -0x2DC9315, -0xE2CB60B
            ),
            DEMO_TRANSFORM(
                -611, -169, -803,  -108, 1009, -129,  813, 7, -620,
                -0x2F3B926, -0x2DFE628, -0xE27AC39
            ),
            DEMO_TRANSFORM(
                -611, -169, -803,  -108, 1009, -129,  813, 7, -620,
                -0x2F74E06, -0x2E0E3A8, -0xE2C6119
            ),
        },
        .localVelocityZ = -0xCBFFC,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3988,
        .modelBaseYaw = 424,
        .modelBaseRoll = -3,
        .projectedPos = DEMO_VEC3(-0x2F5CCCE, -0x2D43644, -0xE2BACDD),
        .movementSpeed = 0xC9202,
        .collisionVelocityX = -0x79CBD,
        .collisionVelocityZ = -0xA00DC,
        .pitchAngle = 3988,
        .facingAngle = 424,
        .leanAngle = -3,
        .surfacePitchAngle = -108,
        .surfaceRollAngle = -4051,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCC400,
        .speedLimit = 0xCC400,
        .rankSpeedBonus = 0x3000,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 112, 3296, 0, 0, 0x9BD80, -0xAC00),
            DEMO_MODEL_PART(1, 4, 304, 3384, 3610, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 714, 3384, 3610, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3074, 3429, 687, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 376, 3456, 3872, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 280, 3744, 72, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 120, 3800, 3912, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 104, 3976, 3248, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3928, 3640, 160, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3512, 3968, 688, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 444,
        .animationFrameCount = 36,
        .animationFramesRemaining = 33,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x2FE4586, -0x2E6627F, -0xE2F96DF),
            DEMO_VEC3(-0x2F1E2AA, -0x2E2E881, -0xE1F5003),
            DEMO_VEC3(-0x2F75172, -0x2E5C296, -0xE34E437),
            DEMO_VEC3(-0x2EAEE96, -0x2E24898, -0xE249D5B),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107600),
        .shadowMtx = DEMO_HEAP_MATRIX(107600 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x2FE4586, -0x2E5177C, -0xE2F96DF),
            DEMO_VEC3(-0x2F1E2AA, -0x2E1AACC, -0xE1F5003),
            DEMO_VEC3(-0x2F75172, -0x2E505FC, -0xE34E437),
            DEMO_VEC3(-0x2EAEE96, -0x2E1994C, -0xE249D5B),
            DEMO_VEC3(-0x2F6A8C9, -0x2E2F484, -0xE259603),
            DEMO_VEC3(-0x2EA45ED, -0x2DF87D4, -0xE154F27),
            DEMO_VEC3(-0x2EFB4B5, -0x2E2E304, -0xE2AE35B),
            DEMO_VEC3(-0x2E351D9, -0x2DF7654, -0xE1A9C7F),
        },
        .groundContactMask = 15,
        .coursePathIndex = 13,
        .coursePathOffset = 0x9D57FD,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(99920),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = 0.0f,
        .replayFrame = 385,
    } },
    /* Segment 6, frame 312. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x2392C460, -0x1780F2B7, -0x18DC93A0),
        .renderPosition = DEMO_VEC3(-0x2392C460, -0x17828BC5, -0x18DC93A0),
        .previousPosition = DEMO_VEC3(-0x2387D639, -0x177ECBEA, -0x18E3A3D9),
        .velocity = DEMO_VEC3(-0xAEE27, -0x2E80D, 0x71039),
        .groundOffset = 0x23F0A,
        .collisionBottomY = -0x1787B3F7,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1532,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x232D3,
        .modeValue = 1024,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -847, -175, 547,  -140, 1008, 105,  -557, 12, -858,
                -0x2392C460, -0x17828BC5, -0x18DC93A0
            ),
            DEMO_TRANSFORM(
                -610, -165, 804,  -395, 938, -107,  -720, -375, -623,
                -0x23934534, -0x177A0924, -0x18DAA394
            ),
            DEMO_TRANSFORM(
                -484, -575, 693,  85, 755, 685,  -897, 382, -309,
                -0x2391E4EC, -0x177AF064, -0x18DBAFB0
            ),
            DEMO_TRANSFORM(
                -484, -575, 693,  -794, 644, -21,  -425, -549, -751,
                -0x239239EC, -0x177DE364, -0x18DE5CF0
            ),
            DEMO_TRANSFORM(
                -697, 461, 590,  729, 602, 389,  -172, 686, -739,
                -0x2393AE9C, -0x177B6C24, -0x18D95420
            ),
            DEMO_TRANSFORM(
                -662, 309, 716,  278, 971, -162,  -728, 89, -712,
                -0x239687DC, -0x177DC6E4, -0x18DAD9E0
            ),
            DEMO_TRANSFORM(
                -676, -290, 710,  -401, 941, 1,  -654, -278, -736,
                -0x23934534, -0x177A0924, -0x18DAA394
            ),
            DEMO_TRANSFORM(
                173, -46, 1007,  -590, 823, 139,  -817, -605, 112,
                -0x23942730, -0x1777F7D4, -0x18DAA2BC
            ),
            DEMO_TRANSFORM(
                -500, -211, 867,  -332, 966, 44,  -828, -260, -541,
                -0x2392671C, -0x17777D3C, -0x18DC5F0C
            ),
            DEMO_TRANSFORM(
                13, -989, 260,  -490, 221, 869,  -897, -136, -472,
                -0x238FE20C, -0x177ABFA4, -0x18DEA92C
            ),
            DEMO_TRANSFORM(
                -887, 30, 509,  -27, 1017, -109,  -509, -108, -881,
                -0x2395B50C, -0x1778E80C, -0x18D8E69C
            ),
            DEMO_TRANSFORM(
                -515, 789, 397,  802, 611, -172,  -371, 224, -927,
                -0x2397C3FC, -0x177CCE8C, -0x18D73AFC
            ),
            DEMO_TRANSFORM(
                -847, -175, 547,  -140, 1008, 105,  -557, 12, -858,
                -0x239049A4, -0x177F1641, -0x18DD3E28
            ),
            DEMO_TRANSFORM(
                -847, -175, 547,  -140, 1008, 105,  -557, 12, -858,
                -0x23954024, -0x17801D21, -0x18DA0948
            ),
        },
        .localVelocityZ = -0xD3620,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3984,
        .modelBaseYaw = -0x7A8A,
        .projectedPos = DEMO_VEC3(-0x23947D60, -0x17752077, -0x18DB76A0),
        .movementSpeed = 0xD0390,
        .grabInputFrames = 13,
        .grabTrickId = 1,
        .grabHoldFrames = 24,
        .aerialTrickId = 6,
        .trickAttackCompletedTrickMask = 0x40,
        .collisionVelocityX = -0xAEE27,
        .collisionVelocityZ = 0x71039,
        .pitchAngle = 3984,
        .facingAngle = -0x7A8A,
        .surfacePitchAngle = -112,
        .surfaceRollAngle = -5,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .subStateStep = 1,
        .targetSpeedLimit = 0xD3A20,
        .speedLimit = 0xD3A20,
        .rankSpeedBonus = 0xA620,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 210, 3296, 0, 0, 0x8A800, -0x14000),
            DEMO_MODEL_PART(1, 4, 206, 3433, 3487, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 971, 3433, 3487, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2655, 3497, 900, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3336, 3424, 712, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 472, 3240, 3776, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 304, 3832, 16, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 192, 3376, 4016, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 664, 3904, 3224, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 2976, 3208, 1192, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 2904, 3760, 984, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 1584,
        .animationFrameCount = 36,
        .animationFramesRemaining = 14,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x2398F7CA, -0x1784FCF5, -0x18D35CF0),
            DEMO_VEC3(-0x2387CDFA, -0x178165D0, -0x18DE73CC),
            DEMO_VEC3(-0x239DBAC6, -0x1785111C, -0x18DAB374),
            DEMO_VEC3(-0x238C90F6, -0x178179F7, -0x18E5CA50),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140368),
        .shadowMtx = DEMO_HEAP_MATRIX(140368 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x2398F7CA, -0x17845FB9, -0x18D35CF0),
            DEMO_VEC3(-0x2387CDFA, -0x1780D29D, -0x18DE73CC),
            DEMO_VEC3(-0x239DBAC6, -0x178444ED, -0x18DAB374),
            DEMO_VEC3(-0x238C90F6, -0x1780B7D1, -0x18E5CA50),
            DEMO_VEC3(-0x238E09A3, -0x17821659, -0x18DA6D29),
            DEMO_VEC3(-0x237CDFD3, -0x177E893D, -0x18E58405),
            DEMO_VEC3(-0x2392CC9F, -0x1781FB8D, -0x18E1C3AD),
            DEMO_VEC3(-0x2381A2CF, -0x177E6E71, -0x18ECDA89),
        },
        .groundContactMask = 15,
        .coursePathIndex = 52,
        .coursePathOffset = 0xBAC71B,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132944),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 1330,
    } },
    /* Segment 7, frame 620. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x2C53107B, -0x19BA9675, -0x116D0445),
        .renderPosition = DEMO_VEC3(-0x2C53107B, -0x19BCBFB8, -0x116D0445),
        .previousPosition = DEMO_VEC3(-0x2C5178BD, -0x19B85C39, -0x11744439),
        .velocity = DEMO_VEC3(-0x197BE, -0x2FB7C, 0x73FF4),
        .groundOffset = 0x2D901,
        .collisionBottomY = -0x19C157B5,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 4030,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x2608D,
        .modeValue = 0x42000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -208, -319, 950,  -47, 972, 316,  -1000, 20, -213,
                -0x2C53107B, -0x19BCBFB8, -0x116D0445
            ),
            DEMO_TRANSFORM(
                140, -304, 967,  -325, 911, 333,  -960, -353, 28,
                -0x2C527551, -0x19B43111, -0x116A0177
            ),
            DEMO_TRANSFORM(
                167, -651, 771,  420, 755, 546,  -917, 227, 391,
                -0x2C524431, -0x19B4DBD5, -0x116BD457
            ),
            DEMO_TRANSFORM(
                167, -651, 771,  -528, 609, 629,  -860, -501, -236,
                -0x2C53E8F1, -0x19B7CF55, -0x116DF6D7
            ),
            DEMO_TRANSFORM(
                -29, 272, 986,  682, 740, -184,  -762, 651, -203,
                -0x2C51DB01, -0x19B5BFD5, -0x1168FEB7
            ),
            DEMO_TRANSFORM(
                100, -76, 1016,  250, 990, 49,  -986, 243, 116,
                -0x2C548581, -0x19B8A415, -0x116846B7
            ),
            DEMO_TRANSFORM(
                452, -445, 802,  -253, 798, 586,  -881, -459, 242,
                -0x2C527551, -0x19B43111, -0x116A0177
            ),
            DEMO_TRANSFORM(
                710, -72, 733,  -471, 737, 529,  -565, -705, 478,
                -0x2C53040D, -0x19B26FE9, -0x1168B76B
            ),
            DEMO_TRANSFORM(
                915, -159, 428,  -13, 949, 381,  -456, -347, 847,
                -0x2C540F29, -0x19B18B39, -0x116AD1A7
            ),
            DEMO_TRANSFORM(
                356, -944, -171,  943, 312, 244,  -173, -243, 979,
                -0x2C563E31, -0x19B4DD19, -0x116D5B1F
            ),
            DEMO_TRANSFORM(
                798, -141, 624,  6, 999, 217,  -639, -165, 781,
                -0x2C51D939, -0x19B3B869, -0x1166E687
            ),
            DEMO_TRANSFORM(
                438, 814, 438,  -603, 619, -547,  -700, -23, 745,
                -0x2C4FEC81, -0x19B7F849, -0x1166395F
            ),
            DEMO_TRANSFORM(
                -208, -319, 950,  -47, 972, 316,  -1000, 20, -213,
                -0x2C51DBB7, -0x19B8FAC0, -0x116EB775
            ),
            DEMO_TRANSFORM(
                -208, -319, 950,  -47, 972, 316,  -1000, 20, -213,
                -0x2C5314D7, -0x19BADA60, -0x11692675
            ),
        },
        .localVelocityZ = -0x7D021,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3889,
        .modelBaseYaw = 0x1773,
        .modelBaseRoll = 6,
        .projectedPos = DEMO_VEC3(-0x2C53DDFB, -0x19AF3035, -0x11695B85),
        .movementSpeed = 0x76C36,
        .grabInputFrames = 21,
        .trickAttackCompletedTrickMask = 0x40,
        .collisionVelocityX = -0x197BE,
        .collisionVelocityZ = 0x73FF4,
        .pitchAngle = 3889,
        .facingAngle = 0x1773,
        .leanAngle = 6,
        .surfacePitchAngle = -207,
        .surfaceRollAngle = -46,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xD4A40,
        .speedLimit = 0xA4400,
        .rankSpeedBonus = 0xB640,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 175, 3296, 0, 0, 0x90980, -0x10C00),
            DEMO_MODEL_PART(1, 4, 241, 3415, 3531, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 880, 3415, 3531, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2803, 3473, 825, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3488, 3328, 416, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 368, 3520, 3888, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 280, 3720, 88, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 40, 3928, 4048, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 4056, 4056, 3280, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 4016, 3792, 8, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3704, 4056, 680, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 564,
        .animationFrameCount = 36,
        .animationFramesRemaining = 31,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x2C50E75B, -0x19C0AED8, -0x11627B3D),
            DEMO_VEC3(-0x2C4CAC77, -0x19BA2B67, -0x1175BB5D),
            DEMO_VEC3(-0x2C59747F, -0x19C16335, -0x11644D2D),
            DEMO_VEC3(-0x2C55399B, -0x19BAC58B, -0x11778D4D),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(138960),
        .shadowMtx = DEMO_HEAP_MATRIX(138960 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x2C50E75B, -0x19C01338, -0x11627B3D),
            DEMO_VEC3(-0x2C4CAC77, -0x19B99884, -0x1175BB5D),
            DEMO_VEC3(-0x2C59747F, -0x19BFE6EC, -0x11644D2D),
            DEMO_VEC3(-0x2C55399B, -0x19B96C38, -0x11778D4D),
            DEMO_VEC3(-0x2C4F4F57, -0x19BD9EB9, -0x1169BCD5),
            DEMO_VEC3(-0x2C4B1473, -0x19B72405, -0x117CFCF5),
            DEMO_VEC3(-0x2C57DD07, -0x19BD6895, -0x116B8B7D),
            DEMO_VEC3(-0x2C53A223, -0x19B6EDE1, -0x117ECB9D),
        },
        .groundContactMask = 15,
        .coursePathIndex = 62,
        .coursePathOffset = 0xB2FC0,
        .rankIndex = 2,
        .shotType = 5,
        .shotCount = 3,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132816),
        .loopSoundVolume = 116,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .speedFan = {
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x2BDCC04D, -0x1975648D, -0x1213D6C8),
            .frontTransform = DEMO_TRANSFORM(
                -671, 323, -700,  -562, 430, 738,  528, 870, -105,
                -0x2BDCC04D, -0x1975648D, -0x1213D6C8
            ),
            .backTransform = DEMO_TRANSFORM(
                -671, 323, -700,  -528, -870, 105,  -562, 430, 738,
                -0x2BE402BD, -0x1970891D, -0x121018A8
            ),
            .modelYaw = 2048,
            .spinAngleX = -0x7400,
            .frontMatrix = DEMO_HEAP_MATRIX(99920),
            .backMatrix = DEMO_HEAP_MATRIX(99920 + sizeof(Mtx)),
            .scaleStep = 16,
        },
        .replayFrame = 1689,
    } },
    /* Segment 8, frame 674. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x2C742C27, -0x19EC6CA6, -0x10D650E3),
        .renderPosition = DEMO_VEC3(-0x2C742C27, -0x19EFC770, -0x10D650E3),
        .previousPosition = DEMO_VEC3(-0x2C71E4B7, -0x19E8EA1C, -0x10E0B08F),
        .velocity = DEMO_VEC3(-0x24770, -0x428AD, 0xA5FAC),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x19F312C9,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1334,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x36DAA,
        .modeValue = 0x42000,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -208, -319, 950,  -56, 972, 314,  -1000, 11, -215,
                -0x2C742C27, -0x19EFC770, -0x10D650E3
            ),
            DEMO_TRANSFORM(
                140, -301, 968,  -238, 940, 326,  -985, -270, 58,
                -0x2C740F31, -0x19E685BC, -0x10D32D7C
            ),
            DEMO_TRANSFORM(
                174, -585, 821,  201, 836, 554,  -988, 67, 257,
                -0x2C73F915, -0x19E73AB0, -0x10D4FEB4
            ),
            DEMO_TRANSFORM(
                174, -585, 821,  -411, 719, 600,  -921, -432, -113,
                -0x2C74C295, -0x19EA7F70, -0x10D728B4
            ),
            DEMO_TRANSFORM(
                49, 165, 1008,  472, 891, -170,  -906, 473, -32,
                -0x2C739015, -0x19E81C70, -0x10D22854
            ),
            DEMO_TRANSFORM(
                98, -73, 1016,  242, 993, 48,  -989, 235, 113,
                -0x2C756815, -0x19EB9830, -0x10D17E14
            ),
            DEMO_TRANSFORM(
                30, -425, 930,  -312, 882, 413,  -974, -297, -103,
                -0x2C740F31, -0x19E685BC, -0x10D32D7C
            ),
            DEMO_TRANSFORM(
                834, -104, 583,  -345, 732, 625,  -480, -707, 561,
                -0x2C74BF1D, -0x19E4959C, -0x10D244C0
            ),
            DEMO_TRANSFORM(
                263, -339, 929,  -234, 912, 399,  -960, -316, 157,
                -0x2C74BEC9, -0x19E3C2A4, -0x10D4A42C
            ),
            DEMO_TRANSFORM(
                224, -998, 33,  258, 91, 986,  -964, -207, 271,
                -0x2C747911, -0x19E67F84, -0x10D8785C
            ),
            DEMO_TRANSFORM(
                -270, -118, 980,  -128, 1011, 86,  -978, -100, -282,
                -0x2C749859, -0x19E5D6D4, -0x10D0190C
            ),
            DEMO_TRANSFORM(
                -109, 686, 751,  427, 716, -593,  -924, 249, -363,
                -0x2C74C0E9, -0x19EA147C, -0x10CE0AA4
            ),
            DEMO_TRANSFORM(
                -208, -319, 950,  -56, 972, 314,  -1000, 11, -215,
                -0x2C72FE23, -0x19EC0074, -0x10D804DF
            ),
            DEMO_TRANSFORM(
                -208, -319, 950,  -56, 972, 314,  -1000, 11, -215,
                -0x2C743743, -0x19EDE014, -0x10D273DF
            ),
        },
        .localVelocityZ = -0xB2DF8,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3889,
        .modelBaseYaw = 0x1773,
        .projectedPos = DEMO_VEC3(-0x2C74F9A7, -0x19E10666, -0x10D2A823),
        .movementSpeed = 0xA9F01,
        .grabInputFrames = 21,
        .trickAttackCompletedTrickMask = 0x40,
        .collisionVelocityX = -0x24770,
        .collisionVelocityZ = 0xA5FAC,
        .pitchAngle = 3889,
        .facingAngle = 0x1773,
        .surfacePitchAngle = -207,
        .surfaceRollAngle = -50,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xD4C60,
        .speedLimit = 0xB3200,
        .rankSpeedBonus = 0xB860,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 3488, 3328, 416, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 472, 3240, 3776, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 304, 3832, 16, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 192, 3376, 4016, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 664, 3904, 3224, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 2976, 3208, 1192, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 2904, 3760, 984, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 1584,
        .animationFrameCount = 36,
        .animationFramesRemaining = 14,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x2C720307, -0x19F3ACF3, -0x10CBC4D9),
            DEMO_VEC3(-0x2C6DC823, -0x19ED2982, -0x10DF04F9),
            DEMO_VEC3(-0x2C7A902B, -0x19F45F7A, -0x10CD9CCD),
            DEMO_VEC3(-0x2C765547, -0x19EDDC08, -0x10E0DCED),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(106192),
        .shadowMtx = DEMO_HEAP_MATRIX(106192 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x2C720307, -0x19F311A4, -0x10CBC4D9),
            DEMO_VEC3(-0x2C6DC823, -0x19EC96F0, -0x10DF04F9),
            DEMO_VEC3(-0x2C7A902B, -0x19F2F7F0, -0x10CD9CCD),
            DEMO_VEC3(-0x2C765547, -0x19EC7D3C, -0x10E0DCED),
            DEMO_VEC3(-0x2C6FBB97, -0x19EF8F1A, -0x10D62485),
            DEMO_VEC3(-0x2C6B80B3, -0x19E91466, -0x10E964A5),
            DEMO_VEC3(-0x2C7848BB, -0x19EF7566, -0x10D7FC79),
            DEMO_VEC3(-0x2C740DD7, -0x19E8FAB2, -0x10EB3C99),
        },
        .groundContactMask = 15,
        .coursePathIndex = 62,
        .coursePathOffset = 0xA0CF84,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(100048),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .speedFan = {
            .localOffset = DEMO_VEC3(-0x300000, 0, 0),
            .backOffset = DEMO_VEC3(0xF0000, 0x230000, 0),
            .worldPos = DEMO_VEC3(-0x2BDCC04D, -0x1975648D, -0x1213D6C8),
            .frontTransform = DEMO_TRANSFORM(
                -671, 323, -700,  -562, 430, 738,  528, 870, -105,
                -0x2BDCC04D, -0x1975648D, -0x1213D6C8
            ),
            .backTransform = DEMO_TRANSFORM(
                -671, 323, -700,  -528, -870, 105,  -562, 430, 738,
                -0x2BE402BD, -0x1970891D, -0x121018A8
            ),
            .modelYaw = 2048,
            .spinAngleX = -0x7400,
            .frontMatrix = DEMO_HEAP_MATRIX(99920),
            .backMatrix = DEMO_HEAP_MATRIX(99920 + sizeof(Mtx)),
            .scaleStep = 16,
        },
        .replayFrame = 1706,
    } },
    /* Segment 9, frame 723. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x72F6FB1, -0x40F8EBB, -0x1266FFE5),
        .renderPosition = DEMO_VEC3(-0x72F6FB1, -0x412E080, -0x1266FFE5),
        .previousPosition = DEMO_VEC3(-0x72771FA, -0x40B2E85, -0x125DFCE9),
        .velocity = DEMO_VEC3(-0x7FDB7, -0x5156C, -0x902FC),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x41643F1,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 3643,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x44A78,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -638, -348, -720,  -175, 959, -307,  780, -67, -658,
                -0x72F6FB1, -0x412E080, -0x1266FFE5
            ),
            DEMO_TRANSFORM(
                -864, -300, -457,  -75, 912, -456,  542, -351, -793,
                -0x7319DC9, -0x409B0B1, -0x12697F38
            ),
            DEMO_TRANSFORM(
                -780, -579, -320,  -527, 842, -239,  399, -17, -941,
                -0x73041FD, -0x40A5D11, -0x12684528
            ),
            DEMO_TRANSFORM(
                -780, -579, -320,  -196, 675, -742,  632, -504, -626,
                -0x72E327D, -0x40DA7D1, -0x126755A8
            ),
            DEMO_TRANSFORM(
                -822, 156, -587,  -115, 929, 411,  596, 397, -730,
                -0x732CA8D, -0x40B3EA1, -0x12699C48
            ),
            DEMO_TRANSFORM(
                -782, 79, 655,  -166, 959, -315,  -638, -348, -720,
                -0x73256CD, -0x40EE021, -0x126B3788
            ),
            DEMO_TRANSFORM(
                -882, -435, -281,  -208, 806, -593,  474, -454, -783,
                -0x7319DC9, -0x409B0B1, -0x12697F38
            ),
            DEMO_TRANSFORM(
                -1002, -26, 200,  -163, 703, -724,  -120, -742, -693,
                -0x73212C9, -0x407EB2D, -0x126ACD10
            ),
            DEMO_TRANSFORM(
                -874, -504, 166,  -530, 809, -330,  30, -369, -953,
                -0x72FDE39, -0x4070D61, -0x1269F808
            ),
            DEMO_TRANSFORM(
                -158, -988, 211,  -971, 207, 243,  -278, -164, -970,
                -0x72BA969, -0x408FB69, -0x12691580
            ),
            DEMO_TRANSFORM(
                -971, 78, -312,  145, 992, -199,  287, -233, -953,
                -0x7342D59, -0x4092DC1, -0x126B57E8
            ),
            DEMO_TRANSFORM(
                -676, 723, -255,  635, 719, 355,  431, 76, -924,
                -0x7371D79, -0x40CDD49, -0x126B53F8
            ),
            DEMO_TRANSFORM(
                -638, -348, -720,  -175, 959, -307,  780, -67, -658,
                -0x72EA6B1, -0x40EFEFC, -0x12654E71
            ),
            DEMO_TRANSFORM(
                -638, -348, -720,  -175, 959, -307,  780, -67, -658,
                -0x73264D1, -0x41108FC, -0x126986D1
            ),
        },
        .localVelocityZ = -0xCCF5E,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3870,
        .modelBaseYaw = -0x4E27,
        .modelBaseRoll = -54,
        .projectedPos = DEMO_VEC3(-0x73223F1, -0x40445BB, -0x126A0CA5),
        .movementSpeed = 0xC0B5C,
        .collisionVelocityX = -0x7FDB7,
        .collisionVelocityZ = -0x902FC,
        .pitchAngle = 3870,
        .facingAngle = -0x4E27,
        .leanAngle = -54,
        .surfacePitchAngle = -226,
        .surfaceRollAngle = -25,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCD360,
        .speedLimit = 0xCD360,
        .rankSpeedBonus = 0x3F60,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 384, 3400, 3856, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 280, 3768, 56, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 232, 3688, 3784, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 248, 3912, 3192, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3792, 3512, 352, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3320, 3864, 736, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 324,
        .animationFrameCount = 36,
        .animationFramesRemaining = 35,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x7393D93, -0x41723D2, -0x126B7B89),
            DEMO_VEC3(-0x72C4C0F, -0x4100D7E, -0x125CE33D),
            DEMO_VEC3(-0x7329353, -0x41769B2, -0x12711C8D),
            DEMO_VEC3(-0x725A1CF, -0x410535E, -0x12628441),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(139856),
        .shadowMtx = DEMO_HEAP_MATRIX(139856 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x7393D93, -0x4161D06, -0x126B7B89),
            DEMO_VEC3(-0x72C4C0F, -0x40F0FC6, -0x125CE33D),
            DEMO_VEC3(-0x7329353, -0x416B13A, -0x12711C8D),
            DEMO_VEC3(-0x725A1CF, -0x40FA3FA, -0x12628441),
            DEMO_VEC3(-0x73144C8, -0x4117E61, -0x1262821F),
            DEMO_VEC3(-0x7245344, -0x40A7121, -0x1253E9D3),
            DEMO_VEC3(-0x72A90B0, -0x4124D19, -0x12680FFF),
            DEMO_VEC3(-0x71D9F2C, -0x40B3FD9, -0x125977B3),
        },
        .groundContactMask = 15,
        .coursePathIndex = 24,
        .coursePathOffset = 0x875A45,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132688),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 508,
    } },
    /* Segment 10, frame 775. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x70F794C, -0x3FE0E23, -0x1242F47D),
        .renderPosition = DEMO_VEC3(-0x70F794C, -0x400D3BA, -0x1242F47D),
        .previousPosition = DEMO_VEC3(-0x7077BE4, -0x3F9AC1E, -0x1239F1DA),
        .velocity = DEMO_VEC3(-0x7FD68, -0x51261, -0x902A3),
        .groundOffset = 0x40000,
        .collisionBottomY = -0x404BE7F,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x9A69,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x44A4D,
        .previousInput = STICK_RIGHT,
        .stickX = 9,
        .stickY = -1,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -638, -348, -720,  -82, 945, -382,  795, -180, -618,
                -0x70F794C, -0x400D3BA, -0x1242F47D
            ),
            DEMO_TRANSFORM(
                -869, -262, -471,  18, 878, -523,  539, -453, -741,
                -0x710C679, -0x3F7B490, -0x1246334F
            ),
            DEMO_TRANSFORM(
                -814, -536, -311,  -445, 862, -322,  431, -121, -919,
                -0x70F8631, -0x3F864A4, -0x1244DEE7
            ),
            DEMO_TRANSFORM(
                -814, -536, -311,  -110, 630, -797,  609, -600, -560,
                -0x70DC8F1, -0x3FBC364, -0x12439CE7
            ),
            DEMO_TRANSFORM(
                -784, 178, -632,  -52, 964, 337,  654, 291, -730,
                -0x7121251, -0x3F929B4, -0x12464087
            ),
            DEMO_TRANSFORM(
                -796, 192, 613,  -73, 943, -390,  -638, -348, -720,
                -0x711DE51, -0x3FCEDF4, -0x12479247
            ),
            DEMO_TRANSFORM(
                -681, -344, -681,  -189, 960, -296,  739, -71, -703,
                -0x710C679, -0x3F7B490, -0x1246334F
            ),
            DEMO_TRANSFORM(
                -960, -226, 269,  -351, 670, -687,  -25, -739, -707,
                -0x71130ED, -0x3F59848, -0x1246D9CF
            ),
            DEMO_TRANSFORM(
                -595, -832, 20,  -506, 341, -820,  660, -487, -610,
                -0x70F7B29, -0x3F4FCD8, -0x12451D5F
            ),
            DEMO_TRANSFORM(
                21, -971, 322,  -788, -221, -613,  651, -235, -753,
                -0x70C0CC9, -0x3F44A30, -0x1241F5C7
            ),
            DEMO_TRANSFORM(
                -534, 416, -766,  314, 930, 287,  814, -85, -614,
                -0x712CF09, -0x3F6ABC8, -0x1248713F
            ),
            DEMO_TRANSFORM(
                -303, 805, -552,  229, 621, 780,  950, 107, -365,
                -0x7155719, -0x3F94A38, -0x124B6FB7
            ),
            DEMO_TRANSFORM(
                -638, -348, -720,  -82, 945, -382,  795, -180, -618,
                -0x70E6D2C, -0x3FCE7F2, -0x1241837D
            ),
            DEMO_TRANSFORM(
                -638, -348, -720,  -82, 945, -382,  795, -180, -618,
                -0x7122B4C, -0x3FEF1F2, -0x1245BBDD
            ),
        },
        .localVelocityZ = -0xCCEDF,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3870,
        .modelBaseYaw = -0x4E27,
        .modelBaseRoll = -131,
        .projectedPos = DEMO_VEC3(-0x7122D8C, -0x3F2C523, -0x1246013D),
        .movementSpeed = 0xC0AE5,
        .collisionVelocityX = -0x7FD68,
        .collisionVelocityZ = -0x902A3,
        .pitchAngle = 3870,
        .facingAngle = -0x4E27,
        .leanAngle = -131,
        .surfacePitchAngle = -226,
        .surfaceRollAngle = -26,
        .leanInput = -368,
        .leanScale = 33,
        .modeAngle = -115,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCD2E0,
        .speedLimit = 0xCD2E0,
        .rankSpeedBonus = 0x3EE0,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 5, 3109, 3995, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 363, 3773, -178, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 873, 3656, 3340, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 893, 4039, 3068, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3029, 3577, 992, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 2817, 3899, 986, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 2,
        .animationStartOffset = 2366,
        .animationFrameDataOffset = 2366,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x71957DC, -0x405B40B, -0x12479CB3),
            DEMO_VEC3(-0x70C6658, -0x3FE9DB7, -0x12390467),
            DEMO_VEC3(-0x7128C40, -0x405D847, -0x124CE493),
            DEMO_VEC3(-0x7059ABC, -0x3FEC1F3, -0x123E4C47),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140368),
        .shadowMtx = DEMO_HEAP_MATRIX(140368 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x71957DC, -0x40394EE, -0x12479CB3),
            DEMO_VEC3(-0x70C6658, -0x3FC87AE, -0x12390467),
            DEMO_VEC3(-0x7128C40, -0x4051FC6, -0x124CE493),
            DEMO_VEC3(-0x7059ABC, -0x3FE1286, -0x123E4C47),
            DEMO_VEC3(-0x7116158, -0x3FF1F9A, -0x123E928E),
            DEMO_VEC3(-0x7045DAC, -0x3F8125A, -0x123009D2),
            DEMO_VEC3(-0x70A9A1C, -0x400C0DE, -0x1243D9E2),
            DEMO_VEC3(-0x6FD9670, -0x3F9B39E, -0x12355126),
        },
        .groundContactMask = 15,
        .coursePathIndex = 24,
        .coursePathOffset = 0x577DE5,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132688),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 504,
    } },
    /* Segment 11, frame 825. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xF5D2087, -0xB12BC84, -0x1EB6328A),
        .renderPosition = DEMO_VEC3(-0xF5D2087, -0xB13693F, -0x1EB6328A),
        .previousPosition = DEMO_VEC3(-0xF5582EB, -0xB0F235E, -0x1EAC6105),
        .velocity = DEMO_VEC3(-0x79D9C, -0x45A66, -0x9D185),
        .groundOffset = 0x1E558,
        .collisionBottomY = -0xB197DC4,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x989D,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3A52F,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -601, -292, -775,  -289, 971, -141,  775, 136, -653,
                -0xF5D2087, -0xB13693F, -0x1EB6328A
            ),
            DEMO_TRANSFORM(
                -828, -317, -510,  -80, 919, -440,  595, -317, -769,
                -0xF608CCC, -0xB0B97DA, -0x1EB67DC9
            ),
            DEMO_TRANSFORM(
                -619, -716, -389,  -810, 591, 200,  85, 429, -924,
                -0xF5F3CF4, -0xB0C4036, -0x1EB534B5
            ),
            DEMO_TRANSFORM(
                -619, -716, -389,  -148, 579, -829,  801, -445, -454,
                -0xF5C1274, -0xB0E8FB6, -0x1EB5FCB5
            ),
            DEMO_TRANSFORM(
                -703, 362, -648,  -455, 495, 770,  587, 818, -179,
                -0xF61AA54, -0xB0D2E56, -0x1EB6B365
            ),
            DEMO_TRANSFORM(
                -779, -124, 651,  -280, 973, -149,  -601, -292, -775,
                -0xF5FE354, -0xB0F1E16, -0x1EB9B5E5
            ),
            DEMO_TRANSFORM(
                -842, -416, -405,  -188, 871, -501,  548, -338, -794,
                -0xF608CCC, -0xB0B97DA, -0x1EB67DC9
            ),
            DEMO_TRANSFORM(
                -1003, -131, 150,  -197, 772, -641,  -31, -658, -782,
                -0xF60F6F8, -0xB09ADEA, -0x1EB797BD
            ),
            DEMO_TRANSFORM(
                -807, -628, 23,  -621, 791, -184,  94, -160, -1005,
                -0xF5EDC74, -0xB08E05A, -0x1EB67B49
            ),
            DEMO_TRANSFORM(
                -39, -1022, -29,  -1006, 33, 183,  -182, 35, -1006,
                -0xF5A7684, -0xB0A6F9A, -0x1EB5D1D1
            ),
            DEMO_TRANSFORM(
                -940, 150, -375,  168, 1009, -15,  367, -75, -951,
                -0xF62F9E4, -0xB0AE85A, -0x1EB87589
            ),
            DEMO_TRANSFORM(
                -667, 746, -210,  565, 657, 544,  531, 238, -840,
                -0xF65ED64, -0xB0E7B4A, -0x1EB95111
            ),
            DEMO_TRANSFORM(
                -601, -292, -775,  -289, 971, -141,  775, 136, -653,
                -0xF5CC85B, -0xB0FCEFB, -0x1EB3DCFA
            ),
            DEMO_TRANSFORM(
                -601, -292, -775,  -289, 971, -141,  775, 136, -653,
                -0xF604E3B, -0xB11861B, -0x1EB8677A
            ),
        },
        .localVelocityZ = -0xCF820,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3907,
        .modelBaseYaw = -0x1E52,
        .modelBaseRoll = 83,
        .projectedPos = DEMO_VEC3(-0xF5F3A47, -0xB073CC4, -0x1EB8E84A),
        .movementSpeed = 0xC6CFF,
        .grabInputFrames = 24,
        .collisionVelocityX = -0x79D9C,
        .collisionVelocityZ = -0x9D185,
        .pitchAngle = 3907,
        .facingAngle = -0x1E52,
        .leanAngle = 83,
        .surfacePitchAngle = -189,
        .leanInput = 112,
        .leanScale = 32,
        .modeAngle = 31,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCFC20,
        .speedLimit = 0xCFC20,
        .rankSpeedBonus = 0x6820,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 229, 3296, 0, 0, 0x86FC0, -0x15E00),
            DEMO_MODEL_PART(1, 4, 187, 3442, 3463, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 1023, 3442, 3463, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2571, 3510, 943, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 373, 3367, 3941, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 290, 3754, 95, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 231, 3659, 3775, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 310, 3877, 3164, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3692, 3518, 510, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3247, 3877, 830, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2488,
        .animationFrameDataOffset = 2488,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0xF6688C5, -0xB178F89, -0x1EBB427A),
            DEMO_VEC3(-0xF5A59C9, -0xB1199B3, -0x1EAB8E2A),
            DEMO_VEC3(-0xF5FE745, -0xB17AA14, -0x1EC0D6EA),
            DEMO_VEC3(-0xF53B849, -0xB11B43D, -0x1EB1229A),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140368),
        .shadowMtx = DEMO_HEAP_MATRIX(140368 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0xF6688C5, -0xB16F551, -0x1EBB427A),
            DEMO_VEC3(-0xF5A59C9, -0xB1106AD, -0x1EAB8E2A),
            DEMO_VEC3(-0xF5FE745, -0xB15CBD1, -0x1EC0D6EA),
            DEMO_VEC3(-0xF53B849, -0xB0FDD2D, -0x1EB1229A),
            DEMO_VEC3(-0xF5EE223, -0xB1338D9, -0x1EB16D21),
            DEMO_VEC3(-0xF52B327, -0xB0D4A35, -0x1EA1B8D1),
            DEMO_VEC3(-0xF5852AF, -0xB11D4D5, -0x1EB70939),
            DEMO_VEC3(-0xF4C23B3, -0xB0BE631, -0x1EA754E9),
        },
        .groundContactMask = 15,
        .coursePathIndex = 33,
        .coursePathOffset = 0x1196E2C,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132688),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 834,
    } },
    /* Segment 12, frame 878. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x14F1428B, -0xD222C42, -0x20D626C4),
        .renderPosition = DEMO_VEC3(-0x14F1428B, -0xD24E677, -0x20D626C4),
        .previousPosition = DEMO_VEC3(-0x14E57496, -0xD1CC812, -0x20D691D2),
        .velocity = DEMO_VEC3(-0xBCDF5, -0x62570, 0x6B0E),
        .groundOffset = 0x39598,
        .collisionBottomY = -0xD28ED82,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x3B63,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x5703E,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -927, -432, 32,  -428, 926, 69,  -58, 49, -1020,
                -0x14F1428B, -0xD24E677, -0x20D626C4
            ),
            DEMO_TRANSFORM(
                -855, -419, 374,  -494, 884, -138,  -266, -297, -942,
                -0x14F53010, -0xD1C5A06, -0x20D4B3F8
            ),
            DEMO_TRANSFORM(
                -653, -705, 351,  -550, 734, 452,  -563, 99, -848,
                -0x14F354BC, -0xD1CD11A, -0x20D5150C
            ),
            DEMO_TRANSFORM(
                -653, -705, 351,  -787, 603, -252,  -33, -431, -927,
                -0x14F12E7C, -0xD1FAF9A, -0x20D6D90C
            ),
            DEMO_TRANSFORM(
                -977, 81, 291,  226, 853, 517,  -202, 559, -833,
                -0x14F5D62C, -0xD1E0BEA, -0x20D3FC2C
            ),
            DEMO_TRANSFORM(
                53, -38, 1021,  -429, 927, 57,  -927, -432, 32,
                -0x14F6B82C, -0xD21612A, -0x20D601EC
            ),
            DEMO_TRANSFORM(
                -713, -556, 478,  -664, 771, -94,  -308, -376, -899,
                -0x14F53010, -0xD1C5A06, -0x20D4B3F8
            ),
            DEMO_TRANSFORM(
                -432, -172, 911,  -734, 676, -220,  -565, -747, -409,
                -0x14F6A5FC, -0xD1AA80E, -0x20D4E8FC
            ),
            DEMO_TRANSFORM(
                -344, -626, 732,  -646, 726, 318,  -714, -355, -639,
                -0x14F4BE58, -0xD197C4E, -0x20D60DD8
            ),
            DEMO_TRANSFORM(
                156, -1002, 134,  -398, 63, 940,  -929, -196, -380,
                -0x14F16130, -0xD1ACB16, -0x20D915D8
            ),
            DEMO_TRANSFORM(
                -833, -60, 591,  -154, 1004, -115,  -573, -184, -826,
                -0x14F83A88, -0xD1C343E, -0x20D3B858
            ),
            DEMO_TRANSFORM(
                -665, 619, 469,  602, 801, -203,  -490, 143, -886,
                -0x14F9A8D0, -0xD2046A6, -0x20D1D338
            ),
            DEMO_TRANSFORM(
                -927, -432, 32,  -428, 926, 69,  -58, 49, -1020,
                -0x14EFC177, -0xD20F4AF, -0x20D54BAC
            ),
            DEMO_TRANSFORM(
                -927, -432, 32,  -428, 926, 69,  -58, 49, -1020,
                -0x14F530B7, -0xD237D0F, -0x20D51A8C
            ),
        },
        .localVelocityZ = -0xD0820,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3812,
        .modelBaseYaw = 0x4417,
        .modelBaseRoll = 27,
        .projectedPos = DEMO_VEC3(-0x14F651CB, -0xD174B82, -0x20D5F904),
        .movementSpeed = 0xBCFDA,
        .grabInputFrames = 24,
        .collisionVelocityX = -0xBCDF5,
        .collisionVelocityZ = 0x6B0E,
        .pitchAngle = 3812,
        .facingAngle = 0x4417,
        .leanAngle = 27,
        .surfacePitchAngle = -284,
        .surfaceRollAngle = -10,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xD0C20,
        .speedLimit = 0xD0C20,
        .rankSpeedBonus = 0x7820,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 135, 3296, 0, 0, 0x97DC0, -0xCE00),
            DEMO_MODEL_PART(1, 4, 281, 3395, 3582, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 773, 3395, 3582, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2978, 3445, 736, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 384, 3400, 3856, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 280, 3768, 56, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 232, 3688, 3784, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 248, 3912, 3192, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3792, 3512, 352, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3320, 3864, 736, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 324,
        .animationFrameCount = 36,
        .animationFramesRemaining = 35,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x14FA6843, -0xD2A1EC8, -0x20D17560),
            DEMO_VEC3(-0x14E79CDB, -0xD214E1A, -0x20D21F44),
            DEMO_VEC3(-0x14FAE83B, -0xD2A5ACD, -0x20DA2E44),
            DEMO_VEC3(-0x14E81CD3, -0xD218A1F, -0x20DAD828),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140368),
        .shadowMtx = DEMO_HEAP_MATRIX(140368 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x14FA6843, -0xD297D7B, -0x20D17560),
            DEMO_VEC3(-0x14E79CDB, -0xD20BB2F, -0x20D21F44),
            DEMO_VEC3(-0x14FAE83B, -0xD2911BF, -0x20DA2E44),
            DEMO_VEC3(-0x14E81CD3, -0xD204F73, -0x20DAD828),
            DEMO_VEC3(-0x14EE93BE, -0xD23F106, -0x20D1E1CC),
            DEMO_VEC3(-0x14DBC856, -0xD1B2EBA, -0x20D28BB0),
            DEMO_VEC3(-0x14EF20D6, -0xD236966, -0x20DA97F4),
            DEMO_VEC3(-0x14DC556E, -0xD1AA71A, -0x20DB41D8),
        },
        .groundContactMask = 15,
        .coursePathIndex = 40,
        .coursePathOffset = 0xAF7496,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132688),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 962,
    } },
    /* Segment 13, frame 928. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0xFFFD694, -0xB59D5B8, -0x1F5EC0CF),
        .renderPosition = DEMO_VEC3(-0xFFFD694, -0xB5C19B8, -0x1F5EC0CF),
        .previousPosition = DEMO_VEC3(-0xFF5D3DB, -0xB55ED01, -0x1F577E5A),
        .velocity = DEMO_VEC3(-0xA02B9, -0x49C49, -0x74275),
        .groundOffset = 0x40000,
        .collisionBottomY = -0xB60894A,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0x11C00,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x3DB51,
        .previousInput = STICK_LEFT,
        .stickX = -21,
        .stickY = -3,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -789, -309, -572,  -394, 943, 34,  517, 246, -847,
                -0xFFFD694, -0xB5C19B8, -0x1F5EC0CF
            ),
            DEMO_TRANSFORM(
                -919, -370, -253,  -343, 952, -144,  288, -44, -980,
                -0x1003ED63, -0xB5345C1, -0x1F5DE0A1
            ),
            DEMO_TRANSFORM(
                -758, -648, -229,  -684, 742, 165,  62, 275, -983,
                -0x1002293B, -0xB53E4A5, -0x1F5D5479
            ),
            DEMO_TRANSFORM(
                -758, -648, -229,  -520, 763, -438,  449, -207, -895,
                -0xFFF7CFB, -0xB56CAA5, -0x1F5DF979
            ),
            DEMO_TRANSFORM(
                -984, 102, -258,  -94, 769, 667,  261, 666, -730,
                -0x1004DADB, -0xB54FA55, -0x1F5E12C9
            ),
            DEMO_TRANSFORM(
                -522, -235, 847,  -387, 946, 24,  -789, -309, -572,
                -0x10047C1B, -0xB57FB95, -0x1F60AE09
            ),
            DEMO_TRANSFORM(
                -963, -307, -156,  -169, 826, -579,  300, -519, -828,
                -0x1003ED63, -0xB5345C1, -0x1F5DE0A1
            ),
            DEMO_TRANSFORM(
                -968, -90, 318,  -284, 730, -657,  -169, -710, -715,
                -0x10044C97, -0xB517521, -0x1F5F2651
            ),
            DEMO_TRANSFORM(
                -698, -675, 319,  -643, 766, 215,  -381, -54, -947,
                -0x1001E7FB, -0xB50E891, -0x1F5EA079
            ),
            DEMO_TRANSFORM(
                18, -1008, -170,  -840, -112, 572,  -582, 129, -830,
                -0xFFDB043, -0xB524039, -0x1F603F19
            ),
            DEMO_TRANSFORM(
                -930, 424, 7,  422, 925, -105,  -51, -92, -1017,
                -0x10069C0B, -0xB5268F1, -0x1F5F63C9
            ),
            DEMO_TRANSFORM(
                -494, 895, 11,  858, 470, 298,  255, 153, -978,
                -0x100A8773, -0xB54FD61, -0x1F5EF5B1
            ),
            DEMO_TRANSFORM(
                -789, -309, -572,  -394, 943, 34,  517, 246, -847,
                -0xFFF0ED8, -0xB589C6C, -0x1F5C5AB3
            ),
            DEMO_TRANSFORM(
                -789, -309, -572,  -394, 943, 34,  517, 246, -847,
                -0x1003AF78, -0xB5A6C4C, -0x1F5FB5D3
            ),
        },
        .localVelocityZ = -0xCFA39,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3896,
        .modelBaseYaw = -0x1D99,
        .modelBaseRoll = 159,
        .projectedPos = DEMO_VEC3(-0x1002C554, -0xB4E65B8, -0x1F60E14F),
        .movementSpeed = 0xC5DA9,
        .grabInputFrames = 24,
        .collisionVelocityX = -0xA02B9,
        .collisionVelocityZ = -0x74275,
        .pitchAngle = 3896,
        .facingAngle = -0x1D99,
        .leanAngle = 159,
        .surfacePitchAngle = -200,
        .surfaceRollAngle = -18,
        .leanInput = 368,
        .leanScale = 35,
        .modeAngle = 120,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xCFE80,
        .speedLimit = 0xCFE80,
        .rankSpeedBonus = 0x6A80,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 111, 3296, 0, 0, 0x9C140, -0xAA00),
            DEMO_MODEL_PART(1, 4, 305, 3383, 3612, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 710, 3383, 3612, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 3080, 3429, 684, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 317, 3362, 0x1073, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 298, 3691, 233, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 111, 3715, 3818, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 256, 3844, 3128, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3634, 3699, 714, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3270, 4029, 988, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2488,
        .animationFrameDataOffset = 2488,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x100A0D8E, -0xB60DF0D, -0x1F60EFB3),
            DEMO_VEC3(-0xFFA0CBA, -0xB5A984A, -0x1F55548F),
            DEMO_VEC3(-0x1005A06E, -0xB61533B, -0x1F682D0F),
            DEMO_VEC3(-0xFF59F9A, -0xB5B0C78, -0x1F5C91EB),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(107088),
        .shadowMtx = DEMO_HEAP_MATRIX(107088 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x100A0D8E, -0xB6049B8, -0x1F60EFB3),
            DEMO_VEC3(-0xFFA0CBA, -0xB5A057C, -0x1F55548F),
            DEMO_VEC3(-0x1005A06E, -0xB5E2DF4, -0x1F682D0F),
            DEMO_VEC3(-0xFF59F9A, -0xB57E9B8, -0x1F5C91EB),
            DEMO_VEC3(-0xFFFF9C5, -0xB5C5D31, -0x1F59CE30),
            DEMO_VEC3(-0xFF01D41, -0xB5618F5, -0x1F4E0078),
            DEMO_VEC3(-0xFFB8A75, -0xB5A1265, -0x1F60FC3C),
            DEMO_VEC3(-0xFEBADF1, -0xB53CE29, -0x1F552E84),
        },
        .groundContactMask = 3,
        .coursePathIndex = 34,
        .coursePathOffset = 0xCD72DC,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(99920),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 853,
    } },
    /* Segment 14, frame 980. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x11243F34, -0xBC5AF3C, -0x1FFD0B01),
        .renderPosition = DEMO_VEC3(-0x11243F34, -0xBC67BE9, -0x1FFD0B01),
        .previousPosition = DEMO_VEC3(-0x1118602C, -0xBC1D722, -0x1FF90883),
        .velocity = DEMO_VEC3(-0xBDF08, -0x4995A, -0x4027E),
        .groundOffset = 0x24F4F,
        .collisionBottomY = -0xBCC707C,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 0xE2A2,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x34A4A,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -946, -258, -291,  -304, 967, 133,  242, 210, -971,
                -0x11243F34, -0xBC67BE9, -0x1FFD0B01
            ),
            DEMO_TRANSFORM(
                -974, -309, 53,  -313, 953, -199,  11, -205, -1002,
                -0x11271FCA, -0xBBE842D, -0x1FFABE1D
            ),
            DEMO_TRANSFORM(
                -757, -688, 6,  -540, 600, 628,  -425, 461, -807,
                -0x1125507E, -0xBBF39E9, -0x1FFA93CD
            ),
            DEMO_TRANSFORM(
                -757, -688, 6,  -608, 665, -481,  319, -360, -902,
                -0x1123347E, -0xBC191E9, -0x1FFD07CD
            ),
            DEMO_TRANSFORM(
                -963, 342, -42,  137, 495, 884,  317, 827, -512,
                -0x11282AFE, -0xBC02239, -0x1FFA6C0D
            ),
            DEMO_TRANSFORM(
                -246, -199, 973,  -301, 970, 122,  -946, -258, -291,
                -0x1128B4BE, -0xBC21179, -0x1FFDE0CD
            ),
            DEMO_TRANSFORM(
                -990, -207, 152,  -256, 807, -573,  -4, -593, -833,
                -0x11271FCA, -0xBBE842D, -0x1FFABE1D
            ),
            DEMO_TRANSFORM(
                -842, 2, 580,  -406, 726, -594,  -413, -719, -597,
                -0x1127B036, -0xBBCBDF5, -0x1FFC0091
            ),
            DEMO_TRANSFORM(
                -595, -620, 554,  -469, 813, 406,  -687, -18, -757,
                -0x11253542, -0xBBC6F0D, -0x1FFC3C35
            ),
            DEMO_TRANSFORM(
                -105, -1005, -157,  -609, -65, 819,  -815, 178, -591,
                -0x1121EBD2, -0xBBE187D, -0x1FFF2D05
            ),
            DEMO_TRANSFORM(
                -814, 555, 272,  485, 852, -290,  -385, -101, -942,
                -0x112A0B12, -0xBBD71CD, -0x1FFB7D45
            ),
            DEMO_TRANSFORM(
                -351, 954, 109,  959, 354, -14,  -51, 97, -1017,
                -0x112DED62, -0xBBF6B5D, -0x1FF9B0CD
            ),
            DEMO_TRANSFORM(
                -946, -258, -291,  -304, 967, 133,  242, 210, -971,
                -0x11228B20, -0xBC30C4D, -0x1FFB15AD
            ),
            DEMO_TRANSFORM(
                -946, -258, -291,  -304, 967, 133,  242, 210, -971,
                -0x112816E0, -0xBC48F4D, -0x1FFCCB4D
            ),
        },
        .localVelocityZ = -0xCF825,
        .localVelocityX = 0x5849,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3930,
        .modelBaseYaw = -0x1CC3,
        .modelBaseRoll = 132,
        .projectedPos = DEMO_VEC3(-0x112722B4, -0xBBA12FC, -0x1FFDEF01),
        .movementSpeed = 0xC87B5,
        .grabInputFrames = 24,
        .collisionVelocityX = -0xBDF08,
        .collisionVelocityZ = -0x4027E,
        .pitchAngle = 3930,
        .facingAngle = -0x1CC3,
        .leanAngle = 132,
        .surfacePitchAngle = -166,
        .surfaceRollAngle = -197,
        .leanInput = 240,
        .leanScale = 32,
        .modeAngle = 139,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xD01E0,
        .speedLimit = 0xD01E0,
        .rankSpeedBonus = 0x6DE0,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 206, 3296, 0, 0, 0x8B340, -0x13A00),
            DEMO_MODEL_PART(1, 4, 210, 3431, 3492, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 960, 3431, 3492, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2672, 3494, 891, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 305, 3361, 0x10AC, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 299, 3677, 262, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 86, 3727, 3827, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 245, 3837, 3121, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 3621, 3737, 757, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 3275, 4062, 1022, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 3,
        .animationStartOffset = 2488,
        .animationFrameDataOffset = 2488,
        .animationFrameCount = 2,
        .animationFramesRemaining = 3,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x112EDF26, -0xBCA9789, -0x1FFBD89B),
            DEMO_VEC3(-0x111BB12E, -0xBC558E5, -0x1FF5EF27),
            DEMO_VEC3(-0x112CCD3A, -0xBCDCD01, -0x200426DB),
            DEMO_VEC3(-0x11199F42, -0xBC7C44B, -0x1FFE3D67),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(139344),
        .shadowMtx = DEMO_HEAP_MATRIX(139344 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x112EDF26, -0xBC9FF55, -0x1FFBD89B),
            DEMO_VEC3(-0x111BB12E, -0xBC4C4F5, -0x1FF5EF27),
            DEMO_VEC3(-0x112CCD3A, -0xBC832DD, -0x200426DB),
            DEMO_VEC3(-0x11199F42, -0xBC2F87D, -0x1FFE3D67),
            DEMO_VEC3(-0x1122F4A2, -0xBC69AE6, -0x1FF7DB95),
            DEMO_VEC3(-0x110FC6AA, -0xBC16086, -0x1FF1F221),
            DEMO_VEC3(-0x1120F9AE, -0xBC48756, -0x20001EE5),
            DEMO_VEC3(-0x110DCBB6, -0xBBF4CF6, -0x1FFA3571),
        },
        .groundContactMask = 3,
        .coursePathIndex = 35,
        .coursePathOffset = 0xE3DFD2,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132688),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.09941520541906357f,
        .replayFrame = 880,
    } },
    /* Segment 15, frame 1043. */
    { .player = {
        .playerIndex = 3,
        .characterId = RACE_CHARACTER_LINDA,
        .snowboardIndex = 6,
        .snowboardTextureIndex = 4,
        .isActive = 1,
        .replayInputSource = RACE_PLAYER_INPUT_REPLAY,
        .pos = DEMO_VEC3(-0x1BC5004E, -0x122B2B13, -0x1D2E3C1B),
        .renderPosition = DEMO_VEC3(-0x1BC5004E, -0x122B6C3D, -0x1D2E3C1B),
        .previousPosition = DEMO_VEC3(-0x1BBA1914, -0x1226C3E9, -0x1D340045),
        .velocity = DEMO_VEC3(-0xAE73A, -0x5286A, 0x5C42A),
        .groundOffset = 0xE561,
        .collisionBottomY = -0x1231EC53,
        .targetGroundOffset = 0x40000,
        .groundCorrectionY = 1079,
        .collisionCenterOffset = 0xC0000,
        .verticalVelocityLimit = -0x46A7F,
        .stanceChangeTimer = 6,
        .modelPartTransforms = {
            DEMO_TRANSFORM(
                -850, -349, 449,  -302, 962, 174,  -482, 11, -903,
                -0x1BC5004E, -0x122B6C3D, -0x1D2E3C1B
            ),
            DEMO_TRANSFORM(
                -639, -329, 728,  -590, 822, -147,  -537, -512, -704,
                -0x1BC683E3, -0x1224179B, -0x1D2B68AD
            ),
            DEMO_TRANSFORM(
                -411, -772, 531,  90, 544, 861,  -932, 393, -151,
                -0x1BC4DB73, -0x12249D57, -0x1D2C4B99
            ),
            DEMO_TRANSFORM(
                -411, -772, 531,  -934, 382, -167,  -73, -551, -858,
                -0x1BC535B3, -0x1226BE17, -0x1D2FA959
            ),
            DEMO_TRANSFORM(
                -858, 380, 407,  550, 453, 734,  92, 835, -584,
                -0x1BC6BB43, -0x12259417, -0x1D2A2969
            ),
            DEMO_TRANSFORM(
                478, 0, 905,  -308, 962, 163,  -850, -349, 449,
                -0x1BC8E183, -0x12275997, -0x1D2D07E9
            ),
            DEMO_TRANSFORM(
                -164, -449, 904,  -660, 740, 248,  -763, -544, -409,
                -0x1BC683E3, -0x1224179B, -0x1D2B68AD
            ),
            DEMO_TRANSFORM(
                -159, -71, 1008,  -747, 694, -68,  -679, -747, -160,
                -0x1BC7F76B, -0x12227713, -0x1D2ADD2D
            ),
            DEMO_TRANSFORM(
                440, 227, 895,  -395, 943, -44,  -835, -327, 493,
                -0x1BC7677B, -0x12218C43, -0x1D2D21D5
            ),
            DEMO_TRANSFORM(
                522, -872, 111,  622, 458, 671,  -622, -274, 764,
                -0x1BC6EF4B, -0x1225C94B, -0x1D2F252D
            ),
            DEMO_TRANSFORM(
                450, -255, 882,  -111, 960, 335,  -911, -244, 395,
                -0x1BC834CB, -0x1223BE73, -0x1D28B785
            ),
            DEMO_TRANSFORM(
                -8, 879, 523,  -446, 468, -792,  -921, -235, 379,
                -0x1BC6AB53, -0x12281EE3, -0x1D27DECD
            ),
            DEMO_TRANSFORM(
                -850, -349, 449,  -302, 962, 174,  -482, 11, -903,
                -0x1BC30AE2, -0x12279695, -0x1D2E616B
            ),
            DEMO_TRANSFORM(
                -850, -349, 449,  -302, 962, 174,  -482, 11, -903,
                -0x1BC80702, -0x1229A2D5, -0x1D2BBECB
            ),
        },
        .localVelocityZ = -0xD1F20,
        .baseSpeedLimit = 0xC9400,
        .gravity = 0xB540,
        .aerialGravity = 0x6AC0,
        .turnStrength = 1488,
        .turnRadiusAtFullLean = 0x1000,
        .turnRadiusAtHalfLean = 0x3000,
        .lateralDeceleration = 0x6B00,
        .forwardDeceleration = 1024,
        .reverseDeceleration = 1024,
        .collisionRadius = 0x80000,
        .collisionHeight = 0xE0000,
        .modelBasePitch = 3869,
        .modelBaseYaw = -0x7AC3,
        .projectedPos = DEMO_VEC3(-0x1BC89E8E, -0x121FE393, -0x1D2C525B),
        .movementSpeed = 0xC5588,
        .grabInputFrames = 5,
        .collisionVelocityX = -0xAE73A,
        .collisionVelocityZ = 0x5C42A,
        .pitchAngle = 3869,
        .facingAngle = -0x7AC3,
        .surfacePitchAngle = -227,
        .surfaceRollAngle = -4087,
        .leanScale = 32,
        .stateFlags = 0x180,
        .updateState = 1,
        .targetSpeedLimit = 0xD2320,
        .speedLimit = 0xD2320,
        .rankSpeedBonus = 0x8F20,
        .modelParts = {
            DEMO_MODEL_PART(255, 0, 3072, 3072, 1016, 0, 0, 0),
            DEMO_MODEL_PART(0, 1, 284, 3296, 0, 0, 0x7D240, -0x1B200),
            DEMO_MODEL_PART(1, 4, 132, 3470, 3393, -0x18000, -0x14000, 0),
            DEMO_MODEL_PART(2, 5, 1168, 3470, 3393, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 2, 2333, 3548, 1063, 0x18000, -0x14000, 0),
            DEMO_MODEL_PART(4, 3, 0, 0, 0, 0, -0x40000, 0),
            DEMO_MODEL_PART(1, 6, 368, 3616, 3896, 0, 0, 0),
            DEMO_MODEL_PART(6, 7, 288, 3688, 112, 0, 0x24000, 0),
            DEMO_MODEL_PART(6, 10, 4088, 32, 144, -0x28000, 0x20000, 0),
            DEMO_MODEL_PART(8, 11, 3848, 56, 3296, -0x28000, -0x40000, 0),
            DEMO_MODEL_PART(6, 8, 4040, 4032, 3944, 0x28000, 0x20000, 0),
            DEMO_MODEL_PART(10, 9, 4008, 40, 720, 0x28000, -0x40000, 0),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, -0x30000, 0x30000, -0xC000),
            DEMO_MODEL_PART(0, 0, 3072, 3072, 1016, 0x30000, 0x30000, -0xC000),
        },
        .modelPartCount = 14,
        .animationId = 1,
        .animationStartOffset = 204,
        .animationFrameDataOffset = 744,
        .animationFrameCount = 36,
        .animationFramesRemaining = 28,
        .animationFrameDuration = 1,
        .shadowCorners = {
            DEMO_VEC3(-0x1BCB8F7A, -0x122FBEE7, -0x1D25D1A3),
            DEMO_VEC3(-0x1BBA5236, -0x1228A2CD, -0x1D2EEEB7),
            DEMO_VEC3(-0x1BCFAE66, -0x122FA01E, -0x1D2D897F),
            DEMO_VEC3(-0x1BBE7122, -0x12288404, -0x1D36A693),
        },
        .shadowVtx = DEMO_HEAP_VERTICES(140368),
        .shadowMtx = DEMO_HEAP_MATRIX(140368 + 4 * sizeof(Vtx)),
        .boardContactPoints = {
            DEMO_VEC3(-0x1BCB8F7A, -0x122F0355, -0x1D25D1A3),
            DEMO_VEC3(-0x1BBA5236, -0x1227EE4D, -0x1D2EEEB7),
            DEMO_VEC3(-0x1BCFAE66, -0x122EEA2D, -0x1D2D897F),
            DEMO_VEC3(-0x1BBE7122, -0x1227D525, -0x1D36A693),
            DEMO_VEC3(-0x1BC0A840, -0x122A8415, -0x1D2B95CD),
            DEMO_VEC3(-0x1BAF6AFC, -0x12236F0D, -0x1D34B2E1),
            DEMO_VEC3(-0x1BC4C72C, -0x122A6AED, -0x1D334DA9),
            DEMO_VEC3(-0x1BB389E8, -0x122355E5, -0x1D3C6ABD),
        },
        .groundContactMask = 15,
        .coursePathIndex = 47,
        .coursePathOffset = 0x2998B8,
        .rankIndex = 3,
        .shotType = 5,
        .shotCount = 3,
        .itemType = 1,
        .randomIndex = 73,
        .modelPartMatrices = DEMO_MODEL_MATRICES(132688),
        .loopSoundVolume = 127,
        .loopSoundId = SFX_BOARD_GLIDE_LOOP,
        .loopSoundPitch = -0.005847953259944916f,
        .replayFrame = 1146,
    } },
},
// clang-format on
