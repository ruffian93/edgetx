/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#pragma once
#include "datahelpers.h"

#include <QtCore>
#include <QObject>
#include <QString>

class AbstractStaticItemModel;
class SemanticVersion;
class BoardJson;
class GeneralSettings;

// identiying names of static abstract item models
constexpr char AIM_BOARDS_POT_TYPE[]        {"boards.pottype"};
constexpr char AIM_BOARDS_SLIDER_TYPE[]     {"boards.slidertype"};
constexpr char AIM_BOARDS_SWITCH_TYPE[]     {"boards.switchtype"};
constexpr char AIM_BOARDS_MODULE_SIZE[]     {"boards.extmodulesize"};
constexpr char AIM_BOARDS_FLEX_TYPE[]       {"boards.flextype"};

// TODO create a Board class with all these functions

namespace Board {

  enum Type
  {
    BOARD_UNKNOWN = -1,
    BOARD_SKY9X,
    BOARD_9XRPRO,
    BOARD_AR9X,
    BOARD_TARANIS_X7,
    BOARD_TARANIS_X7_ACCESS,
    BOARD_TARANIS_X9D,
    BOARD_TARANIS_X9DP,
    BOARD_TARANIS_X9DP_2019,
    BOARD_TARANIS_X9E,
    BOARD_HORUS_X12S,
    BOARD_X10,
    BOARD_X10_EXPRESS,
    BOARD_TARANIS_XLITE,
    BOARD_TARANIS_XLITES,
    BOARD_TARANIS_X9LITE,
    BOARD_TARANIS_X9LITES,
    BOARD_JUMPER_T12,
    BOARD_JUMPER_T16,
    BOARD_RADIOMASTER_TX16S,
    BOARD_JUMPER_T18,
    BOARD_JUMPER_T20,
    BOARD_RADIOMASTER_TX12,
    BOARD_RADIOMASTER_TX12_MK2,
    BOARD_RADIOMASTER_BOXER,
    BOARD_RADIOMASTER_T8,
    BOARD_JUMPER_TLITE,
    BOARD_JUMPER_TLITE_F4,
    BOARD_FLYSKY_NV14,
    BOARD_FLYSKY_PL18,
    BOARD_RADIOMASTER_ZORRO,
    BOARD_JUMPER_TPRO,
    BOARD_BETAFPV_LR3PRO,
    BOARD_IFLIGHT_COMMANDO8,
    BOARD_FLYSKY_EL18,
    BOARD_JUMPER_TPROV2,
    BOARD_RADIOMASTER_POCKET,
    BOARD_TYPE_COUNT,
    BOARD_TYPE_MAX = BOARD_TYPE_COUNT - 1
  };

  enum PotType
  {
    POT_NONE,
    POT_WITH_DETENT,
    POT_MULTIPOS_SWITCH,
    POT_WITHOUT_DETENT,
    POT_SLIDER_WITH_DETENT,
    POT_TYPE_COUNT
  };

  enum SliderType
  {
    SLIDER_NONE,
    SLIDER_WITH_DETENT,
    SLIDER_TYPE_COUNT
  };

  enum SwitchType
  {
    SWITCH_NOT_AVAILABLE,
    SWITCH_TOGGLE,
    SWITCH_2POS,
    SWITCH_3POS,
    SWITCH_FUNC,
    SWITCH_TYPE_COUNT
  };

  enum StickAxes {
    STICK_AXIS_LH = 0,
    STICK_AXIS_LV,
    STICK_AXIS_RV,
    STICK_AXIS_RH,
    STICK_AXIS_COUNT
  };

  enum TrimAxes {
    TRIM_AXIS_LH = 0,
    TRIM_AXIS_LV,
    TRIM_AXIS_RV,
    TRIM_AXIS_RH,
    TRIM_AXIS_T5,
    TRIM_AXIS_T6,
    TRIM_AXIS_T7,
    TRIM_AXIS_T8,
    TRIM_AXIS_COUNT
  };

  enum TrimSwitches
  {
    TRIM_SW_LH_DEC,
    TRIM_SW_LH_INC,
    TRIM_SW_LV_DEC,
    TRIM_SW_LV_INC,
    TRIM_SW_RV_DEC,
    TRIM_SW_RV_INC,
    TRIM_SW_RH_DEC,
    TRIM_SW_RH_INC,
    TRIM_SW_T5_DEC,
    TRIM_SW_T5_INC,
    TRIM_SW_T6_DEC,
    TRIM_SW_T6_INC,
    TRIM_SW_T7_DEC,
    TRIM_SW_T7_INC,
    TRIM_SW_T8_DEC,
    TRIM_SW_T8_INC,
    TRIM_SW_COUNT
  };

