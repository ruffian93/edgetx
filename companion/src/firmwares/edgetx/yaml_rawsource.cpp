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

#include "yaml_rawsource.h"
#include "eeprominterface.h"
#include "boardjson.h"

static const YamlLookupTable spacemouseLut = {
  {  0, "SPACEMOUSE_A"  },
  {  1, "SPACEMOUSE_B"  },
  {  2, "SPACEMOUSE_C"  },
  {  3, "SPACEMOUSE_D"  },
  {  4, "SPACEMOUSE_E"  },
  {  5, "SPACEMOUSE_F"  },
};


std::string YamlRawSourceEncode(const RawSource& rhs)
{
  Board::Type board = getCurrentBoard();
  Boards b = Boards(board);
  std::string src_str;
  char c = 'A';
  switch (rhs.type) {
    case SOURCE_TYPE_VIRTUAL_INPUT:
      src_str += "I" + std::to_string(rhs.index);
      break;
    case SOURCE_TYPE_LUA_OUTPUT:
      src_str += "lua(";
      src_str += std::to_string(rhs.index / 16);
      src_str += ",";
      src_str += std::to_string(rhs.index % 16);
      src_str += ")";
      break;
    case SOURCE_TYPE_STICK:
      src_str = Boards::getInputYamlName(rhs.index, BoardJson::YLT_REF).toStdString();
      break;
    case SOURCE_TYPE_TRIM:
      src_str = Boards::getTrimYamlName(rhs.index, BoardJson::YLT_REF).toStdString();
      break;
    case SOURCE_TYPE_MIN:
      src_str += "MIN";
      break;
    case SOURCE_TYPE_MAX:
      src_str += "MAX";
      break;
    case SOURCE_TYPE_SWITCH:
      src_str += Boards::getSwitchYamlName(rhs.index, BoardJson::YLT_REF).toStdString();
      break;
    case SOURCE_TYPE_CUSTOM_SWITCH:
      src_str += "ls(";
      src_str += std::to_string(rhs.index + 1);
      src_str += ")";
      break;
    case SOURCE_TYPE_CYC:
      src_str = b.getRawSourceCyclicTag(rhs.index);
      break;
    case SOURCE_TYPE_PPM:
      src_str += "tr(";
      src_str += std::to_string(rhs.index);
      src_str += ")";
      break;
    case SOURCE_TYPE_CH:
      src_str += "ch(";
      src_str += std::to_string(rhs.index);
      src_str += ")";
      break;
    case SOURCE_TYPE_GVAR:
      src_str += "gv(";
      src_str += std::to_string(rhs.index);
      src_str += ")";
      break;
    case SOURCE_TYPE_SPECIAL:
      src_str = b.getRawSourceSpecialTypeTag(rhs.index);
      break;
    case SOURCE_TYPE_TELEMETRY:
      src_str = "tele(";
      switch (rhs.index % 3) {
        case 0:
          break;
        case 1:
          src_str += '-';
          break;
        case 2:
          src_str += '+';
          break;
      }
      src_str += std::to_string(rhs.index / 3);
      src_str += ")";
      break;
    case SOURCE_TYPE_SPACEMOUSE:
      src_str = "SPACEMOUSE_";
      c += rhs.index;
      src_str += c;
      break;
    default:
      src_str = "NONE";
      break;
  }
  return src_str;
}

