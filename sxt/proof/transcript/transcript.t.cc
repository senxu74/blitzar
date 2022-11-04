#include "sxt/proof/transcript/transcript.h"

#include <array>
#include <iostream>

#include "sxt/base/test/unit_test.h"

using namespace sxt::prft;

TEST_CASE("simple protocol with one message and one challenge") {
  transcript real_transcript("test protocol");

  uint8_t label_data[] = "some data";
  real_transcript.append_message("some label", {label_data, sizeof(label_data) - 1});

  std::array<uint8_t, 32> real_challenge = {};

  real_transcript.challenge_bytes(real_challenge, "challenge");

  std::array<uint8_t, 32> expected_challenge = {
      213, 162, 25,  114, 208, 213, 254, 50,  12, 13,  38, 63,  172, 127, 255, 184,
      20,  90,  166, 64,  175, 110, 155, 202, 23, 124, 3,  199, 239, 207, 6,   21};

  REQUIRE(real_challenge == expected_challenge);
}

TEST_CASE("complex protocol with multiple messages and challenges") {
  /// these messages are long enough to wrap around the sponge state, and
  /// with multiple rounds of messages and challenges.

  transcript real_transcript("test protocol");

  std::array<uint8_t, 1024> data;
  memset(data.data(), 99, data.size());

  uint8_t label_data[] = "some data";
  real_transcript.append_message("step1", {label_data, sizeof(label_data) - 1});

  std::array<uint8_t, 32> real_challenge;
  std::vector<std::array<uint8_t, 32>> expected_challenges = {
      {182, 26, 85, 64,  195, 220, 22, 229, 215, 70,  194, 46,  11,  253, 53, 9,
       98,  30, 55, 224, 136, 161, 68, 85,  129, 129, 58,  116, 110, 156, 18, 234},
      {198, 108, 19, 209, 95,  244, 91, 83,  24, 238, 44,  239, 153, 211, 235, 131,
       70,  235, 3,  116, 104, 9,   14, 227, 37, 105, 113, 70,  23,  198, 108, 120},
      {202, 177, 119, 84,  68, 106, 171, 174, 233, 185, 98, 0,  108, 133, 69, 127,
       214, 30,  225, 199, 14, 231, 90,  111, 146, 219, 67, 11, 180, 196, 74, 125},
      {21, 160, 217, 83,  173, 241, 181, 90, 210, 222, 254, 102, 73, 85, 134, 98,
       0,  186, 44,  251, 60,  29,  2,   32, 158, 253, 99,  105, 27, 7,  226, 62},
      {183, 89,  138, 214, 149, 171, 241, 184, 104, 35, 1,   209, 61,  58,  18, 8,
       84,  124, 121, 205, 0,   216, 253, 92,  120, 23, 189, 155, 238, 163, 6,  208},
      {59, 85, 24,  239, 252, 168, 226, 121, 66,  66, 90,  141, 2,   114, 35,  218,
       76, 43, 134, 34,  111, 36,  127, 76,  234, 45, 240, 31,  113, 83,  188, 154},
      {164, 150, 67, 25, 102, 29,  213, 34, 242, 249, 189, 103, 110, 28, 151, 92,
       160, 0,   74, 87, 223, 146, 252, 93, 202, 254, 144, 209, 205, 25, 171, 75},
      {153, 7,   235, 184, 12, 44, 134, 92,  130, 194, 22,  66, 237, 122, 57,  26,
       43,  253, 87,  81,  6,  71, 176, 115, 170, 166, 189, 14, 80,  180, 137, 65},
      {157, 232, 67,  134, 75, 164, 235, 75,  219, 156, 177, 124, 128, 129, 65,  140,
       79,  166, 210, 80,  87, 110, 107, 225, 156, 164, 97,  60,  5,   44,  230, 145},
      {102, 55, 113, 222, 201, 95,  15,  77, 247, 53,  173, 121, 194, 10,  219, 85,
       152, 96, 26,  126, 195, 217, 109, 57, 186, 107, 232, 214, 171, 193, 155, 204},
      {75,  59, 43, 134, 70,  121, 26,  91,  173, 173, 129, 178, 119, 178, 97,  245,
       201, 36, 3,  253, 112, 253, 228, 243, 9,   188, 11,  22,  245, 30,  107, 171},
      {162, 1,   164, 112, 244, 16, 56, 21,  74, 125, 29, 48,  197, 33,  141, 225,
       238, 196, 65,  132, 26,  93, 31, 124, 86, 44,  21, 252, 49,  234, 39,  70},
      {154, 95, 89,  14,  36,  148, 198, 13, 179, 213, 233, 86, 254, 10,  193, 224,
       113, 80, 189, 208, 194, 32,  246, 38, 26,  162, 208, 36, 132, 145, 20,  54},
      {216, 41, 177, 255, 238, 180, 32,  74, 177, 210, 239, 126, 115, 55, 192, 56,
       43,  60, 62,  223, 16,  228, 249, 64, 115, 205, 29,  120, 44,  57, 190, 4},
      {137, 20, 144, 115, 25,  48,  84,  103, 13, 253, 174, 115, 235, 54, 105, 189,
       222, 73, 117, 151, 225, 225, 145, 106, 97, 138, 115, 37,  232, 23, 82,  24},
      {50, 53,  178, 227, 33, 151, 78, 155, 225, 91, 45,  111, 90,  115, 212, 198,
       5,  163, 204, 104, 31, 236, 19, 171, 35,  39, 210, 235, 224, 186, 254, 50},
      {11, 190, 153, 75, 159, 14, 154, 149, 166, 70, 166, 125, 87, 45,  82, 161,
       20, 49,  105, 44, 98,  68, 219, 142, 208, 25, 124, 217, 17, 167, 55, 155},
      {246, 162, 184, 209, 116, 255, 25, 42, 34,  189, 111, 94,  212, 151, 58,  140,
       134, 57,  138, 111, 95,  12,  31, 7,  161, 44,  112, 252, 142, 186, 126, 53},
      {147, 236, 141, 141, 51, 192, 213, 166, 124, 87,  177, 102, 50, 69, 190, 11,
       6,   81,  120, 70,  1,  128, 179, 144, 231, 195, 243, 198, 87, 6,  10,  171},
      {253, 94,  196, 121, 101, 242, 124, 165, 222, 203, 34, 117, 234, 58, 39,  155,
       201, 185, 20,  139, 29,  85,  182, 109, 101, 129, 56, 8,   188, 50, 234, 131},
      {163, 219, 255, 178, 196, 156, 154, 2,   255, 89,  186, 255, 1,   63,  148, 96,
       85,  17,  24,  22,  121, 200, 166, 167, 161, 153, 111, 8,   177, 236, 44,  131},
      {98,  151, 172, 242, 214, 5,   75,  35, 94,  144, 244, 66, 35,  173, 218, 171,
       139, 153, 11,  114, 12,  147, 220, 25, 182, 15,  84,  0,  108, 216, 21,  33},
      {214, 229, 192, 219, 16,  57, 255, 125, 40,  223, 239, 1,   53, 230, 56, 126,
       61,  160, 87,  83,  154, 72, 94,  162, 156, 249, 173, 166, 41, 213, 9,  1},
      {22, 144, 29,  161, 227, 139, 247, 222, 142, 39,  224, 109, 27,  215, 251, 104,
       68, 150, 211, 98,  17,  239, 22,  152, 42,  253, 73,  141, 184, 239, 25,  200},
      {124, 134, 236, 31, 106, 151, 136, 26,  218, 42,  242, 177, 75, 243, 3,  209,
       2,   188, 113, 95, 200, 250, 104, 151, 71,  130, 36,  111, 93, 99,  98, 17},
      {231, 171, 4,   2,   4,   191, 251, 115, 86,  77, 225, 41,  20,  180, 209, 131,
       104, 82,  211, 118, 152, 238, 91,  239, 241, 8,  159, 161, 193, 15,  210, 242},
      {241, 211, 165, 14, 207, 138, 133, 20, 237, 4,   245, 1,  16, 66,  151, 158,
       254, 68,  8,   75, 141, 195, 50,  69, 101, 156, 103, 64, 13, 206, 134, 134},
      {23, 44, 142, 91,  119, 198, 101, 194, 205, 159, 168, 244, 253, 148, 45,  197,
       1,  55, 166, 228, 103, 24,  215, 19,  160, 240, 28,  208, 243, 2,   110, 243},
      {99,  147, 223, 102, 143, 28, 22,  240, 139, 30, 180, 244, 214, 83, 38,  129,
       218, 190, 32,  92,  36,  0,  249, 221, 39,  29, 215, 216, 174, 67, 231, 81},
      {132, 131, 79, 136, 140, 77, 135, 228, 239, 150, 45, 191, 94,  17,  96, 156,
       204, 49,  38, 167, 200, 41, 192, 179, 96,  2,   59, 17,  181, 102, 35, 124},
      {163, 246, 168, 5,  111, 225, 224, 144, 160, 213, 160, 127, 35,  97,  248, 184,
       4,   163, 228, 93, 245, 199, 74,  129, 232, 193, 196, 48,  238, 126, 36,  248},
      {168, 201, 51, 245, 79,  174, 118, 227, 249, 190, 169, 54,  72, 193, 48, 142,
       125, 250, 33, 82,  221, 81,  103, 79,  243, 202, 67,  131, 81, 207, 0,  60}};

  for (size_t i = 0; i < 32; ++i) {
    real_transcript.challenge_bytes(real_challenge, "challenge");

    REQUIRE(real_challenge == expected_challenges[i]);

    real_transcript.append_message("bigdata", data);

    real_transcript.append_message("challengedata", real_challenge);
  }
}