  enum Capability {
    FactoryInstalledPots,
    FactoryInstalledSwitches,
    FlexInputs,
    FlexSwitches,
    FunctionSwitches,
    Gyros,
    GyroAxes,
    HasAudioMuteGPIO,
    HasColorLcd,
    HasExternalModuleSupport,
    HasInternalModuleSupport,
    HasIntModuleHeartbeatGPIO,
    HasLedStripGPIO,
    HasRTC,
    HasSDCard,
    HasTrainerModuleCPPM,
    HasTrainerModuleSBUS,
    HasVBat,
    MaxAnalogs,
    Inputs,
    InputSwitches,
    Joysticks,
    JoystickAxes,
    MultiposPots,
    MultiposPotsPositions,
    NumFunctionSwitchesPositions,
    NumTrims,
    NumTrimSwitches,
    Pots,
    Sliders,
    SportMaxBaudRate,
    StandardSwitches,
    Sticks,
    Switches,
    SwitchesPositions,
  };

  struct SwitchPosition {
    SwitchPosition(unsigned int index, unsigned int position):
      index(index),
      position(position)
      {
      }
      unsigned int index;
      unsigned int position;
  };

  enum SwitchTypeMasks {
    SwitchTypeFlagNone    = 1 << 1,
    SwitchTypeFlag2Pos    = 1 << 2,
    SwitchTypeFlag3Pos    = 1 << 3,
    SwitchTypeContextNone = SwitchTypeFlagNone,
    SwitchTypeContext2Pos = SwitchTypeContextNone | SwitchTypeFlag2Pos,
    SwitchTypeContext3Pos = SwitchTypeContext2Pos | SwitchTypeFlag3Pos
  };

  enum ExternalModuleSizes {
    EXTMODSIZE_NONE,
    EXTMODSIZE_STD,
    EXTMODSIZE_SMALL,
    EXTMODSIZE_BOTH,
    EXTMODSIZE_COUNT
  };

  enum AnalogInputType
  {
    AIT_NONE,
    AIT_STICK,
    AIT_FLEX,
    AIT_VBAT,
    AIT_RTC_BAT,
    AIT_SWITCH,
  };

  enum FlexType {
    FLEX_NONE = 0,
    FLEX_POT,
    FLEX_POT_CENTER,
    FLEX_SLIDER,
    FLEX_MULTIPOS,
    FLEX_AXIS_X,
    FLEX_AXIS_Y,
    FLEX_SWITCH,
    FLEX_TYPE_COUNT
  };

  enum FlexTypeMasks {
    FlexTypeFlagNotSwitch   = 1 << 1,
    FlexTypeFlagSwitch      = 1 << 2,
    FlexTypeContextNoSwitch = FlexTypeFlagNotSwitch,
    FlexTypeContextSwitch   = FlexTypeContextNoSwitch | FlexTypeFlagSwitch
  };

  enum LookupValueType {
    LVT_TAG = 0,
    LVT_NAME
  };

  struct InputInfo {
    InputInfo() :
      type(AIT_NONE),
      tag(""),
      name(""),
      shortName(""),
      flexType(FLEX_NONE),
      inverted(false)
    {}

    AnalogInputType type;
    std::string tag;
    std::string name;
    std::string label;
    std::string shortName;
    FlexType flexType;
    bool inverted;
  };

  struct SwitchInfo {
    SwitchInfo() :
      type(SWITCH_NOT_AVAILABLE),
      tag(""),
      name(""),
      inverted(false)
    {}

    SwitchType type;
    std::string tag;
    std::string name;
    bool inverted;
  };
}

class Boards
{
  Q_DECLARE_TR_FUNCTIONS(Boards)

  public:

    Boards(Board::Type board);
    virtual ~Boards() {}

    void setBoardType(const Board::Type & board);
    Board::Type getBoardType() const { return m_boardType; }

    const uint32_t getFourCC() const { return getFourCC(m_boardType); }
    const int getEEpromSize() const { return getEEpromSize(m_boardType); }
    const int getFlashSize() const { return getFlashSize(m_boardType); }
    const int getCapability(Board::Capability capability) const { return getCapability(m_boardType, capability); }
    const bool isBoardCompatible(Board::Type board2) const { return isBoardCompatible(m_boardType, board2); }