RawSource YamlRawSourceDecode(const std::string& src_str)
{
  Board::Type board = getCurrentBoard();
  Boards b = Boards(board);
  RawSource rhs;
  const char* val = src_str.data();
  size_t val_len = src_str.size();

  if (val_len > 1 && val[0] == 'I'
      && (val[1] >= '0') && (val[1] <= '9')) {

    int idx = std::stoi(src_str.substr(1));
    if (idx < CPN_MAX_INPUTS) {
      rhs = RawSource(SOURCE_TYPE_VIRTUAL_INPUT, idx);
    }

  } else if ((val_len == 2 &&
              val[0] == 'S' &&
              val[1] >= 'A' && val[1] <= 'Z') ||
             (val_len == 3 && (
              (val[0] == 'F' && val[1] == 'L') ||
              (val[0] == 'S' && val[1] == 'W')) &&
              val[2] >= '1' && val[2] <= '9')) {

    int idx = Boards::getSwitchYamlIndex(src_str.c_str(), BoardJson::YLT_REF);
    if (idx >= 0) {
      rhs = RawSource(SOURCE_TYPE_SWITCH, idx);
    }

  } else if (val_len > 4 &&
             val[0] == 'l' &&
             val[1] == 'u' &&
             val[2] == 'a' &&
             val[3] == '(') {

    std::stringstream src(src_str.substr(4));
    int script = 0, output = 0;                   //  TODO: check rename outputs to inputs???
    src >> script;
    src.ignore();
    src >> output;
    if (script < CPN_MAX_SCRIPTS && output < CPN_MAX_SCRIPT_INPUTS)
      rhs = RawSource(SOURCE_TYPE_LUA_OUTPUT, script * 16 + output);

  } else if (val_len > 3 &&
             val[0] == 'l' &&
             val[1] == 's' &&
             val[2] == '(') {

    std::stringstream src(src_str.substr(3));
    int ls = 0;
    src >> ls;
    if (ls > 0 && ls <= CPN_MAX_LOGICAL_SWITCHES)
      rhs = RawSource(SOURCE_TYPE_CUSTOM_SWITCH, ls - 1);

    // appears depreciated - maybe early support for T20 as SWn the current std and no match in encode
  } else if (val_len > 3 &&
             val[0] == 'f' &&
             val[1] == 's' &&
             val[2] == '(') {

    std::stringstream src(src_str.substr(3));
    int fs = 0;
    src >> fs;
    if (fs > 0) {
      int fsidx = Boards::getSwitchYamlIndex(QString("SW%1").arg(fs), BoardJson::YLT_REF);
      if (fsidx >= 0)
        rhs = RawSource(SOURCE_TYPE_SWITCH, fsidx);
    }

  } else if (val_len > 3 &&
             val[0] == 't' &&
             val[1] == 'r' &&
             val[2] == '(') {

    std::stringstream src(src_str.substr(3));
    int tr = 0;
    src >> tr;
    if (tr < getCurrentFirmware()->getCapability(TrainerInputs))
      rhs = RawSource(SOURCE_TYPE_PPM, tr);

  } else if (val_len > 3 &&
             val[0] == 'c' &&
             val[1] == 'h' &&
             val[2] == '(') {

    std::stringstream src(src_str.substr(3));
    int ch = 0;
    src >> ch;
    if (ch < CPN_MAX_CHNOUT)
      rhs = RawSource(SOURCE_TYPE_CH, ch);

  } else if (val_len > 3 &&
             val[0] == 'g' &&
             val[1] == 'v' &&
             val[2] == '(') {

    std::stringstream src(src_str.substr(3));
    int gv = 0;
    src >> gv;
    if (gv < CPN_MAX_GVARS)
      rhs = RawSource(SOURCE_TYPE_GVAR, gv);

  } else if (val_len > 5 &&
             val[0] == 't' &&
             val[1] == 'e' &&
             val[2] == 'l' &&
             val[3] == 'e' &&
             val[4] == '(') {

    std::stringstream src(src_str.substr(5));

    // parse sign
    uint8_t sign = 0;
    char c = src.peek();
    if (c == '-') {
      sign = 1;
      src.ignore();
    } else if (c == '+') {
      sign = 2;
      src.ignore();
    }

    int sensor = 0;
    src >> sensor;
    if (sensor < CPN_MAX_SENSORS)
      rhs = RawSource(SOURCE_TYPE_TELEMETRY, sensor * 3 + sign);

  } else {

    YAML::Node node(src_str);

    if (node.IsScalar() && node.as<std::string>() == "MAX") {
      rhs.type = SOURCE_TYPE_MAX;
      rhs.index = 0;
      return rhs;
    }

    if (node.IsScalar() && node.as<std::string>() == "MIN") {
      rhs.type = SOURCE_TYPE_MIN;
      rhs.index = 0;
      return rhs;
    }

    std::string ana_str;
    node >> ana_str;

    if (radioSettingsVersion < SemanticVersion(QString(CPN_ADC_REFACTOR_VERSION))) {
      ana_str = Boards::getLegacyAnalogMappedInputTag(ana_str.c_str());
      int idx = Boards::getInputIndex(ana_str.c_str(), Board::LVT_TAG);
      if (idx >= 0 && Boards::isInputStick(idx))
        ana_str = Boards::getInputName(idx).toStdString();
    }

    int ana_idx = Boards::getInputYamlIndex(ana_str.c_str(), BoardJson::YLT_REF);
    if (ana_idx >= 0) {
      rhs.type = SOURCE_TYPE_STICK;
      rhs.index = ana_idx;
      return rhs;
    }

    std::string trim_str;
    node >> trim_str;

    if (radioSettingsVersion < SemanticVersion(QString(CPN_ADC_REFACTOR_VERSION))) {
      int idx = b.getLegacyTrimSourceIndex(src_str.c_str());
      if (idx >= 0)
        trim_str = Boards::getTrimYamlName(idx, BoardJson::YLT_REF).toStdString();
    }

    int trm_idx = Boards::getTrimYamlIndex(trim_str.c_str(), BoardJson::YLT_REF);
    if (trm_idx >= 0) {
      rhs.type = SOURCE_TYPE_TRIM;
      rhs.index = trm_idx;
      return rhs;
    }

    std::string special_str;
    node >> special_str;

    if (radioSettingsVersion < SemanticVersion(QString(CPN_ADC_REFACTOR_VERSION))) {
      if (special_str.size() == 6 && special_str.substr(0, 5) == "TIMER") {
        special_str = "Tmr" + special_str.substr(5, 1);
      }
    }

    int sp_idx = b.getRawSourceSpecialTypeIndex(special_str.c_str());
    if (sp_idx >= 0) {
      rhs.type = SOURCE_TYPE_SPECIAL;
      rhs.index = sp_idx;
      return rhs;
    }

    int cyc_idx = b.getRawSourceCyclicIndex(src_str.c_str());
    if (cyc_idx >= 0) {
      rhs.type = SOURCE_TYPE_CYC;
      rhs.index = cyc_idx;
      return rhs;
    }

    if (node.IsScalar() && node.as<std::string>().size() == 12 && node.as<std::string>().substr(0, 11) == "SPACEMOUSE_") {
      int sm_idx = 0;
      node >> spacemouseLut >> sm_idx;
      if (sm_idx >= 0) {
        rhs.type = SOURCE_TYPE_SPACEMOUSE;
        rhs.index = sm_idx;
        return rhs;
      }
    }
  }

  return rhs;
}

namespace YAML {
Node convert<RawSource>::encode(const RawSource& rhs)
{
  return Node(YamlRawSourceEncode(rhs));
}

bool convert<RawSource>::decode(const Node& node, RawSource& rhs)
{
  std::string src_str = node.Scalar();
  rhs = YamlRawSourceDecode(src_str);
  return true;
}
}  // namespace YAML