    static uint32_t getFourCC(Board::Type board);
    static int getEEpromSize(Board::Type board);
    static int getFlashSize(Board::Type board);
    static int getCapability(Board::Type board, Board::Capability capability);
    static QString getAxisName(int index);
    static bool isBoardCompatible(Board::Type board1, Board::Type board2);
    static QString getBoardName(Board::Type board);
    static QString switchTypeToString(int value);
    static AbstractStaticItemModel * switchTypeItemModel();
    static AbstractStaticItemModel * intModuleTypeItemModel();
    static QList<int> getSupportedInternalModules(Board::Type board);
    static int getDefaultInternalModules(Board::Type board);
    static int getDefaultExternalModuleSize(Board::Type board);
    static QString externalModuleSizeToString(int value);
    static AbstractStaticItemModel * externalModuleSizeItemModel();

    static BoardJson* getBoardJson(Board::Type board = Board::BOARD_UNKNOWN);

    static int getInputsCalibrated(Board::Type board = Board::BOARD_UNKNOWN);

    static Board::InputInfo getInputInfo(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static int getInputIndex(QString val, Board::LookupValueType lvt, Board::Type board = Board::BOARD_UNKNOWN);
    static QString getInputName(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static int getInputPotIndex(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static int getInputSliderIndex(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static QString getInputTag(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static int getInputTagOffset(QString tag, Board::Type board = Board::BOARD_UNKNOWN);
    static int getInputTypeOffset(Board::AnalogInputType type, Board::Type board = Board::BOARD_UNKNOWN);
    static int getInputYamlIndex(QString val, int ylt, Board::Type board = Board::BOARD_UNKNOWN);
    static QString getInputYamlName(int index, int ylt, Board::Type board = Board::BOARD_UNKNOWN);

    static Board::SwitchInfo getSwitchInfo(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static int getSwitchIndex(QString val, Board::LookupValueType lvt, Board::Type board = Board::BOARD_UNKNOWN);
    static QString getSwitchName(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static QString getSwitchTag(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static int getSwitchTagNum(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static int getSwitchYamlIndex(QString val, int ylt, Board::Type board = Board::BOARD_UNKNOWN);
    static QString getSwitchYamlName(int index, int ylt, Board::Type board = Board::BOARD_UNKNOWN);

    static int getTrimYamlIndex(QString val, int ylt, Board::Type board = Board::BOARD_UNKNOWN);
    static QString getTrimYamlName(int index, int ylt, Board::Type board = Board::BOARD_UNKNOWN);

    STRINGTAGMAPPINGFUNCS(legacyTrimSourcesLookupTable, LegacyTrimSource);
    STRINGTAGMAPPINGFUNCS(trimSwitchesLookupTable, TrimSwitch);
    STRINGTAGMAPPINGFUNCS(rawSwitchTypesLookupTable, RawSwitchType);
    STRINGTAGMAPPINGFUNCS(rawSourceSpecialTypesLookupTable, RawSourceSpecialType);
    STRINGTAGMAPPINGFUNCS(rawSourceCyclicLookupTable, RawSourceCyclic);

    static bool isInputAvailable(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static bool isInputCalibrated(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static bool isInputConfigurable(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static bool isInputIgnored(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static bool isInputPot(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static bool isInputStick(int index, Board::Type board = Board::BOARD_UNKNOWN);

    static bool isSwitchConfigurable(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static bool isSwitchFlex(int index, Board::Type board = Board::BOARD_UNKNOWN);
    static bool isSwitchFunc(int index, Board::Type board = Board::BOARD_UNKNOWN);

    static QString flexTypeToString(int value);
    static AbstractStaticItemModel * flexTypeItemModel();

    static std::string getLegacyAnalogMappedInputTag(const char * legacytag, Board::Type board = Board::BOARD_UNKNOWN);

  private:

    Board::Type m_boardType = Board::BOARD_UNKNOWN;
    BoardJson* m_boardJson = nullptr;

    const StringTagMappingTable legacyTrimSourcesLookupTable;
    const StringTagMappingTable trimSwitchesLookupTable;
    const StringTagMappingTable rawSwitchTypesLookupTable;
    const StringTagMappingTable rawSourceSpecialTypesLookupTable;
    const StringTagMappingTable rawSourceCyclicLookupTable;

    static StringTagMappingTable getLegacyAnalogsLookupTable(Board::Type board = Board::BOARD_UNKNOWN);
};

// temporary aliases for transition period, use Boards class instead.
#define getBoardCapability(b__, c__)   Boards::getCapability(b__, c__)

inline bool IS_SKY9X(Board::Type board)
{
  return board == Board::BOARD_SKY9X || board == Board::BOARD_9XRPRO || board == Board::BOARD_AR9X;
}

inline bool IS_9XRPRO(Board::Type board)
{
  return board == Board::BOARD_9XRPRO;
}

inline bool IS_BETAFPV_LR3PRO(Board::Type board)
{
  return board == Board::BOARD_BETAFPV_LR3PRO;
}

inline bool IS_IFLIGHT_COMMANDO8(Board::Type board)
{
  return board == Board::BOARD_IFLIGHT_COMMANDO8;
}

inline bool IS_JUMPER_T12(Board::Type board)
{
  return board == Board::BOARD_JUMPER_T12;
}

inline bool IS_JUMPER_TLITE(Board::Type board)
{
  return board == Board::BOARD_JUMPER_TLITE || board == Board::BOARD_JUMPER_TLITE_F4;
}

inline bool IS_JUMPER_TPRO(Board::Type board)
{
  return board == Board::BOARD_JUMPER_TPRO || board == Board::BOARD_JUMPER_TPROV2;
}

inline bool IS_JUMPER_TPROV1(Board::Type board)
{
  return board == Board::BOARD_JUMPER_TPRO;
}

inline bool IS_JUMPER_TPROV2(Board::Type board)
{
  return board == Board::BOARD_JUMPER_TPROV2;
}

inline bool IS_JUMPER_T16(Board::Type board)
{
  return board == Board::BOARD_JUMPER_T16;
}

inline bool IS_JUMPER_T18(Board::Type board)
{
  return board == Board::BOARD_JUMPER_T18;
}

inline bool IS_JUMPER_T20(Board::Type board)
{
  return board == Board::BOARD_JUMPER_T20;
}

inline bool IS_RADIOMASTER_TX16S(Board::Type board)
{
  return board == Board::BOARD_RADIOMASTER_TX16S;
}

inline bool IS_RADIOMASTER_TX12(Board::Type board)
{
  return board == Board::BOARD_RADIOMASTER_TX12;
}

inline bool IS_RADIOMASTER_TX12_MK2(Board::Type board)
{
  return board == Board::BOARD_RADIOMASTER_TX12_MK2;
}

inline bool IS_RADIOMASTER_ZORRO(Board::Type board)
{
  return board == Board::BOARD_RADIOMASTER_ZORRO;
}

inline bool IS_RADIOMASTER_BOXER(Board::Type board)
{
  return board == Board::BOARD_RADIOMASTER_BOXER;
}

inline bool IS_RADIOMASTER_POCKET(Board::Type board)
{
  return board == Board::BOARD_RADIOMASTER_POCKET;
}


inline bool IS_RADIOMASTER_T8(Board::Type board)
{
  return board == Board::BOARD_RADIOMASTER_T8;
}

inline bool IS_FAMILY_T16(Board::Type board)
{
  return board == Board::BOARD_JUMPER_T16 || board == Board::BOARD_RADIOMASTER_TX16S || board == Board::BOARD_JUMPER_T18;
}

inline bool IS_FAMILY_T12(Board::Type board)
{
  return board == Board::BOARD_JUMPER_T12 ||
         board == Board::BOARD_JUMPER_T20 ||
         board == Board::BOARD_JUMPER_TLITE ||
         board == Board::BOARD_JUMPER_TLITE_F4 ||
         board == Board::BOARD_JUMPER_TPRO ||
         board == Board::BOARD_JUMPER_TPROV2 ||
         board == Board::BOARD_RADIOMASTER_TX12 ||
         board == Board::BOARD_RADIOMASTER_TX12_MK2 ||
         board == Board::BOARD_RADIOMASTER_ZORRO ||
         board == Board::BOARD_RADIOMASTER_BOXER ||
         board == Board::BOARD_RADIOMASTER_POCKET ||
         board == Board::BOARD_RADIOMASTER_T8 ||
         board == Board::BOARD_BETAFPV_LR3PRO ||
         board == Board::BOARD_IFLIGHT_COMMANDO8;
}

inline bool IS_FLYSKY_NV14(Board::Type board)
{
  return (board == Board::BOARD_FLYSKY_NV14);
}

inline bool IS_FLYSKY_EL18(Board::Type board)
{
  return (board == Board::BOARD_FLYSKY_EL18);
}

inline bool IS_FLYSKY_PL18(Board::Type board)
{
  return (board == Board::BOARD_FLYSKY_PL18);
}

inline bool IS_TARANIS_XLITE(Board::Type board)
{
  return board == Board::BOARD_TARANIS_XLITE || board == Board::BOARD_TARANIS_XLITES;
}

inline bool IS_TARANIS_XLITES(Board::Type board)
{
  return board == Board::BOARD_TARANIS_XLITES;
}

inline bool IS_TARANIS_X7(Board::Type board)
{
  return board == Board::BOARD_TARANIS_X7 || board == Board::BOARD_TARANIS_X7_ACCESS;
}

inline bool IS_TARANIS_X7_ACCESS(Board::Type board)
{
  return board == Board::BOARD_TARANIS_X7_ACCESS;
}

inline bool IS_TARANIS_X9LITE(Board::Type board)
{
  return board == Board::BOARD_TARANIS_X9LITE || board == Board::BOARD_TARANIS_X9LITES;
}

inline bool IS_TARANIS_X9LITES(Board::Type board)
{
  return board == Board::BOARD_TARANIS_X9LITES;
}

inline bool IS_TARANIS_X9(Board::Type board)
{
  return board==Board::BOARD_TARANIS_X9D || board==Board::BOARD_TARANIS_X9DP || board==Board::BOARD_TARANIS_X9DP_2019 || board==Board::BOARD_TARANIS_X9E;
}

inline bool IS_TARANIS_X9D(Board::Type board)
{
  return board == Board::BOARD_TARANIS_X9D || board == Board::BOARD_TARANIS_X9DP || board == Board::BOARD_TARANIS_X9DP_2019;
}

inline bool IS_TARANIS_PLUS(Board::Type board)
{
  return board == Board::BOARD_TARANIS_X9DP || board == Board::BOARD_TARANIS_X9E;
}

inline bool IS_TARANIS_X9E(Board::Type board)
{
  return board == Board::BOARD_TARANIS_X9E;
}

inline bool IS_TARANIS_SMALL(Board::Type board)
{
  return IS_TARANIS_X7(board) || IS_TARANIS_XLITE(board) || IS_TARANIS_X9LITE(board) || IS_FAMILY_T12(board);
}

inline bool IS_TARANIS(Board::Type board)
{
  return IS_TARANIS_X9(board) || IS_TARANIS_SMALL(board);
}

inline bool IS_HORUS_X10(Board::Type board)
{
  return board == Board::BOARD_X10 || board == Board::BOARD_X10_EXPRESS;
}

inline bool IS_HORUS_X12S(Board::Type board)
{
  return board == Board::BOARD_HORUS_X12S;
}

inline bool IS_FAMILY_HORUS(Board::Type board)
{
  return IS_HORUS_X12S(board) || IS_HORUS_X10(board);
}

inline bool IS_FAMILY_HORUS_OR_T16(Board::Type board)
{
  return IS_FAMILY_HORUS(board) || IS_FAMILY_T16(board) ||
    IS_FLYSKY_NV14(board)/*generally*/ || IS_FLYSKY_EL18(board)/*generally*/
    || IS_FLYSKY_PL18(board);
}

inline bool IS_HORUS_OR_TARANIS(Board::Type board)
{
  return IS_FAMILY_HORUS_OR_T16(board) || IS_TARANIS(board);
}

inline bool IS_STM32(Board::Type board)
{
  return IS_TARANIS(board) || IS_FAMILY_HORUS_OR_T16(board) ||
    IS_FLYSKY_NV14(board) || IS_FLYSKY_EL18(board) || IS_FLYSKY_PL18(board);
}

inline bool IS_ARM(Board::Type board)
{
  return IS_STM32(board) || IS_SKY9X(board);
}

inline bool HAS_LARGE_LCD(Board::Type board)
{
  return IS_FAMILY_HORUS_OR_T16(board) || IS_TARANIS_X9(board);
}

inline bool HAS_EXTERNAL_ANTENNA(Board::Type board)
{
  return (IS_FAMILY_HORUS(board) || IS_FAMILY_T16(board) || (IS_TARANIS_XLITE(board) && !IS_TARANIS_XLITES(board)));
}

inline bool IS_TARANIS_X9DP_2019(Board::Type board)
{
  return (board == Board::BOARD_TARANIS_X9DP_2019);
}

inline bool IS_ACCESS_RADIO(Board::Type board)
{
  return IS_TARANIS_XLITES(board) || IS_TARANIS_X9LITE(board) ||
         board == Board::BOARD_TARANIS_X9DP_2019 ||
         board == Board::BOARD_X10_EXPRESS || IS_TARANIS_X7_ACCESS(board);
}

inline bool IS_ACCESS_RADIO(Board::Type board, const QString & id)
{
  return IS_ACCESS_RADIO(board) ||
         (IS_FAMILY_HORUS_OR_T16(board) && id.contains("internalaccess"));
}

inline bool HAS_EEPROM_YAML(Board::Type board)
{
  return IS_FAMILY_HORUS_OR_T16(board);
}
